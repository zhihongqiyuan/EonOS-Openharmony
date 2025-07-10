// Copyright (c) 2023 Huawei Device Co., Ltd.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use std::fs::File;
use std::future::Future;
use std::io::IoSlice;
use std::os::unix::io::{AsFd, AsRawFd, BorrowedFd, IntoRawFd, OwnedFd, RawFd};
use std::pin::Pin;
use std::process::{Child as StdChild, ExitStatus, Stdio};
use std::task::{Context, Poll};
use std::{fmt, io};

use super::pipe::Pipe;
use crate::io::{AsyncRead, AsyncWrite, ReadBuf};
use crate::net::AsyncSource;
use crate::signal::{signal, Signal, SignalKind};

pub(crate) struct Child {
    std: Option<StdChild>,
    signal: Signal,
}

impl Child {
    pub(crate) fn new(child: StdChild) -> io::Result<Self> {
        Ok(Child {
            std: Some(child),
            signal: signal(SignalKind::child())?,
        })
    }

    pub(crate) fn id(&self) -> u32 {
        // std is always Some before drop.
        self.std.as_ref().unwrap().id()
    }

    pub(crate) fn kill(&mut self) -> io::Result<()> {
        // std is always Some before drop.
        self.std.as_mut().unwrap().kill()
    }

    pub(crate) fn try_wait(&mut self) -> io::Result<Option<ExitStatus>> {
        // std is always Some before drop.
        self.std.as_mut().unwrap().try_wait()
    }
}

impl fmt::Debug for Child {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.std.fmt(f)
    }
}

impl Future for Child {
    type Output = io::Result<ExitStatus>;

    fn poll(mut self: Pin<&mut Self>, cx: &mut Context<'_>) -> Poll<Self::Output> {
        loop {
            let signal_pending = self.signal.poll_recv(cx).is_pending();

            if let Some(status) = self.try_wait()? {
                return Poll::Ready(Ok(status));
            }

            if signal_pending {
                return Poll::Pending;
            }
        }
    }
}

impl Drop for Child {
    fn drop(&mut self) {
        if let Ok(Some(_)) = self.try_wait() {
            return;
        }
        // std is always Some before drop.
        let std = self.std.take().unwrap();
        crate::process::GlobalZombieChild::get_instance().push(std);
    }
}

pub(crate) fn stdio<T: IntoRawFd>(io: T) -> io::Result<ChildStdio> {
    let mut pipe = Pipe::from(io);
    set_nonblock(&mut pipe, true)?;

    AsyncSource::new(pipe, None).map(|inner| ChildStdio { inner })
}

fn to_blocking_file(io: ChildStdio) -> io::Result<File> {
    let mut file = io.inner.io_take()?.fd;
    set_nonblock(&mut file, false)?;
    Ok(file)
}

fn set_nonblock<T: AsRawFd>(fd: &mut T, block: bool) -> io::Result<()> {
    unsafe {
        let fd = fd.as_raw_fd();
        let mut flags = libc::fcntl(fd, libc::F_GETFL);
        if flags == -1 {
            return Err(io::Error::last_os_error());
        }
        if block {
            flags |= libc::O_NONBLOCK;
        } else {
            flags &= !libc::O_NONBLOCK;
        }
        match libc::fcntl(fd, libc::F_SETFL, flags) {
            -1 => Err(io::Error::last_os_error()),
            _ => Ok(()),
        }
    }
}

pub(crate) fn to_stdio(io: ChildStdio) -> io::Result<Stdio> {
    to_blocking_file(io).map(Stdio::from)
}

#[derive(Debug)]
pub(crate) struct ChildStdio {
    inner: AsyncSource<Pipe>,
}

impl ChildStdio {
    pub(crate) fn into_owned_fd(self) -> io::Result<OwnedFd> {
        to_blocking_file(self).map(OwnedFd::from)
    }
}

impl AsyncWrite for ChildStdio {
    fn poll_write(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &[u8],
    ) -> Poll<io::Result<usize>> {
        self.inner.poll_write(cx, buf)
    }

    fn poll_write_vectored(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        bufs: &[IoSlice<'_>],
    ) -> Poll<io::Result<usize>> {
        self.inner.poll_write_vectored(cx, bufs)
    }

    fn is_write_vectored(&self) -> bool {
        true
    }

    fn poll_flush(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<io::Result<()>> {
        Poll::Ready(Ok(()))
    }

    fn poll_shutdown(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<io::Result<()>> {
        Poll::Ready(Ok(()))
    }
}

impl AsyncRead for ChildStdio {
    fn poll_read(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &mut ReadBuf<'_>,
    ) -> Poll<io::Result<()>> {
        self.inner.poll_read(cx, buf)
    }
}

impl AsRawFd for ChildStdio {
    fn as_raw_fd(&self) -> RawFd {
        AsRawFd::as_raw_fd(&*(self.inner))
    }
}

impl AsFd for ChildStdio {
    fn as_fd(&self) -> BorrowedFd<'_> {
        unsafe { BorrowedFd::borrow_raw(self.as_raw_fd()) }
    }
}

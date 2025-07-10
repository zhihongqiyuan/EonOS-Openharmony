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

use std::io;
use std::mem::{self, size_of, MaybeUninit};
use std::net::{self, SocketAddr};
use std::os::unix::io::{AsRawFd, FromRawFd, RawFd};
use std::time::Duration;

use libc::{
    c_int, c_void, linger, socklen_t, AF_INET, AF_INET6, SOCK_STREAM, SOL_SOCKET, SO_LINGER,
    SO_REUSEADDR,
};

use super::super::socket_addr::socket_addr_trans;
use super::TcpStream;
use crate::source::Fd;
use crate::sys::unix::socket::socket_new;

pub(crate) struct TcpSocket {
    socket: c_int,
}

impl TcpSocket {
    pub(crate) fn new_socket(addr: SocketAddr) -> io::Result<TcpSocket> {
        if addr.is_ipv4() {
            TcpSocket::create_socket(AF_INET, SOCK_STREAM)
        } else {
            TcpSocket::create_socket(AF_INET6, SOCK_STREAM)
        }
    }

    pub(crate) fn create_socket(domain: c_int, socket_type: c_int) -> io::Result<TcpSocket> {
        let socket = socket_new(domain, socket_type)?;
        Ok(TcpSocket {
            socket: socket as c_int,
        })
    }

    pub(crate) fn set_reuse(&self, is_reuse: bool) -> io::Result<()> {
        let set_value: c_int = i32::from(is_reuse);

        match syscall!(setsockopt(
            self.socket,
            SOL_SOCKET,
            SO_REUSEADDR,
            (&set_value as *const c_int).cast::<c_void>(),
            size_of::<c_int>() as socklen_t
        )) {
            Err(err) => Err(err),
            Ok(_) => Ok(()),
        }
    }

    pub(crate) fn bind(&self, addr: SocketAddr) -> io::Result<()> {
        let (raw_addr, addr_length) = socket_addr_trans(&addr);
        match syscall!(bind(self.socket, raw_addr.as_ptr(), addr_length)) {
            Err(err) => Err(err),
            Ok(_) => Ok(()),
        }
    }

    pub(crate) fn listen(self, max_connect: c_int) -> io::Result<()> {
        syscall!(listen(self.socket, max_connect))?;
        Ok(())
    }

    pub(crate) fn connect(self, addr: SocketAddr) -> io::Result<TcpStream> {
        let stream = TcpStream {
            inner: unsafe { net::TcpStream::from_raw_fd(self.socket) },
        };
        let (raw_addr, addr_length) = socket_addr_trans(&addr);
        match syscall!(connect(self.socket, raw_addr.as_ptr(), addr_length)) {
            Err(err) if err.raw_os_error() != Some(libc::EINPROGRESS) => Err(err),
            _ => Ok(stream),
        }
    }
}

impl AsRawFd for TcpSocket {
    fn as_raw_fd(&self) -> RawFd {
        self.socket
    }
}

impl FromRawFd for TcpSocket {
    unsafe fn from_raw_fd(fd: RawFd) -> TcpSocket {
        TcpSocket { socket: fd }
    }
}

pub(crate) fn get_sock_linger(fd: Fd) -> io::Result<Option<Duration>> {
    let mut payload: MaybeUninit<linger> = MaybeUninit::uninit();
    let mut len = mem::size_of::<linger>() as libc::socklen_t;

    syscall!(getsockopt(
        fd as c_int,
        SOL_SOCKET,
        SO_LINGER,
        payload.as_mut_ptr().cast(),
        &mut len,
    ))
    .map(|_| {
        let linger = unsafe { payload.assume_init() };
        from_linger(linger)
    })
}

pub(crate) fn set_sock_linger(fd: Fd, duration: Option<Duration>) -> io::Result<()> {
    let payload = into_linger(duration);
    syscall!(setsockopt(
        fd as c_int,
        SOL_SOCKET,
        SO_LINGER,
        (&payload as *const linger).cast::<c_void>(),
        mem::size_of::<linger>() as libc::socklen_t,
    ))
    .map(|_| ())
}

fn from_linger(linger: linger) -> Option<Duration> {
    if linger.l_onoff == 0 {
        None
    } else {
        Some(Duration::from_secs(linger.l_linger as u64))
    }
}

fn into_linger(duration: Option<Duration>) -> linger {
    match duration {
        None => linger {
            l_onoff: 0,
            l_linger: 0,
        },
        Some(dur) => linger {
            l_onoff: 1,
            l_linger: dur.as_secs() as _,
        },
    }
}

#[cfg(test)]
mod test {
    use std::ffi::c_int;
    use std::os::fd::{AsRawFd, FromRawFd};

    use crate::sys::unix::tcp::socket::into_linger;
    use crate::sys::unix::tcp::TcpSocket;

    /// UT for `into_linger`
    ///
    /// # Brief
    /// 1. Call `into_linger` with parameter None
    /// 2. Check if the returned linger is correct
    #[test]
    fn ut_into_linger_none() {
        let linger = into_linger(None);
        assert_eq!(linger.l_linger, 0);
        assert_eq!(linger.l_linger, 0);
    }
}

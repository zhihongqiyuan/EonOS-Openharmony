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
use std::os::fd::{AsRawFd, FromRawFd};
use std::os::unix::net;
use std::path::Path;

use libc::AF_UNIX;

use super::socket_addr::socket_addr_trans_un;
#[cfg(target_os = "macos")]
use crate::sys::socket::set_non_block;
use crate::sys::socket::socket_new;

pub(crate) fn bind(path: &Path) -> io::Result<net::UnixListener> {
    let (socket_addr, addr_length) = socket_addr_trans_un(path)?;
    let socket_addr = (&socket_addr as *const libc::sockaddr_un).cast::<libc::sockaddr>();

    let socket = socket_new(AF_UNIX, libc::SOCK_STREAM)?;
    let net = unsafe { net::UnixListener::from_raw_fd(socket) };

    syscall!(bind(socket, socket_addr, addr_length))?;
    // set backlog
    syscall!(listen(socket, 1024))?;

    Ok(net)
}

pub(crate) fn connect(path: &Path) -> io::Result<net::UnixStream> {
    let (sockaddr, addr_length) = socket_addr_trans_un(path)?;
    let sockaddr = (&sockaddr as *const libc::sockaddr_un).cast::<libc::sockaddr>();

    let socket = socket_new(AF_UNIX, libc::SOCK_STREAM)?;
    let net = unsafe { net::UnixStream::from_raw_fd(socket) };
    match syscall!(connect(socket, sockaddr, addr_length)) {
        Err(err) if err.raw_os_error() != Some(libc::EINPROGRESS) => Err(err),
        _ => Ok(net),
    }
}

pub(crate) fn unbound() -> io::Result<net::UnixDatagram> {
    let socket = socket_new(AF_UNIX, libc::SOCK_DGRAM)?;
    let net = unsafe { net::UnixDatagram::from_raw_fd(socket) };
    Ok(net)
}

pub(crate) fn data_gram_bind(path: &Path) -> io::Result<net::UnixDatagram> {
    let (socket_addr, addr_length) = socket_addr_trans_un(path)?;
    let socket_addr = (&socket_addr as *const libc::sockaddr_un).cast::<libc::sockaddr>();

    let socket = unbound()?;
    match syscall!(bind(socket.as_raw_fd(), socket_addr, addr_length)) {
        Err(err) => Err(err),
        Ok(_) => Ok(socket),
    }
}

pub(crate) fn stream_pair() -> io::Result<(net::UnixStream, net::UnixStream)> {
    pair(libc::SOCK_STREAM)
}

pub(crate) fn datagram_pair() -> io::Result<(net::UnixDatagram, net::UnixDatagram)> {
    pair(libc::SOCK_DGRAM)
}

fn pair<T: FromRawFd>(socket_type: libc::c_int) -> io::Result<(T, T)> {
    #[cfg(target_os = "linux")]
    let socket_type = socket_type | libc::SOCK_NONBLOCK | libc::SOCK_CLOEXEC;

    // uninitialized fd
    let mut fds = [-1; 2];
    syscall!(socketpair(AF_UNIX, socket_type, 0, fds.as_mut_ptr()))?;

    #[cfg(target_os = "macos")]
    {
        set_non_block(fds[0])?;
        set_non_block(fds[1])?;
    }

    Ok(unsafe { (T::from_raw_fd(fds[0]), T::from_raw_fd(fds[1])) })
}

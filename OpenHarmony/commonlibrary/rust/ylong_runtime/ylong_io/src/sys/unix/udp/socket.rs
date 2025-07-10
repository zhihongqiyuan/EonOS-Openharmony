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

use std::net::SocketAddr;
use std::os::unix::io::{AsRawFd, FromRawFd, RawFd};
use std::{io, net};

use libc::{c_int, AF_INET, AF_INET6, SOCK_DGRAM};

use super::super::socket_addr::socket_addr_trans;
use crate::sys::socket::socket_new;
use crate::UdpSocket;

pub(crate) struct UdpSock {
    socket: c_int,
}

impl UdpSock {
    pub(crate) fn new_socket(addr: SocketAddr) -> io::Result<UdpSock> {
        if addr.is_ipv4() {
            UdpSock::create_socket(AF_INET, SOCK_DGRAM)
        } else {
            UdpSock::create_socket(AF_INET6, SOCK_DGRAM)
        }
    }

    pub(crate) fn create_socket(domain: c_int, socket_type: c_int) -> io::Result<UdpSock> {
        let socket = socket_new(domain, socket_type)?;
        Ok(UdpSock {
            socket: socket as c_int,
        })
    }

    pub(crate) fn bind(self, addr: SocketAddr) -> io::Result<UdpSocket> {
        let udp_socket = UdpSocket {
            inner: unsafe { net::UdpSocket::from_raw_fd(self.socket) },
        };
        let (raw_addr, addr_length) = socket_addr_trans(&addr);
        match syscall!(bind(self.socket, raw_addr.as_ptr(), addr_length)) {
            Err(err) if err.raw_os_error() != Some(libc::EINPROGRESS) => Err(err),
            _ => Ok(udp_socket),
        }
    }
}

impl AsRawFd for UdpSock {
    fn as_raw_fd(&self) -> RawFd {
        self.socket
    }
}

impl FromRawFd for UdpSock {
    unsafe fn from_raw_fd(fd: RawFd) -> UdpSock {
        UdpSock { socket: fd }
    }
}

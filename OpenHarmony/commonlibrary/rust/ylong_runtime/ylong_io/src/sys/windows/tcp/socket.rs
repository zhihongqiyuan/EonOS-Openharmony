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

use std::mem::MaybeUninit;
use std::net::SocketAddr;
use std::os::windows::io::{AsRawSocket, FromRawSocket, RawSocket};
use std::time::Duration;
use std::{io, mem};

use libc::{c_int, getsockopt};

use crate::sys::winapi::{
    closesocket, ioctlsocket, setsockopt, socket, ADDRESS_FAMILY, AF_INET, AF_INET6, FIONBIO,
    INVALID_SOCKET, LINGER, SOCKET, SOCKET_ERROR, SOCK_STREAM, SOL_SOCKET, SO_LINGER,
};
use crate::sys::windows::net::init;
use crate::sys::windows::socket_addr::socket_addr_trans;

pub(crate) struct TcpSocket {
    socket: SOCKET,
}

impl TcpSocket {
    /// Gets new socket
    pub(crate) fn new_socket(addr: SocketAddr) -> io::Result<TcpSocket> {
        if addr.is_ipv4() {
            Self::create_socket(AF_INET, SOCK_STREAM)
        } else {
            Self::create_socket(AF_INET6, SOCK_STREAM)
        }
    }

    fn create_socket(domain: ADDRESS_FAMILY, socket_type: u16) -> io::Result<TcpSocket> {
        init();

        let socket = socket_syscall!(
            socket(domain as i32, socket_type as i32, 0),
            PartialEq::eq,
            INVALID_SOCKET
        )?;

        match socket_syscall!(ioctlsocket(socket, FIONBIO, &mut 1), PartialEq::ne, 0) {
            Err(err) => {
                let _ = unsafe { closesocket(socket) };
                Err(err)
            }
            Ok(_) => Ok(TcpSocket {
                socket: socket as SOCKET,
            }),
        }
    }

    /// System call to bind Socket.
    pub(crate) fn bind(&self, addr: SocketAddr) -> io::Result<()> {
        use crate::sys::winapi::bind;

        let (raw_addr, raw_addr_length) = socket_addr_trans(&addr);
        socket_syscall!(
            bind(self.socket as _, raw_addr.as_ptr(), raw_addr_length),
            PartialEq::eq,
            SOCKET_ERROR
        )?;
        Ok(())
    }

    /// System call to listen.
    pub(crate) fn listen(self, backlog: u32) -> io::Result<()> {
        use std::convert::TryInto;

        use crate::sys::winapi::listen;

        let backlog = backlog.try_into().unwrap_or(i32::MAX);
        socket_syscall!(
            listen(self.socket as _, backlog),
            PartialEq::eq,
            SOCKET_ERROR
        )?;
        Ok(())
    }

    /// System call to connect.
    pub(crate) fn connect(self, addr: SocketAddr) -> io::Result<()> {
        use crate::sys::winapi::connect;

        let (socket_addr, socket_addr_length) = socket_addr_trans(&addr);
        let res = socket_syscall!(
            connect(self.socket as _, socket_addr.as_ptr(), socket_addr_length),
            PartialEq::eq,
            SOCKET_ERROR
        );

        match res {
            Err(e) if e.kind() != io::ErrorKind::WouldBlock => Err(e),
            _ => Ok(()),
        }
    }
}

impl AsRawSocket for TcpSocket {
    fn as_raw_socket(&self) -> RawSocket {
        self.socket as RawSocket
    }
}

impl FromRawSocket for TcpSocket {
    unsafe fn from_raw_socket(sock: RawSocket) -> Self {
        TcpSocket {
            socket: sock as SOCKET,
        }
    }
}

pub(crate) fn get_sock_linger(socket: RawSocket) -> io::Result<Option<Duration>> {
    let mut optval: MaybeUninit<LINGER> = MaybeUninit::uninit();
    let mut optlen = mem::size_of::<LINGER>() as c_int;

    socket_syscall!(
        getsockopt(
            socket as SOCKET,
            SOL_SOCKET as c_int,
            SO_LINGER as c_int,
            optval.as_mut_ptr().cast(),
            &mut optlen,
        ),
        PartialEq::eq,
        SOCKET_ERROR
    )
    .map(|_| {
        let linger = unsafe { optval.assume_init() };
        from_linger(linger)
    })
}

pub(crate) fn set_sock_linger(socket: RawSocket, linger: Option<Duration>) -> io::Result<()> {
    let optval = into_linger(linger);
    socket_syscall!(
        setsockopt(
            socket as SOCKET,
            SOL_SOCKET as c_int,
            SO_LINGER as c_int,
            (&optval as *const LINGER).cast(),
            mem::size_of::<LINGER>() as c_int,
        ),
        PartialEq::eq,
        SOCKET_ERROR
    )
    .map(|_| ())
}

fn from_linger(linger: LINGER) -> Option<Duration> {
    if linger.l_onoff == 0 {
        None
    } else {
        Some(Duration::from_secs(linger.l_linger as u64))
    }
}

fn into_linger(linger: Option<Duration>) -> LINGER {
    match linger {
        None => LINGER {
            l_onoff: 0,
            l_linger: 0,
        },
        Some(dur) => LINGER {
            l_onoff: 1,
            l_linger: dur.as_secs() as _,
        },
    }
}

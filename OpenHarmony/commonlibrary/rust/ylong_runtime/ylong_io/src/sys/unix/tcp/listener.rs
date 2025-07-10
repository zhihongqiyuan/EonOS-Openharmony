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
use std::mem::{size_of, MaybeUninit};
use std::net::{self, SocketAddr};
use std::os::unix::io::{AsRawFd, FromRawFd};

use libc::{c_int, sockaddr_in, sockaddr_in6, sockaddr_storage, socklen_t};

use super::{TcpSocket, TcpStream};
use crate::source::Fd;
#[cfg(target_os = "macos")]
use crate::sys::socket::set_non_block;
use crate::{Interest, Selector, Source, Token};

/// A socket server.
pub struct TcpListener {
    pub(crate) inner: net::TcpListener,
}

impl TcpListener {
    /// Binds a new tcp Listener to the specific address to receive connection
    /// requests.
    ///
    /// The socket will be set to `SO_REUSEADDR`.
    pub fn bind(addr: SocketAddr) -> io::Result<TcpListener> {
        let socket = TcpSocket::new_socket(addr)?;
        let listener = TcpListener {
            inner: unsafe { net::TcpListener::from_raw_fd(socket.as_raw_fd()) },
        };
        socket.set_reuse(true)?;
        socket.bind(addr)?;
        socket.listen(1024)?;
        Ok(listener)
    }

    /// Accepts connections and returns the `TcpStream` and the remote peer
    /// address.
    ///
    /// # Error
    /// This may return an `Err(e)` where `e.kind()` is
    /// `io::ErrorKind::WouldBlock`. This means a stream may be ready at a
    /// later point and one should wait for an event before calling `accept`
    /// again.
    pub fn accept(&self) -> io::Result<(TcpStream, SocketAddr)> {
        let mut addr: MaybeUninit<sockaddr_storage> = MaybeUninit::uninit();
        let mut length = size_of::<sockaddr_storage>() as socklen_t;

        #[cfg(target_os = "linux")]
        let stream = match syscall!(accept4(
            self.inner.as_raw_fd(),
            addr.as_mut_ptr().cast::<_>(),
            &mut length,
            libc::SOCK_CLOEXEC | libc::SOCK_NONBLOCK,
        )) {
            Ok(socket) => unsafe { net::TcpStream::from_raw_fd(socket) },
            Err(err) => {
                return Err(err);
            }
        };

        #[cfg(target_os = "macos")]
        let stream = match syscall!(accept(
            self.inner.as_raw_fd(),
            addr.as_mut_ptr() as *mut _,
            &mut length
        )) {
            Ok(socket) => {
                set_non_block(socket)?;
                unsafe { net::TcpStream::from_raw_fd(socket) }
            }
            Err(e) => return Err(e),
        };

        let ret = unsafe { trans_addr_2_socket(addr.as_ptr()) };

        match ret {
            Ok(sockaddr) => Ok((TcpStream::from_std(stream), sockaddr)),
            Err(err) => Err(err),
        }
    }

    /// Returns the local socket address of this listener.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let mut server = TcpListener::bind(addr).unwrap();
    /// let ret = server.local_addr().unwrap();
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Gets the value of the IP_TTL option for this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let mut server = TcpListener::bind(addr).unwrap();
    /// let ret = server.ttl().unwrap();
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.inner.ttl()
    }

    /// Sets the value for the IP_TTL option on this socket.
    /// This value sets the time-to-live field that is used in every packet sent
    /// from this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let mut server = TcpListener::bind(addr).unwrap();
    /// let ret = server.set_ttl(100).unwrap();
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.inner.set_ttl(ttl)
    }

    /// Gets the value of the SO_ERROR option on this socket.
    /// This will retrieve the stored error in the underlying socket, clearing
    /// the field in the process. This can be useful for checking errors between
    /// calls.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let mut server = TcpListener::bind(addr).unwrap();
    /// let ret = server.take_error().unwrap();
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.inner.take_error()
    }
}

pub(crate) unsafe fn trans_addr_2_socket(
    storage: *const libc::sockaddr_storage,
) -> io::Result<SocketAddr> {
    match (*storage).ss_family as c_int {
        libc::AF_INET => Ok(SocketAddr::V4(*(storage.cast::<sockaddr_in>().cast::<_>()))),
        libc::AF_INET6 => Ok(SocketAddr::V6(
            *(storage.cast::<sockaddr_in6>().cast::<_>()),
        )),
        _ => {
            let err = io::Error::new(io::ErrorKind::Other, "Cannot transfer address into socket.");
            Err(err)
        }
    }
}

impl Source for TcpListener {
    fn register(
        &mut self,
        selector: &Selector,
        token: Token,
        interests: Interest,
    ) -> io::Result<()> {
        selector.register(self.get_fd(), token, interests)
    }

    fn deregister(&mut self, selector: &Selector) -> io::Result<()> {
        selector.deregister(self.get_fd())
    }

    fn get_fd(&self) -> Fd {
        self.inner.as_raw_fd()
    }
}

impl std::fmt::Debug for TcpListener {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.inner.fmt(f)
    }
}

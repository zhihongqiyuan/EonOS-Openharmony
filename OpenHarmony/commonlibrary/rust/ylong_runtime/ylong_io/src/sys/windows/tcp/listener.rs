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

use std::fmt::Formatter;
use std::net::SocketAddr;
use std::os::windows::io::{AsRawSocket, FromRawSocket, IntoRawSocket, RawSocket};
use std::{fmt, io, net};

use crate::source::Fd;
use crate::sys::windows::tcp::TcpSocket;
use crate::sys::NetState;
use crate::{Interest, Selector, Source, TcpStream, Token};

/// A TCP socket server, listening for connections.
pub struct TcpListener {
    pub(crate) inner: net::TcpListener,
    /// State is None if the socket has not been Registered.
    pub(crate) state: NetState,
}

impl TcpListener {
    /// Binds a new tcp Listener to the specific address to receive connection
    /// requests.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let server = TcpListener::bind(addr).unwrap();
    /// ```
    pub fn bind(addr: SocketAddr) -> io::Result<TcpListener> {
        let socket = TcpSocket::new_socket(addr)?;
        let listener = unsafe { TcpListener::from_raw_socket(socket.as_raw_socket() as _) };

        socket.bind(addr)?;
        socket.listen(1024)?;
        Ok(listener)
    }

    /// Creates `TcpListener` from raw TcpListener.
    pub fn from_std(listener: net::TcpListener) -> TcpListener {
        TcpListener {
            inner: listener,
            state: NetState::new(),
        }
    }

    /// Accepts connections and returns the `TcpStream` and the remote peer
    /// address.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpListener;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let mut server = TcpListener::bind(addr).unwrap();
    /// let ret = server.accept().unwrap();
    /// ```
    pub fn accept(&self) -> io::Result<(TcpStream, SocketAddr)> {
        self.state
            .try_io(|inner| inner.accept(), &self.inner)
            .map(|(stream, addr)| (TcpStream::from_std(stream), addr))
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

impl Source for TcpListener {
    fn register(
        &mut self,
        selector: &Selector,
        token: Token,
        interests: Interest,
    ) -> io::Result<()> {
        self.state
            .register(selector, token, interests, self.as_raw_socket())
    }

    fn deregister(&mut self, _selector: &Selector) -> io::Result<()> {
        self.state.deregister()
    }

    fn get_fd(&self) -> Fd {
        self.inner.as_raw_socket()
    }
}

impl fmt::Debug for TcpListener {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        self.inner.fmt(f)
    }
}

impl IntoRawSocket for TcpListener {
    fn into_raw_socket(self) -> RawSocket {
        self.inner.into_raw_socket()
    }
}

impl AsRawSocket for TcpListener {
    fn as_raw_socket(&self) -> RawSocket {
        self.inner.as_raw_socket()
    }
}

impl FromRawSocket for TcpListener {
    /// Converts a `RawSocket` to a `TcpListener`.
    unsafe fn from_raw_socket(socket: RawSocket) -> TcpListener {
        TcpListener::from_std(FromRawSocket::from_raw_socket(socket))
    }
}

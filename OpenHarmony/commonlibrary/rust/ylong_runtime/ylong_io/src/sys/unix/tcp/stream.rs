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

use std::io::{self, IoSlice, IoSliceMut, Read, Write};
use std::net::{self, Shutdown, SocketAddr};
use std::os::unix::io::AsRawFd;
use std::time::Duration;

use super::TcpSocket;
use crate::source::Fd;
use crate::sys::unix::tcp::socket::{get_sock_linger, set_sock_linger};
use crate::{Interest, Selector, Source, Token};

/// A non-blocking TCP Stream between a local socket and a remote socket.
pub struct TcpStream {
    /// Raw TCP socket
    pub inner: net::TcpStream,
}

impl TcpStream {
    /// Create a new TCP stream and issue a non-blocking connect to the
    /// specified address.
    pub fn connect(addr: SocketAddr) -> io::Result<TcpStream> {
        let socket = TcpSocket::new_socket(addr)?;
        socket.connect(addr)
    }

    /// Creates a new `TcpStream` from a standard `net::TcpStream`.
    pub fn from_std(stream: net::TcpStream) -> TcpStream {
        TcpStream { inner: stream }
    }

    /// Clones the TcpStream.
    pub fn try_clone(&self) -> io::Result<Self> {
        Ok(TcpStream {
            inner: self.inner.try_clone()?,
        })
    }

    /// Returns the socket address of the local half of this TCP connection.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::net::{IpAddr, Ipv4Addr};
    ///
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// assert_eq!(
    ///     stream.local_addr().unwrap().ip(),
    ///     IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1))
    /// );
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Returns the socket address of the remote half of this TCP connection.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// assert_eq!(
    ///     stream.peer_addr().unwrap(),
    ///     SocketAddr::V4(SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 1234))
    /// );
    /// ```
    pub fn peer_addr(&self) -> io::Result<SocketAddr> {
        self.inner.peer_addr()
    }

    /// Sets the value of the `TCP_NODELAY`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// stream.set_nodelay(true).expect("set_nodelay call failed");
    /// ```
    pub fn set_nodelay(&self, nodelay: bool) -> io::Result<()> {
        self.inner.set_nodelay(nodelay)
    }

    /// Gets the value of the `TCP_NODELAY`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// stream.set_nodelay(true).expect("set_nodelay call failed");
    /// assert_eq!(stream.nodelay().unwrap_or(false), true);
    /// ```
    pub fn nodelay(&self) -> io::Result<bool> {
        self.inner.nodelay()
    }

    /// Gets the value of the linger on this socket by getting `SO_LINGER`
    /// option.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// println!("{:?}", stream.linger());
    /// ```
    pub fn linger(&self) -> io::Result<Option<Duration>> {
        get_sock_linger(self.get_fd())
    }

    /// Sets the value of the linger on this socket by setting `SO_LINGER`
    /// option.
    ///
    /// This value controls how the socket close when a stream has unsent data.
    /// If SO_LINGER is set, the socket will still open for the duration as
    /// the system attempts to send pending data. Otherwise, the system may
    /// close the socket immediately, or wait for a default timeout.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// stream.set_linger(None).expect("Sets linger fail.");
    /// ```
    pub fn set_linger(&self, linger: Option<Duration>) -> io::Result<()> {
        set_sock_linger(self.get_fd(), linger)
    }

    /// Sets the value for the `IP_TTL`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// stream.set_ttl(100).expect("set_ttl call failed");
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.inner.set_ttl(ttl)
    }

    /// Gets the value of the `IP_TTL`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// stream.set_ttl(100).expect("set_ttl call failed");
    /// assert_eq!(stream.ttl().unwrap_or(0), 100);
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.inner.ttl()
    }

    /// Gets the value of the `SO_ERROR` option on this socket.
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.inner.take_error()
    }

    /// Shuts down the read, write, or both halves of this connection.
    pub fn shutdown(&self, how: Shutdown) -> io::Result<()> {
        self.inner.shutdown(how)
    }

    /// Same as std::net::TcpStream::peek().
    ///
    /// Receives data on the socket from the remote address to which it is
    /// connected, without removing that data from the queue. On success,
    /// returns the number of bytes peeked.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::TcpStream;
    ///
    /// let addr = "127.0.0.1:1234".parse().unwrap();
    /// let stream = TcpStream::connect(addr).expect("Couldn't connect to the server...");
    /// let mut buf = [0; 10];
    /// let len = stream.peek(&mut buf).expect("peek failed");
    /// ```
    pub fn peek(&self, buf: &mut [u8]) -> io::Result<usize> {
        self.inner.peek(buf)
    }
}

impl Read for TcpStream {
    fn read(&mut self, buf: &mut [u8]) -> io::Result<usize> {
        self.inner.read(buf)
    }

    fn read_vectored(&mut self, bufs: &mut [IoSliceMut<'_>]) -> io::Result<usize> {
        self.inner.read_vectored(bufs)
    }
}

impl Write for TcpStream {
    fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
        self.inner.write(buf)
    }

    fn write_vectored(&mut self, bufs: &[IoSlice<'_>]) -> io::Result<usize> {
        self.inner.write_vectored(bufs)
    }

    fn flush(&mut self) -> io::Result<()> {
        self.inner.flush()
    }
}

impl Read for &TcpStream {
    fn read(&mut self, buf: &mut [u8]) -> io::Result<usize> {
        let mut inner = &self.inner;
        inner.read(buf)
    }

    fn read_vectored(&mut self, bufs: &mut [IoSliceMut<'_>]) -> io::Result<usize> {
        let mut inner = &self.inner;
        inner.read_vectored(bufs)
    }
}

impl Write for &TcpStream {
    fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
        let mut inner = &self.inner;
        inner.write(buf)
    }

    fn write_vectored(&mut self, bufs: &[IoSlice<'_>]) -> io::Result<usize> {
        let mut inner = &self.inner;
        inner.write_vectored(bufs)
    }

    fn flush(&mut self) -> io::Result<()> {
        let mut inner = &self.inner;
        inner.flush()
    }
}

impl std::fmt::Debug for TcpStream {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.inner.fmt(f)
    }
}

impl Source for TcpStream {
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

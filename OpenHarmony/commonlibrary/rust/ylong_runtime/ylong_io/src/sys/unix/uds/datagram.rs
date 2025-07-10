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

use std::os::fd::{AsRawFd, FromRawFd, IntoRawFd, RawFd};
use std::os::unix::net;
use std::path::Path;
use std::{fmt, io};

use crate::source::Fd;
use crate::{Interest, Selector, Source, Token};

/// A Unix datagram socket.
pub struct UnixDatagram {
    pub(crate) inner: net::UnixDatagram,
}

impl UnixDatagram {
    /// Creates a Unix Datagram Socket bound to the given path.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// if let Ok(socket) = UnixDatagram::bind("/socket/path") {
    ///     println!("socket binds successfully");
    /// };
    /// ```
    pub fn bind<P: AsRef<Path>>(path: P) -> io::Result<UnixDatagram> {
        super::socket::data_gram_bind(path.as_ref()).map(Self::from_std)
    }

    /// Creates a Unix Datagram Socket bound from std `UnixDatagram`.
    ///
    /// # Examples
    /// ```no_run
    /// use std::os::unix::net::UnixDatagram;
    ///
    /// use ylong_io::UnixDatagram as YlongUnixDatagram;
    ///
    /// if let Ok(socket) = UnixDatagram::bind("/path/to/the/socket") {
    ///     println!("socket binds successfully");
    ///     let ylong_sock = YlongUnixDatagram::from_std(socket);
    /// };
    /// ```
    pub fn from_std(socket: net::UnixDatagram) -> UnixDatagram {
        UnixDatagram { inner: socket }
    }

    /// Creates a Unix Datagram Socket with no bound.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// let socket = UnixDatagram::unbound();
    /// ```
    pub fn unbound() -> io::Result<UnixDatagram> {
        super::socket::unbound().map(Self::from_std)
    }

    /// Connects the socket to the specified path.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// let socket = UnixDatagram::unbound().unwrap();
    /// if let Ok(socket) = socket.connect("/socket/path") {
    ///     println!("socket connects successfully");
    /// };
    /// ```
    pub fn connect<P: AsRef<Path>>(&self, path: P) -> io::Result<()> {
        self.inner.connect(path)
    }

    /// Creates an unnamed pair of connected sockets.
    /// Returns two `UnixDatagrams`s, they connect to each other.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// if let Ok((sock1, sock2)) = UnixDatagram::pair() {
    ///     println!("unix datagram pair creates successfully");
    /// };
    /// ```
    pub fn pair() -> io::Result<(UnixDatagram, UnixDatagram)> {
        super::socket::datagram_pair()
            .map(|(datagram1, datagram2)| (Self::from_std(datagram1), Self::from_std(datagram2)))
    }

    /// Creates a new independently owned handle to the underlying socket.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::bind("/tmp/sock")?;
    ///     let sock_copy = socket.try_clone().expect("try_clone() fail");
    ///     Ok(())
    /// }
    /// ```
    pub fn try_clone(&self) -> io::Result<UnixDatagram> {
        Ok(Self::from_std(self.inner.try_clone()?))
    }

    /// Returns the local socket address of this UnixDatagram.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::bind("/tmp/sock")?;
    ///     let addr = socket.local_addr().expect("get local_addr() fail");
    ///     Ok(())
    /// }
    /// ```
    pub fn local_addr(&self) -> io::Result<net::SocketAddr> {
        self.inner.local_addr()
    }

    /// Returns the local socket address of this UnixDatagram's peer.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::bind("/tmp/sock")?;
    ///     let addr = socket.peer_addr().expect("get peer_addr() fail");
    ///     Ok(())
    /// }
    /// ```
    pub fn peer_addr(&self) -> io::Result<net::SocketAddr> {
        self.inner.peer_addr()
    }

    /// Receives data from the socket.
    /// On success, returns the number of bytes read and the address from
    /// whence the data came.
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     let mut buf = vec![0; 8];
    ///     let (size, sender) = socket.recv_from(buf.as_mut_slice())?;
    ///     println!("received {size} bytes from {sender:?}");
    ///     Ok(())
    /// }
    /// ```
    pub fn recv_from(&self, buf: &mut [u8]) -> io::Result<(usize, net::SocketAddr)> {
        self.inner.recv_from(buf)
    }

    /// Receives data from the socket.
    /// On success, returns the number of bytes read.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::bind("/socket/path")?;
    ///     let mut buf = vec![0; 8];
    ///     socket.recv(buf.as_mut_slice()).expect("recv failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn recv(&self, buf: &mut [u8]) -> io::Result<usize> {
        self.inner.recv(buf)
    }

    /// Sends data on the socket to the specified address.
    /// On success, returns the number of bytes written.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     socket
    ///         .send_to(b"hello world", "/some/sock")
    ///         .expect("send_to failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn send_to<P: AsRef<Path>>(&self, buf: &[u8], path: P) -> io::Result<usize> {
        self.inner.send_to(buf, path)
    }

    /// Sends data on the socket to the socket's peer.
    /// On success, returns the number of bytes written.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     socket.connect("/some/sock").expect("connect fail");
    ///     socket.send(b"hello world").expect("send_to failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn send(&self, buf: &[u8]) -> io::Result<usize> {
        self.inner.send(buf)
    }

    /// Returns the value of the `SO_ERROR` option.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     if let Ok(Some(err)) = socket.take_error() {
    ///         println!("get error: {err:?}");
    ///     }
    ///     Ok(())
    /// }
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.inner.take_error()
    }

    /// Shuts down this connection.
    ///
    /// # Examples
    /// ```no_run
    /// use std::net::Shutdown;
    ///
    /// use ylong_io::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     socket.shutdown(Shutdown::Both).expect("shutdown() failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn shutdown(&self, how: std::net::Shutdown) -> io::Result<()> {
        self.inner.shutdown(how)
    }
}

impl Source for UnixDatagram {
    fn register(
        &mut self,
        selector: &Selector,
        token: Token,
        interests: Interest,
    ) -> io::Result<()> {
        selector.register(self.inner.as_raw_fd(), token, interests)
    }

    fn deregister(&mut self, selector: &Selector) -> io::Result<()> {
        selector.deregister(self.inner.as_raw_fd())
    }

    fn get_fd(&self) -> Fd {
        self.inner.as_raw_fd()
    }
}

impl fmt::Debug for UnixDatagram {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.inner.fmt(f)
    }
}

impl IntoRawFd for UnixDatagram {
    fn into_raw_fd(self) -> RawFd {
        self.inner.into_raw_fd()
    }
}

impl FromRawFd for UnixDatagram {
    unsafe fn from_raw_fd(fd: RawFd) -> UnixDatagram {
        UnixDatagram::from_std(FromRawFd::from_raw_fd(fd))
    }
}

#[cfg(test)]
mod test {
    use std::net::Shutdown;
    use std::os::fd::{FromRawFd, IntoRawFd};

    use crate::UnixDatagram;

    /// UT for `UnixDatagram::pair`
    ///
    /// # Brief
    /// 1. Create a pair of UnixDatagram
    /// 2. Check if the peer address is correct
    /// 3. Check if the local address is correct
    /// 4. Shutdown both UnixDatagram
    #[test]
    fn ut_uds_datagram_pair() {
        let (sender, receiver) = UnixDatagram::pair().unwrap();
        let sender2 = sender.try_clone().unwrap();

        let addr = sender2.local_addr().unwrap();
        let fmt = format!("{addr:?}");
        assert_eq!(&fmt, "(unnamed)");

        let addr = receiver.peer_addr().unwrap();
        let fmt = format!("{addr:?}");
        assert_eq!(&fmt, "(unnamed)");

        let fd = receiver.into_raw_fd();
        let receiver2 = unsafe { UnixDatagram::from_raw_fd(fd) };
        let addr = receiver2.local_addr().unwrap();
        let fmt = format!("{addr:?}");
        assert_eq!(&fmt, "(unnamed)");

        receiver2.shutdown(Shutdown::Both).unwrap();
        sender.shutdown(Shutdown::Both).unwrap()
    }
}

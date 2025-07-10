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
use std::net::SocketAddr;

use ylong_io::Interest;

use crate::net::sys::addr::ToSocketAddrs;
use crate::net::{AsyncSource, TcpStream};

/// An asynchronous version of [`std::net::TcpListener`]. Provides async
/// bind/accept methods.
///
/// # Example
/// ```rust
/// use std::io;
///
/// use ylong_runtime::net::TcpListener;
///
/// async fn io_func() -> io::Result<()> {
///     let addr = "127.0.0.1:8080";
///     let server = TcpListener::bind(addr).await?;
///     let (stream, address) = server.accept().await?;
///     Ok(())
/// }
/// ```
pub struct TcpListener {
    source: AsyncSource<ylong_io::TcpListener>,
}

impl TcpListener {
    /// A TCP socket server, asynchronously listening for connections.
    ///
    /// After creating a `TcpListener` by binding it to a socket address, it
    /// listens for incoming TCP connections asynchronously. These
    /// connections can be accepted by calling [`TcpListener::accept`]
    ///
    /// # Note
    ///
    /// If there are multiple addresses in SocketAddr, it will attempt to
    /// connect them in sequence until one of the addrs returns success. If
    /// all connections fail, it returns the error of the last connection.
    /// This behavior is consistent with std.
    ///
    /// # Panic
    /// Calling this method outside of a Ylong Runtime could cause panic.
    ///
    /// # Example
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080";
    ///     let server = TcpListener::bind(addr).await?;
    ///     Ok(())
    /// }
    /// ```
    pub async fn bind<A: ToSocketAddrs>(addr: A) -> io::Result<TcpListener> {
        super::super::addr::each_addr(addr, ylong_io::TcpListener::bind)
            .await
            .map(TcpListener::new)
            .and_then(|op| op)
    }

    /// Asynchronously accepts a new incoming connection from this listener.
    ///
    /// When connection gets established, the corresponding [`TcpStream`] and
    /// the remote peer's address will be returned.
    ///
    /// # Panic
    /// Calling this method outside of a Ylong Runtime could cause panic.
    ///
    /// # Example
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080";
    ///     let server = TcpListener::bind(addr).await?;
    ///     let (stream, address) = server.accept().await?;
    ///     Ok(())
    /// }
    /// ```
    pub async fn accept(&self) -> io::Result<(TcpStream, SocketAddr)> {
        let (stream, addr) = self
            .source
            .async_process(Interest::READABLE, || self.source.accept())
            .await?;
        let stream = TcpStream::new(stream)?;
        Ok((stream, addr))
    }

    /// Returns the local socket address of this listener.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut server = TcpListener::bind(addr).await.unwrap();
    ///     let ret = server.local_addr().unwrap();
    ///     Ok(())
    /// }
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.source.local_addr()
    }

    /// Gets the value of the IP_TTL option for this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut server = TcpListener::bind(addr).await.unwrap();
    ///     let ret = server.ttl().unwrap();
    ///     Ok(())
    /// }
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.source.ttl()
    }

    /// Sets the value for the IP_TTL option on this socket.
    /// This value sets the time-to-live field that is used in every packet sent
    /// from this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut server = TcpListener::bind(addr).await.unwrap();
    ///     let ret = server.set_ttl(100).unwrap();
    ///     Ok(())
    /// }
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.source.set_ttl(ttl)
    }

    /// Sets the value for the IP_TTL option on this socket.
    /// This value sets the time-to-live field that is used in every packet sent
    /// from this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut server = TcpListener::bind(addr).await.unwrap();
    ///     let ret = server.take_error().unwrap();
    ///     Ok(())
    /// }
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.source.take_error()
    }

    // Registers the ylong_io::TcpListener's fd to the reactor, and returns the
    // async TcpListener
    pub(crate) fn new(listener: ylong_io::TcpListener) -> io::Result<Self> {
        let source = AsyncSource::new(listener, None)?;
        Ok(TcpListener { source })
    }

    /// Sets the owner for this source's fd
    ///
    /// # Error
    /// This method calls libc::fchown, libc::fchown returns the following
    /// errors [`libc::EBADF`]: The fd argument is not an open file descriptor.
    /// [`libc::EPERM`]: The effective user ID does not match the owner of the file or the process does not have appropriate privilege and _POSIX_CHOWN_RESTRICTED indicates that such privilege is required.
    /// [`libc::EROFS`]:The file referred to by fildes resides on a read-only file system.
    /// [`libc::EINVAL`]: The owner or group ID is not a value supported by the implementation.
    /// [`libc::EIO`]: A physical I/O error has occurred.
    /// [`libc::EINTR`]: The fchown() function was interrupted by a signal which was caught.
    ///
    /// # Example
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpListener;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut server = TcpListener::bind(addr).await?;
    ///     server.fchown(0, 0)?;
    ///     Ok(())
    /// }
    /// ```
    #[cfg(target_os = "linux")]
    pub fn fchown(&self, uid: uid_t, gid: gid_t) -> io::Result<()> {
        self.source.fchown(uid, gid)
    }
}

#[cfg(windows)]
use std::os::windows::io::{AsRawSocket, RawSocket};

#[cfg(windows)]
impl AsRawSocket for TcpListener {
    fn as_raw_socket(&self) -> RawSocket {
        self.source.as_raw_socket()
    }
}

#[cfg(unix)]
use std::os::fd::{AsRawFd, RawFd};

#[cfg(target_os = "linux")]
use libc::{gid_t, uid_t};
#[cfg(unix)]
use ylong_io::Source;

#[cfg(unix)]
impl AsRawFd for TcpListener {
    fn as_raw_fd(&self) -> RawFd {
        self.source.get_fd()
    }
}

#[cfg(test)]
mod test {
    use crate::net::TcpListener;

    const ADDR: &str = "127.0.0.1:0";

    /// UT test cases for `TcpListener`.
    ///
    /// # Brief
    /// 1. Bind `TcpListener`.
    /// 2. Call set_ttl(), ttl(), take_error().
    /// 3. Check result is correct.
    #[test]
    fn ut_tcp_listener_basic() {
        crate::block_on(async {
            let server = TcpListener::bind(ADDR).await.unwrap();

            server.set_ttl(101).unwrap();
            assert_eq!(server.ttl().unwrap(), 101);

            assert!(server.take_error().unwrap().is_none());
        })
    }
}

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

use std::fmt::{Debug, Formatter};
use std::io;
use std::io::{IoSlice, Read, Write};
use std::net::{Shutdown, SocketAddr};
use std::pin::Pin;
use std::sync::Arc;
use std::task::{Context, Poll};
use std::time::Duration;

use ylong_io::Interest;

use super::split::{SplitReadHalf, SplitWriteHalf};
use crate::io::{AsyncRead, AsyncWrite, ReadBuf};
use crate::net::sys::tcp::split::{BorrowReadHalf, BorrowWriteHalf};
use crate::net::sys::ToSocketAddrs;
use crate::net::AsyncSource;

/// An asynchronous version of [`std::net::TcpStream`]
///
/// After creating a `TcpStream` by either connecting to a remote host or
/// accepting a connection on a `TcpListener`, data can be transmitted
/// asynchronously by reading and writing to it.
///
///
/// # Example
/// ```rust
/// use std::io;
/// use std::io::{IoSlice, IoSliceMut};
///
/// use ylong_runtime::io::{AsyncReadExt, AsyncWriteExt};
/// use ylong_runtime::net::TcpStream;
///
/// async fn io_func() -> io::Result<()> {
///     let addr = "127.0.0.1:8080";
///     let mut stream = TcpStream::connect(addr).await?;
///
///     let _ = stream.write(b"hello client").await?;
///     let _ = stream
///         .write_vectored(&[IoSlice::new(b"hello client")])
///         .await?;
///
///     let mut read_buf = [0 as u8; 1024];
///     let _ = stream.read(&mut read_buf).await?;
///     let _ = stream.read(&mut read_buf).await?;
///     Ok(())
/// }
/// ```
pub struct TcpStream {
    pub(crate) source: AsyncSource<ylong_io::TcpStream>,
}

impl Debug for TcpStream {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        self.source.fmt(f)
    }
}

impl TcpStream {
    /// Opens a TCP connection to a remote host asynchronously.
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
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080";
    ///     let mut stream = TcpStream::connect(addr).await?;
    ///     Ok(())
    /// }
    /// ```
    pub async fn connect<A: ToSocketAddrs>(addr: A) -> io::Result<Self> {
        let stream = super::super::addr::each_addr(addr, ylong_io::TcpStream::connect).await?;
        Self::connect_inner(stream).await
    }

    async fn connect_inner(stream: ylong_io::TcpStream) -> io::Result<Self> {
        let stream = TcpStream::new(stream)?;
        stream
            .source
            .async_process(
                // Wait until the stream is writable
                Interest::WRITABLE,
                || Ok(()),
            )
            .await?;

        if let Some(e) = stream.source.take_error()? {
            return Err(e);
        }
        Ok(stream)
    }

    // Registers the ylong_io::TcpStream's fd to the reactor, and returns async
    // TcpStream.
    pub(crate) fn new(stream: ylong_io::TcpStream) -> io::Result<Self> {
        let source = AsyncSource::new(stream, None)?;
        Ok(TcpStream { source })
    }

    /// Waits for the socket to become readable.
    ///
    /// This function is usually paired up with [`TcpStream::try_read`]
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080";
    ///     let stream = TcpStream::connect(local_addr).await?;
    ///
    ///     let mut buf = vec![0; 12];
    ///
    ///     loop {
    ///         stream.readable().await?;
    ///         match stream.try_read(&mut buf) {
    ///             Ok(_) => break,
    ///             Err(e) if e.kind() == io::ErrorKind::WouldBlock => {}
    ///             Err(e) => panic!("{:?}", e),
    ///         }
    ///     }
    ///
    ///     Ok(())
    /// }
    /// ```
    pub async fn readable(&self) -> io::Result<()> {
        self.source.entry.readiness(Interest::READABLE).await?;
        Ok(())
    }

    /// Attempts to read data from the stream to the provided buffer, returning
    /// the number of bytes read.
    ///
    /// This function is usually paired up with [`TcpStream::readable`]
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080";
    ///     let stream = TcpStream::connect(local_addr).await?;
    ///
    ///     let mut buf = vec![0; 12];
    ///
    ///     loop {
    ///         stream.readable().await?;
    ///         match stream.try_read(&mut buf) {
    ///             Ok(_) => break,
    ///             Err(e) if e.kind() == io::ErrorKind::WouldBlock => {}
    ///             Err(e) => panic!("{:?}", e),
    ///         }
    ///     }
    ///
    ///     Ok(())
    /// }
    /// ```
    pub fn try_read(&self, buf: &mut [u8]) -> io::Result<usize> {
        self.source
            .try_io(Interest::READABLE, || (&*self.source).read(buf))
    }

    /// Waits for the socket to become writable.
    ///
    /// This function is usually paired up with [`TcpStream::try_write`]
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080";
    ///     let stream = TcpStream::connect(local_addr).await?;
    ///
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_write(b"write message") {
    ///             Ok(_) => break,
    ///             Err(e) if e.kind() == io::ErrorKind::WouldBlock => {}
    ///             Err(e) => panic!("{:?}", e),
    ///         }
    ///     }
    ///
    ///     Ok(())
    /// }
    /// ```
    pub async fn writable(&self) -> io::Result<()> {
        self.source.entry.readiness(Interest::WRITABLE).await?;
        Ok(())
    }

    /// Attempts to write data to the stream, returning the number of bytes
    /// written.
    ///
    /// This function is usually paired up with [`TcpStream::writable`]
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080";
    ///     let stream = TcpStream::connect(local_addr).await?;
    ///
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_write(b"write message") {
    ///             Ok(_) => break,
    ///             Err(e) if e.kind() == io::ErrorKind::WouldBlock => {}
    ///             Err(e) => panic!("{:?}", e),
    ///         }
    ///     }
    ///
    ///     Ok(())
    /// }
    /// ```
    pub fn try_write(&self, buf: &[u8]) -> io::Result<usize> {
        self.source
            .try_io(Interest::WRITABLE, || (&*self.source).write(buf))
    }

    /// Returns the socket address of the remote half of this TCP connection.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    /// use std::net::{IpAddr, Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     assert_eq!(
    ///         stream.local_addr().unwrap().ip(),
    ///         IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1))
    ///     );
    ///     Ok(())
    /// }
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.source.local_addr()
    }

    /// Returns the socket address of the remote half of this TCP connection.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     assert_eq!(
    ///         stream.peer_addr().unwrap(),
    ///         SocketAddr::V4(SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 1234))
    ///     );
    ///     Ok(())
    /// }
    /// ```
    pub fn peer_addr(&self) -> io::Result<SocketAddr> {
        self.source.peer_addr()
    }

    /// Sets the value of the `TCP_NODELAY`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     stream.set_nodelay(true).expect("set_nodelay call failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn set_nodelay(&self, nodelay: bool) -> io::Result<()> {
        self.source.set_nodelay(nodelay)
    }

    /// Gets the value of the `TCP_NODELAY`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     stream.set_nodelay(true).expect("set_nodelay call failed");
    ///     assert_eq!(stream.nodelay().unwrap_or(false), true);
    ///     Ok(())
    /// }
    /// ```
    pub fn nodelay(&self) -> io::Result<bool> {
        self.source.nodelay()
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
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///
    ///     stream.set_linger(None).expect("Sets linger fail.");
    ///     Ok(())
    /// }
    /// ```
    pub fn set_linger(&self, linger: Option<Duration>) -> io::Result<()> {
        self.source.set_linger(linger)
    }

    /// Gets the value of the linger on this socket by getting `SO_LINGER`
    /// option.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///
    ///     println!("{:?}", stream.linger());
    ///     Ok(())
    /// }
    /// ```
    pub fn linger(&self) -> io::Result<Option<Duration>> {
        self.source.linger()
    }

    /// Sets the value for the `IP_TTL`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     stream.set_ttl(100).expect("set_ttl call failed");
    ///     Ok(())
    /// }
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.source.set_ttl(ttl)
    }

    /// Gets the value of the `IP_TTL`.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     stream.set_ttl(100).expect("set_ttl call failed");
    ///     assert_eq!(stream.ttl().unwrap_or(0), 100);
    ///     Ok(())
    /// }
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.source.ttl()
    }

    /// Splits a TcpStream into a read half and a write half with reference,
    /// which can be used to read and write the stream concurrently.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let mut stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     let (read, write) = stream.split();
    ///     Ok(())
    /// }
    /// ```
    pub fn split(&mut self) -> (BorrowReadHalf, BorrowWriteHalf) {
        let read = BorrowReadHalf(self);
        let write = BorrowWriteHalf(self);
        (read, write)
    }

    /// Splits a TcpStream into a read half and a write half,
    /// which can be used to read and write the stream concurrently.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     let (read, write) = stream.into_split();
    ///     Ok(())
    /// }
    /// ```
    pub fn into_split(self) -> (SplitReadHalf, SplitWriteHalf) {
        let arc = Arc::new(self);
        let read = SplitReadHalf(Arc::clone(&arc));
        let write = SplitWriteHalf(Arc::clone(&arc));
        (read, write)
    }

    /// Receives data on the socket from the remote address to which it is
    /// connected, without removing that data from the queue.
    /// On success, returns the number of bytes peeked.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     let mut buf = [0; 10];
    ///     let len = stream.peek(&mut buf).await.expect("peek failed");
    ///     Ok(())
    /// }
    /// ```
    pub async fn peek(&self, buf: &mut [u8]) -> io::Result<usize> {
        self.source
            .async_process(Interest::READABLE, || self.source.peek(buf))
            .await
    }

    /// Gets the value of the SO_ERROR.
    ///
    /// # Example
    ///
    /// ```no_run
    /// use std::io;
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     match stream.take_error() {
    ///         Ok(Some(error)) => println!("TcpStream error: {error:?}"),
    ///         Ok(None) => println!("No error"),
    ///         Err(error) => println!("TcpStream.take_error failed: {error:?}"),
    ///     }
    ///     Ok(())
    /// }
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.source.take_error()
    }

    // todo: make this async
    /// Shutdown TcpStream
    pub fn shutdown(&self, how: Shutdown) -> io::Result<()> {
        self.source.shutdown(how)
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
    /// use ylong_runtime::net::TcpStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:1234";
    ///     let stream = TcpStream::connect(addr)
    ///         .await
    ///         .expect("Couldn't connect to the server...");
    ///     stream.fchown(0, 0)?;
    ///     Ok(())
    /// }
    /// ```
    #[cfg(target_os = "linux")]
    pub fn fchown(&self, uid: uid_t, gid: gid_t) -> io::Result<()> {
        self.source.fchown(uid, gid)
    }
}

impl AsyncRead for TcpStream {
    fn poll_read(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &mut ReadBuf<'_>,
    ) -> Poll<io::Result<()>> {
        self.source.poll_read(cx, buf)
    }
}

impl AsyncWrite for TcpStream {
    fn poll_write(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &[u8],
    ) -> Poll<io::Result<usize>> {
        self.source.poll_write(cx, buf)
    }

    fn poll_write_vectored(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        bufs: &[IoSlice<'_>],
    ) -> Poll<io::Result<usize>> {
        self.source.poll_write_vectored(cx, bufs)
    }

    fn is_write_vectored(&self) -> bool {
        true
    }

    fn poll_flush(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<io::Result<()>> {
        Poll::Ready(Ok(()))
    }

    fn poll_shutdown(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<io::Result<()>> {
        self.source.shutdown(std::net::Shutdown::Write)?;
        Poll::Ready(Ok(()))
    }
}

#[cfg(windows)]
use std::os::windows::io::{AsRawSocket, RawSocket};

#[cfg(windows)]
impl AsRawSocket for TcpStream {
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
impl AsRawFd for TcpStream {
    fn as_raw_fd(&self) -> RawFd {
        self.source.get_fd()
    }
}

#[cfg(test)]
mod test {
    use std::net::Ipv4Addr;
    use std::time::Duration;

    use crate::io::AsyncWriteExt;
    use crate::net::{TcpListener, TcpStream};

    const ADDR: &str = "127.0.0.1:0";

    /// UT test cases for `TcpStream`.
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Call peer_addr(), local_addr(), set_ttl(), ttl(), set_nodelay(),
    ///    nodelay(), take_error().
    /// 4. Check result is correct.
    #[test]
    fn ut_tcp_stream_basic() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let stream = stream.unwrap();
                assert_eq!(stream.peer_addr().unwrap(), addr);
                assert_eq!(
                    stream.local_addr().unwrap().ip(),
                    Ipv4Addr::new(127, 0, 0, 1)
                );
                stream.set_ttl(101).unwrap();
                assert_eq!(stream.ttl().unwrap(), 101);
                stream.set_nodelay(true).unwrap();
                assert!(stream.nodelay().unwrap());
                assert!(stream.linger().unwrap().is_none());
                stream.set_linger(Some(Duration::from_secs(1))).unwrap();
                assert_eq!(stream.linger().unwrap(), Some(Duration::from_secs(1)));
                assert!(stream.take_error().unwrap().is_none());
            });

            listener.accept().await.unwrap();

            handle.await.unwrap();
        });
    }

    /// UT test cases for `TcpStream`.
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Call peek() to get.
    /// 4. Check result is correct.
    #[test]
    fn ut_tcp_stream_peek() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let stream = stream.unwrap();
                let mut buf = [0; 100];
                let len = stream.peek(&mut buf).await.expect("peek failed");
                let buf = &buf[0..len];
                assert_eq!(len, 5);
                assert_eq!(String::from_utf8_lossy(buf), "hello");
            });

            let (mut stream, _) = listener.accept().await.unwrap();
            stream.write(b"hello").await.unwrap();

            handle.await.unwrap();
        });
    }

    /// UT test cases for `TcpStream`.
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. After accept, try to write buf.
    /// 2. `TcpStream` connect to listener and try to read buf.
    /// 4. Check result is correct.
    #[test]
    fn ut_tcp_stream_try() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let stream = stream.unwrap();
                let mut buf = vec![0; 5];
                stream.readable().await.unwrap();
                stream.try_read(&mut buf).unwrap();
                assert_eq!(buf, b"hello");
            });

            let (stream, _) = listener.accept().await.unwrap();
            stream.writable().await.unwrap();
            stream.try_write(b"hello").unwrap();

            handle.await.unwrap();
        });
    }
}

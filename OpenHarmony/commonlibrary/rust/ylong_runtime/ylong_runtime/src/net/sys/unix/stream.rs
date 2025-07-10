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

use std::fmt;
use std::io::{Error, IoSlice, Read, Result, Write};
use std::net::Shutdown;
use std::os::unix::io::{AsFd, AsRawFd, BorrowedFd, RawFd};
use std::os::unix::net;
use std::path::Path;
use std::pin::Pin;
use std::task::{Context, Poll};

use ylong_io::{Interest, Source};

use crate::io::{AsyncRead, AsyncWrite, ReadBuf};
use crate::net::{AsyncSource, Ready};

/// A non-blocking UDS Stream between two local sockets.
pub struct UnixStream {
    source: AsyncSource<ylong_io::UnixStream>,
}

impl UnixStream {
    /// Creates a new `UnixStream` from `ylong_io::UnixStream`
    pub(crate) fn new(stream: ylong_io::UnixStream) -> Result<Self> {
        Ok(UnixStream {
            source: AsyncSource::new(stream, None)?,
        })
    }

    /// Opens a UDS connection to a remote host asynchronously.
    ///
    /// # Example
    /// ```no_run
    /// use std::io;
    ///
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let mut stream = UnixStream::connect("/tmp/sock").await?;
    ///     Ok(())
    /// }
    /// ```
    pub async fn connect<P: AsRef<Path>>(path: P) -> Result<UnixStream> {
        let stream = UnixStream::new(ylong_io::UnixStream::connect(path)?)?;

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

    /// Creates new `UnixStream` from a `std::os::unix::net::UnixStream`.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::os::unix::net::UnixStream as StdUnixStream;
    ///
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn dox() -> Result<(), Box<dyn Error>> {
    ///     let std_stream = StdUnixStream::connect("/socket/path")?;
    ///     std_stream.set_nonblocking(true)?;
    ///     let stream = UnixStream::from_std(std_stream)?;
    ///     Ok(())
    /// }
    /// ```
    pub fn from_std(listener: net::UnixStream) -> Result<UnixStream> {
        let stream = ylong_io::UnixStream::from_std(listener);
        Ok(UnixStream {
            source: AsyncSource::new(stream, None)?,
        })
    }

    /// Creates an unnamed pair of connected sockets.
    /// Returns two `UnixStream`s which are connected to each other.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixStream;
    ///
    /// let (stream1, stream2) = match UnixStream::pair() {
    ///     Ok((stream1, stream2)) => (stream1, stream2),
    ///     Err(err) => {
    ///         println!("Couldn't create a pair of sockets: {err:?}");
    ///         return;
    ///     }
    /// };
    /// ```
    pub fn pair() -> Result<(UnixStream, UnixStream)> {
        let (stream1, stream2) = ylong_io::UnixStream::pair()?;
        let stream1 = UnixStream::new(stream1)?;
        let stream2 = UnixStream::new(stream2)?;

        Ok((stream1, stream2))
    }

    /// Waits for any of the requested ready states.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixStream::connect("/socket/path").await?;
    ///
    ///     loop {
    ///         let ready = stream
    ///             .ready(Interest::READABLE | Interest::WRITABLE)
    ///             .await?;
    ///
    ///         if ready.is_readable() {
    ///             let mut data = vec![0; 128];
    ///             match stream.try_read(&mut data) {
    ///                 Ok(n) => {
    ///                     println!("read {} bytes", n);
    ///                 }
    ///                 Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                     continue;
    ///                 }
    ///                 Err(e) => {
    ///                     return Err(e.into());
    ///                 }
    ///             }
    ///         }
    ///
    ///         if ready.is_writable() {
    ///             match stream.try_write(b"hello world") {
    ///                 Ok(n) => {
    ///                     println!("write {} bytes", n);
    ///                 }
    ///                 Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                     continue;
    ///                 }
    ///                 Err(e) => {
    ///                     return Err(e.into());
    ///                 }
    ///             }
    ///         }
    ///     }
    /// }
    /// ```
    pub async fn ready(&self, interest: Interest) -> Result<Ready> {
        let event = self.source.entry.readiness(interest).await?;
        Ok(event.ready)
    }

    /// Waits for `Interest::READABLE` requested ready states.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixStream::connect("/socket/path").await?;
    ///     loop {
    ///         stream.readable().await?;
    ///         let mut data = vec![0; 128];
    ///         match stream.try_read(&mut data) {
    ///             Ok(n) => {
    ///                 println!("read {} bytes", n);
    ///             }
    ///             Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                 continue;
    ///             }
    ///             Err(e) => {
    ///                 return Err(e.into());
    ///             }
    ///         }
    ///     }
    /// }
    /// ```
    pub async fn readable(&self) -> Result<()> {
        self.ready(Interest::READABLE).await?;
        Ok(())
    }

    /// Trys to read stream.
    /// This method will immediately return the result.
    /// If it is currently unavailable, it will return `WouldBlock`
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixStream::connect("/socket/path").await?;
    ///     loop {
    ///         stream.readable().await?;
    ///         let mut data = vec![0; 128];
    ///         match stream.try_read(&mut data) {
    ///             Ok(n) => {
    ///                 println!("read {} bytes", n);
    ///             }
    ///             Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                 continue;
    ///             }
    ///             Err(e) => {
    ///                 return Err(e.into());
    ///             }
    ///         }
    ///     }
    /// }
    /// ```
    pub fn try_read(&self, buf: &mut [u8]) -> Result<usize> {
        self.source
            .try_io(Interest::READABLE, || (&*self.source).read(buf))
    }

    /// Waits for `Interest::WRITABLE` requested ready states.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixStream::connect("/socket/path").await?;
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_write(b"hello world") {
    ///             Ok(n) => {
    ///                 println!("write {} bytes", n);
    ///             }
    ///             Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                 continue;
    ///             }
    ///             Err(e) => {
    ///                 return Err(e.into());
    ///             }
    ///         }
    ///     }
    /// }
    /// ```
    pub async fn writable(&self) -> Result<()> {
        self.ready(Interest::WRITABLE).await?;
        Ok(())
    }

    /// Trys to write stream.
    /// This method will immediately return the result.
    /// If it is currently unavailable, it will return `WouldBlock`
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixStream::connect("/socket/path").await?;
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_write(b"hello world") {
    ///             Ok(n) => {
    ///                 println!("write {} bytes", n);
    ///             }
    ///             Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
    ///                 continue;
    ///             }
    ///             Err(e) => {
    ///                 return Err(e.into());
    ///             }
    ///         }
    ///     }
    /// }
    /// ```
    pub fn try_write(&self, buf: &[u8]) -> Result<usize> {
        self.source
            .try_io(Interest::WRITABLE, || (&*self.source).write(buf))
    }

    /// Returns the error of the `SO_ERROR` option.
    ///
    /// # Examples
    /// ```no_run
    /// use std::io::Result;
    ///
    /// use ylong_runtime::net::UnixStream;
    ///
    /// async fn test() -> Result<()> {
    ///     let socket = UnixStream::connect("/tmp/sock").await?;
    ///     if let Ok(Some(err)) = socket.take_error() {
    ///         println!("get error: {err:?}");
    ///     }
    ///     Ok(())
    /// }
    /// ```
    pub fn take_error(&self) -> Result<Option<Error>> {
        self.source.take_error()
    }

    /// Shutdown UnixStream
    pub fn shutdown(&self, how: Shutdown) -> Result<()> {
        self.source.shutdown(how)
    }
}

impl AsyncRead for UnixStream {
    fn poll_read(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &mut ReadBuf<'_>,
    ) -> Poll<Result<()>> {
        self.source.poll_read(cx, buf)
    }
}

impl AsyncWrite for UnixStream {
    fn poll_write(self: Pin<&mut Self>, cx: &mut Context<'_>, buf: &[u8]) -> Poll<Result<usize>> {
        self.source.poll_write(cx, buf)
    }

    fn poll_write_vectored(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        bufs: &[IoSlice<'_>],
    ) -> Poll<Result<usize>> {
        self.source.poll_write_vectored(cx, bufs)
    }

    fn is_write_vectored(&self) -> bool {
        true
    }

    fn poll_flush(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<Result<()>> {
        Poll::Ready(Ok(()))
    }

    fn poll_shutdown(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<Result<()>> {
        self.source.shutdown(std::net::Shutdown::Write)?;
        Poll::Ready(Ok(()))
    }
}

impl fmt::Debug for UnixStream {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.source.fmt(f)
    }
}

impl AsRawFd for UnixStream {
    fn as_raw_fd(&self) -> RawFd {
        self.source.get_fd()
    }
}

impl AsFd for UnixStream {
    fn as_fd(&self) -> BorrowedFd<'_> {
        unsafe { BorrowedFd::borrow_raw(self.as_raw_fd()) }
    }
}

#[cfg(test)]
mod test {
    use std::io;
    use std::os::fd::{AsFd, AsRawFd};

    use crate::io::{AsyncReadExt, AsyncWriteExt};
    use crate::net::UnixStream;

    /// Uds UnixStream test case.
    ///
    /// # Title
    /// ut_uds_stream_baisc_test
    ///
    /// # Brief
    /// 1. Create a std UnixStream with `pair()`.
    /// 2. Convert std UnixStream to Ylong_runtime UnixStream.
    /// 3. Check result is correct.
    #[test]
    fn ut_uds_stream_baisc_test() {
        let (stream, _) = std::os::unix::net::UnixStream::pair().unwrap();
        let handle = crate::spawn(async {
            let res = UnixStream::from_std(stream);
            assert!(res.is_ok());
            let stream = res.unwrap();
            assert!(stream.as_fd().as_raw_fd() >= 0);
            assert!(stream.as_raw_fd() >= 0);
            assert!(stream.take_error().is_ok());
        });
        crate::block_on(handle).unwrap();
    }

    /// uds UnixStream test case.
    ///
    /// # Title
    /// ut_uds_stream_pair_test
    ///
    /// # Brief
    /// 1. Creates a server and a client with `pair()`.
    /// 2. Server Sends message and client recv it.
    #[test]
    fn ut_uds_stream_pair_test() {
        let handle = crate::spawn(async {
            let (mut server, mut client) = UnixStream::pair().unwrap();

            server.write_all(b"hello").await.unwrap();
            server.flush().await.unwrap();

            let mut read_buf = [0_u8; 5];
            let len = client.read(&mut read_buf).await.unwrap();
            assert_eq!(std::str::from_utf8(&read_buf).unwrap(), "hello".to_string());
            assert_eq!(len, "hello".len());
        });
        crate::block_on(handle).unwrap();
    }

    /// Uds UnixStream try_xxx() test case.
    ///
    /// # Title
    /// ut_uds_stream_try_test
    ///
    /// # Brief
    /// 1. Creates a server and a client with `pair()`.
    /// 2. Server send message with `writable()` and `try_write()`.
    /// 3. Client receive message with `readable()` and `try_read()`.
    /// 4. Check result is correct.
    #[test]
    fn ut_uds_stream_try_test() {
        let handle = crate::spawn(async {
            let (server, client) = UnixStream::pair().unwrap();
            loop {
                server.writable().await.unwrap();
                match server.try_write(b"hello") {
                    Ok(n) => {
                        assert_eq!(n, "hello".len());
                        break;
                    }
                    Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => continue,
                    Err(e) => panic!("{e:?}"),
                }
            }
            loop {
                client.readable().await.unwrap();
                let mut data = vec![0; 5];
                match client.try_read(&mut data) {
                    Ok(n) => {
                        assert_eq!(n, "hello".len());
                        assert_eq!(std::str::from_utf8(&data).unwrap(), "hello".to_string());
                        break;
                    }
                    Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => continue,
                    Err(e) => panic!("{e:?}"),
                }
            }
        });
        crate::block_on(handle).unwrap();
    }
}

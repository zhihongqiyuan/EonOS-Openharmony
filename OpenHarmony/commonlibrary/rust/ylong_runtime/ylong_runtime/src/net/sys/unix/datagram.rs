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
use std::io::{Error, Result};
use std::os::unix::io::{AsFd, AsRawFd, BorrowedFd, RawFd};
use std::os::unix::net;
use std::path::Path;

use ylong_io::{Interest, Source};

use crate::net::{AsyncSource, Ready};

/// A Unix datagram socket.
pub struct UnixDatagram {
    source: AsyncSource<ylong_io::UnixDatagram>,
}

impl UnixDatagram {
    /// Creates a new `UnixDatagram` from `ylong_io::UnixDatagram`
    fn new(datagram: ylong_io::UnixDatagram) -> Result<Self> {
        Ok(UnixDatagram {
            source: AsyncSource::new(datagram, None)?,
        })
    }

    /// Creates a Unix Datagram Socket bound to the given path.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// let _ = UnixDatagram::bind("/socket/path").unwrap();
    /// ```
    pub fn bind<P: AsRef<Path>>(path: P) -> Result<UnixDatagram> {
        UnixDatagram::new(ylong_io::UnixDatagram::bind(path)?)
    }

    /// Waits for any of the requested ready states.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixDatagram::bind("/socket/path")?;
    ///
    ///     loop {
    ///         let ready = stream
    ///             .ready(Interest::READABLE | Interest::WRITABLE)
    ///             .await?;
    ///
    ///         if ready.is_readable() {
    ///             let mut data = vec![0; 128];
    ///             match stream.try_recv(&mut data) {
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
    ///             match stream.try_send(b"hello world") {
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
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixDatagram::bind("/socket/path")?;
    ///     loop {
    ///         stream.readable().await?;
    ///         let mut data = vec![0; 128];
    ///         match stream.try_recv(&mut data) {
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

    /// Trys to recv message.
    /// This method will immediately return the result.
    /// If it is currently unavailable, it will return `WouldBlock`
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixDatagram::bind("/socket/path")?;
    ///     loop {
    ///         stream.readable().await?;
    ///         let mut data = vec![0; 128];
    ///         match stream.try_recv(&mut data) {
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
    pub fn try_recv(&self, buf: &mut [u8]) -> Result<usize> {
        self.source
            .try_io(Interest::READABLE, || (*self.source).recv(buf))
    }

    /// Waits for `Interest::WRITABLE` requested ready states.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixDatagram::bind("/socket/path")?;
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_send(b"hello world") {
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

    /// Trys to send message.
    /// This method will immediately return the result.
    /// If it is currently unavailable, it will return `WouldBlock`
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::io::ErrorKind;
    ///
    /// use ylong_io::Interest;
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> Result<(), Box<dyn Error>> {
    ///     let stream = UnixDatagram::bind("/socket/path")?;
    ///     loop {
    ///         stream.writable().await?;
    ///         match stream.try_send(b"hello world") {
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
    pub fn try_send(&self, buf: &[u8]) -> Result<usize> {
        self.source
            .try_io(Interest::WRITABLE, || (*self.source).send(buf))
    }

    /// Creates an unnamed pair of connected sockets.
    /// Returns two `UnixDatagram`s which are connected to each other.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// let (datagram1, datagram2) = UnixDatagram::pair().unwrap();
    /// ```
    pub fn pair() -> Result<(UnixDatagram, UnixDatagram)> {
        let (datagram1, datagram2) = ylong_io::UnixDatagram::pair()?;
        let datagram1 = UnixDatagram::new(datagram1)?;
        let datagram2 = UnixDatagram::new(datagram2)?;

        Ok((datagram1, datagram2))
    }

    /// Creates new `UnixDatagram` from a `std::os::unix::net::UnixDatagram`.
    ///
    /// # Examples
    /// ```no_run
    /// use std::error::Error;
    /// use std::os::unix::net::UnixDatagram as StdUnixDatagram;
    ///
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn dox() -> Result<(), Box<dyn Error>> {
    ///     let std_stream = StdUnixDatagram::bind("/socket/path")?;
    ///     std_stream.set_nonblocking(true)?;
    ///     let stream = UnixDatagram::from_std(std_stream)?;
    ///     Ok(())
    /// }
    /// ```
    pub fn from_std(listener: net::UnixDatagram) -> Result<UnixDatagram> {
        let stream = ylong_io::UnixDatagram::from_std(listener);
        Ok(UnixDatagram {
            source: AsyncSource::new(stream, None)?,
        })
    }

    /// Creates a Unix Datagram Socket with no bound.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// let socket = UnixDatagram::unbound();
    /// ```
    pub fn unbound() -> Result<UnixDatagram> {
        UnixDatagram::new(ylong_io::UnixDatagram::unbound()?)
    }

    /// Connects the socket to the specified path.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_io::UnixDatagram;
    ///
    /// let socket = UnixDatagram::unbound().unwrap();
    /// match socket.connect("/socket/path") {
    ///     Ok(socket) => socket,
    ///     Err(err) => {
    ///         println!("connect fail: {err:?}");
    ///     }
    /// };
    /// ```
    pub fn connect<P: AsRef<Path>>(&self, path: P) -> Result<()> {
        self.source.connect(path)
    }

    /// Sends data on the socket to the socket's peer.
    /// On success, returns the number of bytes written.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     socket.connect("/some/sock").expect("connect fail");
    ///     socket.send(b"hello world").await.expect("send_to failed");
    ///     Ok(())
    /// }
    /// ```
    pub async fn send(&self, buf: &[u8]) -> Result<usize> {
        self.source
            .async_process(Interest::WRITABLE, || self.source.send(buf))
            .await
    }

    /// Sends data on the socket to the specified address.
    /// On success, returns the number of bytes written.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     socket
    ///         .send_to(b"hello world", "/some/sock")
    ///         .await
    ///         .expect("send_to failed");
    ///     Ok(())
    /// }
    /// ```
    pub async fn send_to<P: AsRef<Path>>(&self, buf: &[u8], path: P) -> Result<usize> {
        self.source
            .async_process(Interest::WRITABLE, || {
                self.source.send_to(buf, path.as_ref())
            })
            .await
    }

    /// Receives data from the socket.
    /// On success, returns the number of bytes read.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::bind("/socket/path")?;
    ///     let mut buf = vec![0; 8];
    ///     socket.recv(buf.as_mut_slice()).await.expect("recv failed");
    ///     Ok(())
    /// }
    /// ```
    pub async fn recv(&self, buf: &mut [u8]) -> Result<usize> {
        self.source
            .async_process(Interest::READABLE, || self.source.recv(buf))
            .await
    }

    /// Receives data from the socket.
    /// On success, returns the number of bytes read and the address from
    /// whence the data came.
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// async fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     let mut buf = vec![0; 8];
    ///     let (size, sender) = socket.recv_from(buf.as_mut_slice()).await?;
    ///     println!("received {size} bytes from {sender:?}");
    ///     Ok(())
    /// }
    /// ```
    pub async fn recv_from(&self, buf: &mut [u8]) -> Result<(usize, net::SocketAddr)> {
        self.source
            .async_process(Interest::READABLE, || self.source.recv_from(buf))
            .await
    }

    /// Returns the value of the `SO_ERROR` option.
    ///
    /// # Examples
    /// ```no_run
    /// use ylong_runtime::net::UnixDatagram;
    ///
    /// fn test() -> std::io::Result<()> {
    ///     let socket = UnixDatagram::unbound()?;
    ///     if let Ok(Some(err)) = socket.take_error() {
    ///         println!("get error: {err:?}");
    ///     }
    ///     Ok(())
    /// }
    /// ```
    pub fn take_error(&self) -> Result<Option<Error>> {
        self.source.take_error()
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
    pub fn shutdown(&self, how: std::net::Shutdown) -> Result<()> {
        self.source.shutdown(how)
    }
}

impl fmt::Debug for UnixDatagram {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.source.fmt(f)
    }
}

impl AsRawFd for UnixDatagram {
    fn as_raw_fd(&self) -> RawFd {
        self.source.get_fd()
    }
}

impl AsFd for UnixDatagram {
    fn as_fd(&self) -> BorrowedFd<'_> {
        unsafe { BorrowedFd::borrow_raw(self.as_raw_fd()) }
    }
}

#[cfg(test)]
mod test {
    use std::io;
    use std::os::fd::{AsFd, AsRawFd};

    use crate::net::UnixDatagram;

    /// Uds UnixDatagram test case.
    ///
    /// # Title
    /// ut_uds_datagram_baisc_test
    ///
    /// # Brief
    /// 1. Create a std UnixDatagram with `pair()`.
    /// 2. Convert std UnixDatagram to Ylong_runtime UnixDatagram.
    /// 3. Check result is correct.
    #[test]
    fn ut_uds_datagram_baisc_test() {
        let (datagram, _) = std::os::unix::net::UnixDatagram::pair().unwrap();
        let handle = crate::spawn(async {
            let res = UnixDatagram::from_std(datagram);
            assert!(res.is_ok());
            let datagram = res.unwrap();
            assert!(datagram.as_fd().as_raw_fd() >= 0);
            assert!(datagram.as_raw_fd() >= 0);
            assert!(datagram.take_error().is_ok());
        });
        crate::block_on(handle).unwrap();
    }

    /// uds UnixDatagram test case.
    ///
    /// # Title
    /// ut_uds_datagram_read_write_test
    ///
    /// # Brief
    /// 1. Creates a server and a client.
    /// 2. Client Sends message and server recv it.
    ///
    /// # Note
    /// Each execution will leave a file under PATH which must be deleted,
    /// otherwise the next bind operation will fail.
    #[test]
    fn ut_uds_datagram_read_write_test() {
        let handle2 = crate::spawn(async {
            let (server, client) = UnixDatagram::pair().unwrap();

            let handle = crate::spawn(async move {
                client.send(b"hello world").await.expect("send failed");
            });

            let mut buf = vec![0; 11];
            server.recv(buf.as_mut_slice()).await.expect("recv failed");
            assert_eq!(
                std::str::from_utf8(&buf).unwrap(),
                "hello world".to_string()
            );

            handle.await.unwrap();
        });
        crate::block_on(handle2).unwrap();
    }

    /// Uds UnixDatagram try_xxx() test case.
    ///
    /// # Title
    /// ut_uds_datagram_try_test
    ///
    /// # Brief
    /// 1. Creates a server and a client with `pair()`.
    /// 2. Server send message with `writable()` and `try_send()`.
    /// 3. Client receive message with `readable()` and `try_recv()`.
    /// 4. Check result is correct.
    #[test]
    fn ut_uds_datagram_try_test() {
        let handle = crate::spawn(async {
            let (server, client) = UnixDatagram::pair().unwrap();
            loop {
                server.writable().await.unwrap();
                match server.try_send(b"hello") {
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
                match client.try_recv(&mut data) {
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

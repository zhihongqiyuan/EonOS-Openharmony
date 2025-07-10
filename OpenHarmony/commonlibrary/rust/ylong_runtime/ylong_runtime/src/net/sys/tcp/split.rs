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

use std::io::IoSlice;
use std::net::Shutdown;
use std::pin::Pin;
use std::sync::Arc;
use std::task::{Context, Poll};

use crate::io::{AsyncRead, AsyncWrite, ReadBuf};
use crate::net::TcpStream;

/// Borrowed read half of a TcpStream
pub struct BorrowReadHalf<'a>(pub(crate) &'a TcpStream);

/// Borrowed write half of a TcpStream
pub struct BorrowWriteHalf<'a>(pub(crate) &'a TcpStream);

impl AsyncRead for BorrowReadHalf<'_> {
    fn poll_read(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &mut ReadBuf<'_>,
    ) -> Poll<std::io::Result<()>> {
        self.0.source.poll_read(cx, buf)
    }
}

impl AsyncWrite for BorrowWriteHalf<'_> {
    fn poll_write(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &[u8],
    ) -> Poll<std::io::Result<usize>> {
        self.0.source.poll_write(cx, buf)
    }

    fn poll_write_vectored(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        bufs: &[IoSlice<'_>],
    ) -> Poll<std::io::Result<usize>> {
        self.0.source.poll_write_vectored(cx, bufs)
    }

    fn is_write_vectored(&self) -> bool {
        self.0.is_write_vectored()
    }

    fn poll_flush(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<std::io::Result<()>> {
        Poll::Ready(Ok(()))
    }

    fn poll_shutdown(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<std::io::Result<()>> {
        self.0.shutdown(Shutdown::Write).into()
    }
}

/// Read half of a TcpStream
pub struct SplitReadHalf(pub(crate) Arc<TcpStream>);

/// Write half of a TcpStream
pub struct SplitWriteHalf(pub(crate) Arc<TcpStream>);

impl AsyncRead for SplitReadHalf {
    fn poll_read(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &mut ReadBuf<'_>,
    ) -> Poll<std::io::Result<()>> {
        self.0.source.poll_read(cx, buf)
    }
}

impl AsyncWrite for SplitWriteHalf {
    fn poll_write(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        buf: &[u8],
    ) -> Poll<std::io::Result<usize>> {
        self.0.source.poll_write(cx, buf)
    }

    fn poll_write_vectored(
        self: Pin<&mut Self>,
        cx: &mut Context<'_>,
        bufs: &[IoSlice<'_>],
    ) -> Poll<std::io::Result<usize>> {
        self.0.source.poll_write_vectored(cx, bufs)
    }

    fn is_write_vectored(&self) -> bool {
        self.0.is_write_vectored()
    }

    fn poll_flush(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<std::io::Result<()>> {
        Poll::Ready(Ok(()))
    }

    fn poll_shutdown(self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<std::io::Result<()>> {
        self.0.shutdown(Shutdown::Write).into()
    }
}

#[cfg(test)]
mod test {
    use std::io::IoSlice;

    use crate::io::{AsyncReadExt, AsyncWriteExt};
    use crate::net::{TcpListener, TcpStream};

    const ADDR: &str = "127.0.0.1:0";

    /// UT test cases for `TcpStream` of split().
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Split TcpStream into read half and write half with borrowed.
    /// 4. Write with write half and read with read half.
    /// 5. Check result is correct.
    #[test]
    fn ut_test_borrow_half() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let mut stream = stream.unwrap();
                let (mut read_half, mut write_half) = stream.split();
                let mut buf = [0; 16];
                write_half.write(b"I am write half.").await.unwrap();

                let n = read_half.read(&mut buf).await.expect("server read err");
                assert_eq!(n, 16);
                assert_eq!(
                    String::from_utf8(Vec::from(buf)).unwrap().as_str(),
                    "hello read half."
                );
            });

            let (mut stream, _) = listener.accept().await.unwrap();
            let mut buf = [0; 16];
            let n = stream.read(&mut buf).await.expect("server read err");
            assert_eq!(n, 16);
            assert_eq!(
                String::from_utf8(Vec::from(buf)).unwrap().as_str(),
                "I am write half."
            );
            stream.write(b"hello read half.").await.unwrap();

            handle.await.unwrap();
        });
    }

    /// UT test cases for `TcpStream` of into_split().
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Split TcpStream into read half and write half with owned.
    /// 4. Write with write half and read with read half.
    /// 5. Check result is correct.
    #[test]
    fn ut_test_owned_half() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let stream = stream.unwrap();

                let (mut read_half, mut write_half) = stream.into_split();
                let mut buf = [0; 16];
                write_half.write(b"I am write half.").await.unwrap();

                let n = read_half.read(&mut buf).await.expect("server read err");
                assert_eq!(n, 16);
                assert_eq!(
                    String::from_utf8(Vec::from(buf)).unwrap().as_str(),
                    "hello read half."
                );
            });

            let (mut stream, _) = listener.accept().await.unwrap();
            let mut buf = [0; 16];
            let n = stream.read(&mut buf).await.expect("server read err");
            assert_eq!(n, 16);
            assert_eq!(
                String::from_utf8(Vec::from(buf)).unwrap().as_str(),
                "I am write half."
            );
            stream.write(b"hello read half.").await.unwrap();

            handle.await.unwrap();
        });
    }

    /// UT test cases for `TcpStream` of split() and write_vectored().
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Split TcpStream into read half and write half with borrowed.
    /// 4. Write with write half and read with read half.
    /// 5. Check result is correct.
    #[test]
    fn ut_test_borrow_half_vector() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let mut stream = stream.unwrap();
                let (mut read_half, mut write_half) = stream.split();
                let mut buf = [0; 6];
                let data1 = [1, 2, 3];
                let data2 = [4, 5, 6];
                let slice1 = IoSlice::new(&data1);
                let slice2 = IoSlice::new(&data2);
                write_half.write_vectored(&[slice1, slice2]).await.unwrap();
                write_half.flush().await.unwrap();
                write_half.shutdown().await.unwrap();

                let n = read_half.read(&mut buf).await.expect("server read err");
                assert_eq!(n, 6);
                assert_eq!(buf, [6, 5, 4, 3, 2, 1]);
            });

            let (mut stream, _) = listener.accept().await.unwrap();
            let mut buf = [0; 6];
            let n = stream.read(&mut buf).await.expect("server read err");
            assert_eq!(n, 6);
            assert_eq!(buf, [1, 2, 3, 4, 5, 6]);
            let data1 = [6, 5, 4];
            let data2 = [3, 2, 1];
            let slice1 = IoSlice::new(&data1);
            let slice2 = IoSlice::new(&data2);
            stream.write_vectored(&[slice1, slice2]).await.unwrap();

            handle.await.unwrap();
        });
    }

    /// UT test cases for `TcpStream` of into_split() and write_vectored().
    ///
    /// # Brief
    /// 1. Bind `TcpListener` and wait for `accept()`.
    /// 2. `TcpStream` connect to listener.
    /// 3. Split TcpStream into read half and write half with owned.
    /// 4. Write with write half and read with read half.
    /// 5. Check result is correct.
    #[test]
    fn ut_test_owned_half_vector() {
        crate::block_on(async {
            let listener = TcpListener::bind(ADDR).await.unwrap();
            let addr = listener.local_addr().unwrap();

            let handle = crate::spawn(async move {
                let mut stream = TcpStream::connect(addr).await;
                while stream.is_err() {
                    stream = TcpStream::connect(addr).await;
                }
                let stream = stream.unwrap();
                let (mut read_half, mut write_half) = stream.into_split();
                let mut buf = [0; 6];
                let data1 = [1, 2, 3];
                let data2 = [4, 5, 6];
                let slice1 = IoSlice::new(&data1);
                let slice2 = IoSlice::new(&data2);
                write_half.write_vectored(&[slice1, slice2]).await.unwrap();
                write_half.flush().await.unwrap();
                write_half.shutdown().await.unwrap();

                let n = read_half.read(&mut buf).await.expect("server read err");
                assert_eq!(n, 6);
                assert_eq!(buf, [6, 5, 4, 3, 2, 1]);
            });

            let (mut stream, _) = listener.accept().await.unwrap();
            let mut buf = [0; 6];
            let n = stream.read(&mut buf).await.expect("server read err");
            assert_eq!(n, 6);
            assert_eq!(buf, [1, 2, 3, 4, 5, 6]);
            let data1 = [6, 5, 4];
            let data2 = [3, 2, 1];
            let slice1 = IoSlice::new(&data1);
            let slice2 = IoSlice::new(&data2);
            stream.write_vectored(&[slice1, slice2]).await.unwrap();

            handle.await.unwrap();
        });
    }
}

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

use std::fs::File;
use std::io;
use std::io::{IoSlice, Write};
use std::os::unix::io::{AsFd, AsRawFd, BorrowedFd, FromRawFd, IntoRawFd, RawFd};

use ylong_io::sys::SourceFd;
use ylong_io::{Interest, Selector, Source, Token};

#[derive(Debug)]
pub(crate) struct Pipe {
    pub(crate) fd: File,
}

impl<T: IntoRawFd> From<T> for Pipe {
    fn from(value: T) -> Self {
        let fd = unsafe { File::from_raw_fd(value.into_raw_fd()) };
        Self { fd }
    }
}

impl<'a> io::Read for &'a Pipe {
    fn read(&mut self, buf: &mut [u8]) -> io::Result<usize> {
        (&self.fd).read(buf)
    }
}

impl<'a> Write for &'a Pipe {
    fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
        (&self.fd).write(buf)
    }

    fn write_vectored(&mut self, bufs: &[IoSlice<'_>]) -> io::Result<usize> {
        (&self.fd).write_vectored(bufs)
    }

    fn flush(&mut self) -> io::Result<()> {
        (&self.fd).flush()
    }
}

impl AsRawFd for Pipe {
    fn as_raw_fd(&self) -> RawFd {
        self.fd.as_raw_fd()
    }
}

impl AsFd for Pipe {
    fn as_fd(&self) -> BorrowedFd<'_> {
        unsafe { BorrowedFd::borrow_raw(self.fd.as_raw_fd()) }
    }
}

impl Source for Pipe {
    fn register(
        &mut self,
        selector: &Selector,
        token: Token,
        interests: Interest,
    ) -> io::Result<()> {
        SourceFd(&self.as_raw_fd()).register(selector, token, interests)
    }

    fn deregister(&mut self, selector: &Selector) -> io::Result<()> {
        SourceFd(&self.as_raw_fd()).deregister(selector)
    }

    fn get_fd(&self) -> ylong_io::Fd {
        self.fd.as_raw_fd()
    }
}

#[cfg(test)]
mod test {
    use std::io::{Read, Seek, Write};
    use std::os::fd::{AsFd, AsRawFd};

    use ylong_io::Source;

    use super::Pipe;

    /// UT test cases for Pipe.
    ///
    /// # Brief
    /// 1. Create a `Pipe` from `std::fs::File`.
    /// 2. Write something into `Pipe`.
    /// 3. Read from `Pipe` and check result.
    #[test]
    fn ut_process_pipe_test() {
        let arg = "Hello, world!";
        let file_path = "pipe_file0.txt";

        let mut file = std::fs::File::options()
            .read(true)
            .write(true)
            .create(true)
            .open(file_path)
            .unwrap();
        let seek = file.stream_position().unwrap();
        let mut pipe = Pipe::from(file);

        assert!(pipe.get_fd().as_raw_fd() >= 0);
        assert!(pipe.as_fd().as_raw_fd() >= 0);
        assert!(pipe.as_raw_fd() >= 0);

        (&pipe).write_all(arg.as_bytes()).unwrap();
        (&pipe).flush().unwrap();

        pipe.fd.seek(std::io::SeekFrom::Start(seek)).unwrap();

        let mut buf = [0; 13];
        (&pipe).read_exact(&mut buf).unwrap();
        assert_eq!(buf, arg.as_bytes());

        std::fs::remove_file(file_path).unwrap();
    }
}

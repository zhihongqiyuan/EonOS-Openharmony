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

use std::ffi::OsStr;
use std::io::{Error, ErrorKind, Result};
use std::os::unix::ffi::OsStrExt;
use std::path::Path;
use std::{ascii, fmt};

fn sun_path_offset(sockaddr: &libc::sockaddr_un) -> usize {
    let path = &sockaddr.sun_path as *const _ as usize;
    let base = sockaddr as *const _ as usize;
    path - base
}

pub(crate) fn socket_addr_trans_un(path: &Path) -> Result<(libc::sockaddr_un, libc::socklen_t)> {
    let sockaddr: std::mem::MaybeUninit<libc::sockaddr_un> =
        std::mem::MaybeUninit::<libc::sockaddr_un>::zeroed();

    let mut sockaddr = unsafe { sockaddr.assume_init() };

    sockaddr.sun_family = libc::AF_UNIX as libc::sa_family_t;

    let bytes = path.as_os_str().as_bytes();
    if bytes.first() == Some(&0) {
        return Err(Error::new(
            ErrorKind::InvalidInput,
            "paths must not start with interior null bytes",
        ));
    }
    // Checks path len.
    if bytes.len() >= sockaddr.sun_path.len() {
        return Err(Error::new(
            ErrorKind::InvalidInput,
            "path must be shorter than libc::sockaddr_un.sun_path",
        ));
    }

    for (dest, source) in sockaddr.sun_path.iter_mut().zip(bytes.iter()) {
        *dest = *source as libc::c_char;
    }

    let mut socklen = sun_path_offset(&sockaddr) + bytes.len();

    match bytes.first() {
        Some(&0) | None => {}
        Some(_) => socklen += 1,
    }

    Ok((sockaddr, socklen as libc::socklen_t))
}

/// This structure is necessary because the `socketaddr` returned
/// when the `listener` calls `accept` cannot be derived from `std`.
pub struct SocketAddr {
    sockaddr: libc::sockaddr_un,
    socklen: libc::socklen_t,
}

impl SocketAddr {
    pub(crate) fn from_parts(sockaddr: libc::sockaddr_un, socklen: libc::socklen_t) -> SocketAddr {
        SocketAddr { sockaddr, socklen }
    }
}

impl fmt::Debug for SocketAddr {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        let offset = sun_path_offset(&self.sockaddr);
        if (self.socklen as usize) < offset {
            return write!(fmt, "(unnamed)");
        }
        let len = self.socklen as usize - offset;
        let path = unsafe { &*(&self.sockaddr.sun_path as *const [libc::c_char] as *const [u8]) };

        if len == 0 || (cfg!(not(target_os = "linux")) && self.sockaddr.sun_path[0] == 0) {
            write!(fmt, "(unnamed)")
        } else if self.sockaddr.sun_path[0] == 0 {
            write!(fmt, "\"")?;
            let name = &path[1..len];
            for byte in name.iter().cloned().flat_map(ascii::escape_default) {
                write!(fmt, "{}", byte as char)?;
            }
            write!(fmt, "\"(abstract)")
        } else {
            write!(
                fmt,
                "{:?} (pathname)",
                <std::ffi::OsStr as std::convert::AsRef<Path>>::as_ref(OsStr::from_bytes(
                    &path[..len - 1]
                ))
            )
        }
    }
}

#[cfg(test)]
mod test {
    use crate::SocketAddr;

    /// UT for uds sockaddr debug info
    ///
    /// # Brief
    /// 1. Create an UDS socket address
    /// 2. Check if the debug info is correct
    #[test]
    fn ut_uds_socket_addr_debug_info() {
        let sock_addr = libc::sockaddr_un {
            sun_family: 1,
            sun_path: [2; 108],
        };

        let addr = SocketAddr::from_parts(sock_addr, 10);
        let fmt = format!("{addr:?}");
        assert!(fmt.contains("\"\\u{2}\\u{2}\\u{2}\\u{2}\\u{2}\\u{2}\\u{2}\" (pathname)"));
    }
}

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
use std::net::{Ipv4Addr, Ipv6Addr, SocketAddr};
use std::os::unix::io::AsRawFd;
use std::{fmt, io, net};

use super::UdpSock;
use crate::source::Fd;
use crate::{Interest, Selector, Source, Token};

/// UdpSocket. The bottom layer uses std::net::UdpSocket。
/// UdpSocket supports bind\connect\send\recv\send_to\recv_from\broadcast.
///
/// # Examples
///
/// ```rust
/// use std::io;
///
/// use ylong_io::UdpSocket;
///
/// async fn io_func() -> io::Result<()> {
///     let sender_addr = "127.0.0.1:8081".parse().unwrap();
///     let receiver_addr = "127.0.0.1:8082".parse().unwrap();
///     let mut sender = UdpSocket::bind(sender_addr)?;
///     let mut receiver = UdpSocket::bind(receiver_addr)?;
///
///     let len = sender.send_to(b"Hello", receiver_addr)?;
///     println!("{:?} bytes sent", len);
///
///     let mut buf = [0; 1024];
///     let (len, addr) = receiver.recv_from(&mut buf)?;
///     println!("{:?} bytes received from {:?}", len, addr);
///
///     let connected_sender = match sender.connect(receiver_addr) {
///         Ok(socket) => socket,
///         Err(e) => {
///             return Err(e);
///         }
///     };
///     let connected_receiver = match receiver.connect(sender_addr) {
///         Ok(socket) => socket,
///         Err(e) => {
///             return Err(e);
///         }
///     };
///     let len = connected_sender.send(b"Hello")?;
///     println!("{:?} bytes sent", len);
///     let len = connected_receiver.recv(&mut buf)?;
///     println!("{:?} bytes received from {:?}", len, sender_addr);
///     Ok(())
/// }
/// ```
pub struct UdpSocket {
    pub(crate) inner: net::UdpSocket,
}

impl UdpSocket {
    /// Creates a UDP socket from the given address.
    /// return io::Error if errors happen.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080".parse().unwrap();
    ///     if let Ok(mut sock) = UdpSocket::bind(addr) {
    ///         println!("socket binds successfully");
    ///     };
    ///     Ok(())
    /// }
    /// ```
    pub fn bind(addr: SocketAddr) -> io::Result<UdpSocket> {
        let socket = UdpSock::new_socket(addr)?;
        socket.bind(addr)
    }

    /// Creates a new `UdpSocket` from a standard `net::UdpSocket`.
    ///
    /// This function is intended to be used to wrap a UDP socket from the
    /// standard library in the io equivalent. The conversion assumes nothing
    /// about the underlying socket; it is left up to the user to set it in
    /// non-blocking mode.
    pub fn from_std(socket: net::UdpSocket) -> UdpSocket {
        UdpSocket { inner: socket }
    }

    /// Returns the local address that this socket is bound to.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080".parse().unwrap();
    ///     let mut sock = UdpSocket::bind(addr)?;
    ///     let local_addr = sock.local_addr()?;
    ///     Ok(())
    /// }
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Sets the value for the IP_TTL option on this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// socket.set_ttl(100).expect("set_ttl call failed");
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.inner.set_ttl(ttl)
    }

    /// Sets the value for the IP_TTL option on this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// socket.set_ttl(100).expect("set_ttl call failed");
    /// assert_eq!(socket.ttl().unwrap(), 100);
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.inner.ttl()
    }

    /// Sends data on the socket to the given address. On success, returns the
    /// number of bytes written.
    ///
    /// # Return value
    /// The function returns:
    /// * `Ok(n)` n is the number of bytes sent.
    /// * `Err(e)` if an error is encountered.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let sock = UdpSocket::bind(local_addr)?;
    ///     let remote_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let len = sock.send_to(b"hello world", remote_addr)?;
    ///     println!("Sent {} bytes", len);
    ///     Ok(())
    /// }
    /// ```
    pub fn send_to(&self, buf: &[u8], target: SocketAddr) -> io::Result<usize> {
        let inner = &self.inner;
        inner.send_to(buf, target)
    }

    /// Receives a single datagram message on the socket. On success, returns
    /// the number of bytes read and the origin. The function must be called
    /// with valid byte array buf of sufficient size to hold the message bytes.
    /// If a message is too long to fit in the supplied buffer, excess bytes may
    /// be discarded.
    ///
    /// # Return value
    /// The function returns:
    /// * `Ok((n, addr))` n is the number of bytes received, addr is the address
    ///   of sender.
    /// * `Err(e)` if an error is encountered.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let sock = UdpSocket::bind(local_addr)?;
    ///     let mut recv_buf = [0_u8; 12];
    ///     let (len, addr) = sock.recv_from(&mut recv_buf)?;
    ///     println!("received {:?} bytes from {:?}", len, addr);
    ///     Ok(())
    /// }
    /// ```
    pub fn recv_from(&self, buf: &mut [u8]) -> io::Result<(usize, SocketAddr)> {
        let inner = &self.inner;
        inner.recv_from(buf)
    }

    /// Receives single datagram on the socket from the remote address to which
    /// it is connected, without removing the message from input queue. On
    /// success, returns the number of bytes peeked.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("couldn't bind to address");
    /// let mut buf = [0; 10];
    /// let (number_of_bytes, src_addr) = socket.peek_from(&mut buf).expect("Didn't receive data");
    /// let filled_buf = &mut buf[..number_of_bytes];
    /// ```
    pub fn peek_from(&self, buf: &mut [u8]) -> io::Result<(usize, SocketAddr)> {
        let inner = &self.inner;
        inner.peek_from(buf)
    }

    /// Connects the UDP socket setting the default destination for send()
    /// and limiting packets that are read via recv from the address specified
    /// in addr. return io::Error if errors happen.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let sock = UdpSocket::bind(local_addr)?;
    ///     let remote_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let connected_sock = match sock.connect(remote_addr) {
    ///         Ok(socket) => socket,
    ///         Err(e) => {
    ///             return Err(e);
    ///         }
    ///     };
    ///     Ok(())
    /// }
    /// ```
    pub fn connect(self, addr: SocketAddr) -> io::Result<ConnectedUdpSocket> {
        let socket = ConnectedUdpSocket::from_std(self.inner);
        socket.inner.connect(addr)?;
        Ok(socket)
    }

    /// Sets the value of the `SO_BROADCAST` option for this socket.
    /// When enabled, this socket is allowed to send packets to a broadcast
    /// address.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let broadcast_socket = UdpSocket::bind(local_addr)?;
    ///     if broadcast_socket.broadcast()? == false {
    ///         broadcast_socket.set_broadcast(true)?;
    ///     }
    ///     assert_eq!(broadcast_socket.broadcast()?, true);
    ///     Ok(())
    /// }
    /// ```
    pub fn set_broadcast(&self, on: bool) -> io::Result<()> {
        self.inner.set_broadcast(on)
    }

    /// Gets the value of the `SO_BROADCAST` option for this socket.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let broadcast_socket = UdpSocket::bind(local_addr)?;
    ///     assert_eq!(broadcast_socket.broadcast()?, false);
    ///     Ok(())
    /// }
    /// ```
    pub fn broadcast(&self) -> io::Result<bool> {
        self.inner.broadcast()
    }

    /// Gets the value of the SO_ERROR option on this socket.
    /// This will retrieve the stored error in the underlying socket, clearing
    /// the field in the process. This can be useful for checking errors between
    /// calls.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let addr = "127.0.0.1:34254".parse().unwrap();
    /// let socket = UdpSocket::bind(addr).expect("couldn't bind to address");
    /// match socket.take_error() {
    ///     Ok(Some(error)) => println!("UdpSocket error: {error:?}"),
    ///     Ok(None) => println!("No error"),
    ///     Err(error) => println!("UdpSocket.take_error failed: {error:?}"),
    /// }
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.inner.take_error()
    }

    /// Gets the value of the IP_MULTICAST_LOOP option for this socket.
    pub fn multicast_loop_v4(&self) -> io::Result<bool> {
        self.inner.multicast_loop_v4()
    }

    /// Sets the value of the IP_MULTICAST_LOOP option for this socket.
    /// If enabled, multicast packets will be looped back to the local socket.
    /// Note that this might not have any effect on IPv6 sockets.
    pub fn set_multicast_loop_v4(&self, multicast_loop_v4: bool) -> io::Result<()> {
        self.inner.set_multicast_loop_v4(multicast_loop_v4)
    }

    /// Gets the value of the IP_MULTICAST_TTL option for this socket.
    pub fn multicast_ttl_v4(&self) -> io::Result<u32> {
        self.inner.multicast_ttl_v4()
    }

    /// Sets the value of the IP_MULTICAST_TTL option for this socket.
    /// Indicates the time-to-live value of outgoing multicast packets for this
    /// socket. The default value is 1 which means that multicast packets don't
    /// leave the local network unless explicitly requested. Note that this
    /// might not have any effect on IPv6 sockets.
    pub fn set_multicast_ttl_v4(&self, multicast_ttl_v4: u32) -> io::Result<()> {
        self.inner.set_multicast_ttl_v4(multicast_ttl_v4)
    }

    /// Gets the value of the IPV6_MULTICAST_LOOP option for this socket.
    pub fn multicast_loop_v6(&self) -> io::Result<bool> {
        self.inner.multicast_loop_v6()
    }

    /// Sets the value of the IPV6_MULTICAST_LOOP option for this socket.
    /// Controls whether this socket sees the multicast packets it sends itself.
    /// Note that this might not have any affect on IPv4 sockets.
    pub fn set_multicast_loop_v6(&self, multicast_loop_v6: bool) -> io::Result<()> {
        self.inner.set_multicast_loop_v6(multicast_loop_v6)
    }

    /// Executes an operation of the IP_ADD_MEMBERSHIP type.
    pub fn join_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
        self.inner.join_multicast_v4(multiaddr, interface)
    }

    /// Executes an operation of the IPV6_ADD_MEMBERSHIP type.
    pub fn join_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32) -> io::Result<()> {
        self.inner.join_multicast_v6(multiaddr, interface)
    }

    /// Executes an operation of the IP_DROP_MEMBERSHIP type.
    pub fn leave_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
        self.inner.leave_multicast_v4(multiaddr, interface)
    }

    /// Executes an operation of the IPV6_DROP_MEMBERSHIP type.
    pub fn leave_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32) -> io::Result<()> {
        self.inner.leave_multicast_v6(multiaddr, interface)
    }
}

/// An already connected, non-blocking UdpSocket
pub struct ConnectedUdpSocket {
    pub(crate) inner: net::UdpSocket,
}

impl ConnectedUdpSocket {
    /// Creates a new `UdpSocket` from a standard `net::UdpSocket`.
    ///
    /// This function is intended to be used to wrap a UDP socket from the
    /// standard library in the io equivalent. The conversion assumes nothing
    /// about the underlying socket; it is left up to the user to set it in
    /// non-blocking mode.
    pub fn from_std(socket: net::UdpSocket) -> ConnectedUdpSocket {
        ConnectedUdpSocket { inner: socket }
    }

    /// Returns the local address that this socket is bound to.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080".parse().unwrap();
    ///     let mut sock = UdpSocket::bind(addr)?;
    ///     let remote_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let connected_sock = match sock.connect(remote_addr) {
    ///         Ok(socket) => socket,
    ///         Err(e) => {
    ///             return Err(e);
    ///         }
    ///     };
    ///     let local_addr = connected_sock.local_addr()?;
    ///     Ok(())
    /// }
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Returns the socket address of the remote peer this socket was connected
    /// to.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let addr = "127.0.0.1:8080".parse().unwrap();
    ///     let peer_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let mut sock = UdpSocket::bind(addr)?;
    ///     let connected_sock = match sock.connect(peer_addr) {
    ///         Ok(socket) => socket,
    ///         Err(e) => {
    ///             return Err(e);
    ///         }
    ///     };
    ///     assert_eq!(connected_sock.peer_addr()?, peer_addr);
    ///     Ok(())
    /// }
    /// ```
    pub fn peer_addr(&self) -> io::Result<SocketAddr> {
        self.inner.peer_addr()
    }

    /// Sets the value for the IP_TTL option on this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let receiver_addr = "127.0.0.1:8081".parse().unwrap();
    /// let connect_socket = socket
    ///     .connect(receiver_addr)
    ///     .expect("Connect Socket Failed!");
    /// connect_socket.set_ttl(100).expect("set_ttl call failed");
    /// ```
    pub fn set_ttl(&self, ttl: u32) -> io::Result<()> {
        self.inner.set_ttl(ttl)
    }

    /// Sets the value for the IP_TTL option on this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let receiver_addr = "127.0.0.1:8081".parse().unwrap();
    /// let connect_socket = socket
    ///     .connect(receiver_addr)
    ///     .expect("Connect Socket Failed!");
    /// connect_socket.set_ttl(100).expect("set_ttl call failed");
    /// assert_eq!(connect_socket.ttl().unwrap(), 100);
    /// ```
    pub fn ttl(&self) -> io::Result<u32> {
        self.inner.ttl()
    }

    /// Sends data on the socket to the remote address that the socket is
    /// connected to. The connect method will connect this socket to a
    /// remote address. This method will fail if the socket is not
    /// connected.
    ///
    /// # Return
    /// On success, the number of bytes sent is returned, otherwise, the
    /// encountered error is returned.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let sock = UdpSocket::bind(local_addr)?;
    ///     let remote_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let connected_sock = match sock.connect(remote_addr) {
    ///         Ok(socket) => socket,
    ///         Err(e) => {
    ///             return Err(e);
    ///         }
    ///     };
    ///     connected_sock.send(b"Hello")?;
    ///     Ok(())
    /// }
    /// ```
    pub fn send(&self, buf: &[u8]) -> io::Result<usize> {
        let inner = &self.inner;
        inner.send(buf)
    }

    /// Receives a single datagram message on the socket from the remote address
    /// to which it is connected. On success, returns the number of bytes read.
    /// The function must be called with valid byte array buf of sufficient size
    /// to hold the message bytes. If a message is too long to fit in the
    /// supplied buffer, excess bytes may be discarded. The connect method
    /// will connect this socket to a remote address. This method will fail
    /// if the socket is not connected.
    ///
    /// # Return value
    /// The function returns:
    /// * `Ok(n)` n is is the number of bytes received
    /// * `Err(e)` if an error is encountered.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let sock = UdpSocket::bind(local_addr)?;
    ///     let remote_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let connected_sock = match sock.connect(remote_addr) {
    ///         Ok(socket) => socket,
    ///         Err(e) => {
    ///             return Err(e);
    ///         }
    ///     };
    ///     let mut recv_buf = [0_u8; 12];
    ///     let n = connected_sock.recv(&mut recv_buf[..])?;
    ///     println!("received {} bytes {:?}", n, &recv_buf[..n]);
    ///     Ok(())
    /// }
    /// ```
    pub fn recv(&self, buf: &mut [u8]) -> io::Result<usize> {
        let inner = &self.inner;
        inner.recv(buf)
    }

    /// Receives single datagram on the socket from the remote address to which
    /// it is connected, without removing the message from input queue. On
    /// success, returns the number of bytes peeked.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:8082".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("couldn't bind to address");
    /// let connect_socket = socket
    ///     .connect(receiver_addr)
    ///     .expect("connect function failed");
    /// let mut buf = [0; 10];
    /// match connect_socket.peek(&mut buf) {
    ///     Ok(received) => println!("received {received} bytes"),
    ///     Err(e) => println!("peek function failed: {e:?}"),
    /// }
    /// ```
    pub fn peek(&self, buf: &mut [u8]) -> io::Result<usize> {
        let inner = &self.inner;
        inner.peek(buf)
    }

    /// Sets the value of the `SO_BROADCAST` option for this socket.
    /// When enabled, this socket is allowed to send packets to a broadcast
    /// address.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let receiver_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let socket = UdpSocket::bind(local_addr)?;
    ///     let connect_socket = socket
    ///         .connect(receiver_addr)
    ///         .expect("connect function failed");
    ///     if connect_socket.broadcast()? == false {
    ///         connect_socket.set_broadcast(true)?;
    ///     }
    ///     assert_eq!(connect_socket.broadcast()?, true);
    ///     Ok(())
    /// }
    /// ```
    pub fn set_broadcast(&self, on: bool) -> io::Result<()> {
        self.inner.set_broadcast(on)
    }

    /// Gets the value of the `SO_BROADCAST` option for this socket.
    ///
    /// # Examples
    ///
    /// ```rust
    /// use std::io;
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// async fn io_func() -> io::Result<()> {
    ///     let local_addr = "127.0.0.1:8080".parse().unwrap();
    ///     let receiver_addr = "127.0.0.1:8081".parse().unwrap();
    ///     let socket = UdpSocket::bind(local_addr)?;
    ///     let connect_socket = socket
    ///         .connect(receiver_addr)
    ///         .expect("connect function failed");
    ///     assert_eq!(connect_socket.broadcast()?, false);
    ///     Ok(())
    /// }
    /// ```
    pub fn broadcast(&self) -> io::Result<bool> {
        self.inner.broadcast()
    }

    /// Gets the value of the IP_MULTICAST_LOOP option for this socket.
    pub fn multicast_loop_v4(&self) -> io::Result<bool> {
        self.inner.multicast_loop_v4()
    }

    /// Sets the value of the IP_MULTICAST_LOOP option for this socket.
    /// If enabled, multicast packets will be looped back to the local socket.
    /// Note that this might not have any effect on IPv6 sockets.
    pub fn set_multicast_loop_v4(&self, multicast_loop_v4: bool) -> io::Result<()> {
        self.inner.set_multicast_loop_v4(multicast_loop_v4)
    }

    /// Gets the value of the IP_MULTICAST_TTL option for this socket.
    pub fn multicast_ttl_v4(&self) -> io::Result<u32> {
        self.inner.multicast_ttl_v4()
    }

    /// Sets the value of the IP_MULTICAST_TTL option for this socket.
    /// Indicates the time-to-live value of outgoing multicast packets for this
    /// socket. The default value is 1 which means that multicast packets don't
    /// leave the local network unless explicitly requested. Note that this
    /// might not have any effect on IPv6 sockets.
    pub fn set_multicast_ttl_v4(&self, multicast_ttl_v4: u32) -> io::Result<()> {
        self.inner.set_multicast_ttl_v4(multicast_ttl_v4)
    }

    /// Gets the value of the IPV6_MULTICAST_LOOP option for this socket.
    pub fn multicast_loop_v6(&self) -> io::Result<bool> {
        self.inner.multicast_loop_v6()
    }

    /// Sets the value of the IPV6_MULTICAST_LOOP option for this socket.
    /// Controls whether this socket sees the multicast packets it sends itself.
    /// Note that this might not have any affect on IPv4 sockets.
    pub fn set_multicast_loop_v6(&self, multicast_loop_v6: bool) -> io::Result<()> {
        self.inner.set_multicast_loop_v6(multicast_loop_v6)
    }

    /// Executes an operation of the IP_ADD_MEMBERSHIP type.
    pub fn join_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
        self.inner.join_multicast_v4(multiaddr, interface)
    }

    /// Executes an operation of the IPV6_ADD_MEMBERSHIP type.
    pub fn join_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32) -> io::Result<()> {
        self.inner.join_multicast_v6(multiaddr, interface)
    }

    /// Executes an operation of the IP_DROP_MEMBERSHIP type.
    pub fn leave_multicast_v4(&self, multiaddr: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
        self.inner.leave_multicast_v4(multiaddr, interface)
    }

    /// Executes an operation of the IPV6_DROP_MEMBERSHIP type.
    pub fn leave_multicast_v6(&self, multiaddr: &Ipv6Addr, interface: u32) -> io::Result<()> {
        self.inner.leave_multicast_v6(multiaddr, interface)
    }

    /// Gets the value of the SO_ERROR option on this socket.
    /// This will retrieve the stored error in the underlying socket, clearing
    /// the field in the process. This can be useful for checking errors between
    /// calls.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let addr = "127.0.0.1:34253".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:34254".parse().unwrap();
    /// let socket = UdpSocket::bind(addr).expect("couldn't bind to address");
    /// let connected_sender = socket
    ///     .connect(receiver_addr)
    ///     .expect("connect function failed");
    /// match connected_sender.take_error() {
    ///     Ok(Some(error)) => println!("ConnectedUdpSocket error: {error:?}"),
    ///     Ok(None) => println!("No error"),
    ///     Err(error) => println!("ConnectedUdpSocket.take_error failed: {error:?}"),
    /// }
    /// ```
    pub fn take_error(&self) -> io::Result<Option<io::Error>> {
        self.inner.take_error()
    }
}

impl fmt::Debug for UdpSocket {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        self.inner.fmt(f)
    }
}

impl fmt::Debug for ConnectedUdpSocket {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        self.inner.fmt(f)
    }
}

impl Source for UdpSocket {
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

impl Source for ConnectedUdpSocket {
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

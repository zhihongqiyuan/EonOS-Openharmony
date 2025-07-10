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
use std::os::windows::io::{AsRawSocket, RawSocket};
use std::{fmt, io, net};

use crate::source::Fd;
use crate::sys::windows::udp::UdpSock;
use crate::sys::NetState;
use crate::{Interest, Selector, Source, Token};

/// A UDP socket.
pub struct UdpSocket {
    pub(crate) inner: net::UdpSocket,
    /// State is None if the socket has not been Registered.
    pub(crate) state: NetState,
}

impl UdpSocket {
    /// Binds a new UdpSocket to the specific address.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let sender = UdpSocket::bind(sender_addr).expect("UdpSocket bind fail!");
    /// ```
    pub fn bind(addr: SocketAddr) -> io::Result<UdpSocket> {
        let sock = UdpSock::new_socket(addr)?;
        sock.bind(addr).map(UdpSocket::from_std)
    }

    /// Connects peer address to get ConnectedUdpSocket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:8082".parse().unwrap();
    ///
    /// let sender = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let connected_sender = sender
    ///     .connect(receiver_addr)
    ///     .expect("Connect Socket Failed!");
    /// ```
    pub fn connect(self, addr: SocketAddr) -> io::Result<ConnectedUdpSocket> {
        let socket = ConnectedUdpSocket::from_std(self);
        socket.inner.connect(addr)?;
        Ok(socket)
    }

    /// Convert net::UdpSocket to ylong_io::UdpSocket
    pub fn from_std(socket: net::UdpSocket) -> UdpSocket {
        UdpSocket {
            inner: socket,
            state: NetState::new(),
        }
    }

    /// Returns the socket address that this socket was created from.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// assert_eq!(
    ///     socket.local_addr().unwrap(),
    ///     SocketAddr::V4(SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 8081))
    /// );
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Sends data on the socket to the given address. On success, returns the
    /// number of bytes written.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:8082".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// socket
    ///     .send_to(&[0; 10], receiver_addr)
    ///     .expect("Send Socket Failed!");
    /// ```
    pub fn send_to(&self, buf: &[u8], target: SocketAddr) -> io::Result<usize> {
        self.state
            .try_io(|inner| inner.send_to(buf, target), &self.inner)
    }

    /// Receives a single datagram message on the socket. On success, returns
    /// the number of bytes read and the origin.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let mut buf = [0; 10];
    /// let (number_of_bytes, src_addr) = socket.recv_from(&mut buf).expect("Didn't receive data");
    /// let filled_buf = &mut buf[..number_of_bytes];
    /// ```
    pub fn recv_from(&self, buf: &mut [u8]) -> io::Result<(usize, SocketAddr)> {
        self.state.try_io(|inner| inner.recv_from(buf), &self.inner)
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
        self.state.try_io(|inner| inner.peek_from(buf), &self.inner)
    }

    /// Sets the value of the SO_BROADCAST option for this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("couldn't bind to address");
    /// socket
    ///     .set_broadcast(false)
    ///     .expect("set_broadcast call failed");
    /// ```
    pub fn set_broadcast(&self, on: bool) -> io::Result<()> {
        self.inner.set_broadcast(on)
    }

    /// Gets the value of the SO_BROADCAST option for this socket.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    ///
    /// let socket = UdpSocket::bind(sender_addr).expect("couldn't bind to address");
    /// socket
    ///     .set_broadcast(false)
    ///     .expect("set_broadcast call failed");
    /// assert_eq!(socket.broadcast().unwrap(), false);
    /// ```
    pub fn broadcast(&self) -> io::Result<bool> {
        self.inner.broadcast()
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

/// UdpSocket which has been connected.
pub struct ConnectedUdpSocket {
    pub(crate) inner: net::UdpSocket,
    /// State is None if the socket has not been Registered.
    pub(crate) state: NetState,
}

impl ConnectedUdpSocket {
    /// Convert net::UdpSocket to ylong_io::ConnectedUdpSocket
    pub fn from_std(socket: UdpSocket) -> ConnectedUdpSocket {
        ConnectedUdpSocket {
            inner: socket.inner,
            state: socket.state,
        }
    }

    /// Returns the socket address that this socket was created from.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:8082".parse().unwrap();
    ///
    /// let sender = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let connected_sender = sender
    ///     .connect(receiver_addr)
    ///     .expect("Connect Socket Failed!");
    ///
    /// assert_eq!(
    ///     connected_sender.local_addr().unwrap(),
    ///     SocketAddr::V4(SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 8081))
    /// );
    /// ```
    pub fn local_addr(&self) -> io::Result<SocketAddr> {
        self.inner.local_addr()
    }

    /// Returns the socket address of the remote peer to which the socket is
    /// connected.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
    ///
    /// use ylong_io::UdpSocket;
    ///
    /// let sender_addr = "127.0.0.1:8081".parse().unwrap();
    /// let receiver_addr = "127.0.0.1:8082".parse().unwrap();
    ///
    /// let sender = UdpSocket::bind(sender_addr).expect("Bind Socket Failed!");
    /// let connected_sender = sender
    ///     .connect(receiver_addr)
    ///     .expect("Connect Socket Failed!");
    ///
    /// assert_eq!(
    ///     connected_sender.peer_addr().unwrap(),
    ///     SocketAddr::V4(SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 8082))
    /// );
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

    /// Sends data on the socket to the remote address to which it is connected.
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
    /// let connected_sender = socket
    ///     .connect(receiver_addr)
    ///     .expect("connect function failed");
    /// connected_sender
    ///     .send(&[0, 1, 2])
    ///     .expect("couldn't send message");
    /// ```
    pub fn send(&self, buf: &[u8]) -> io::Result<usize> {
        self.state.try_io(|inner| inner.send(buf), &self.inner)
    }

    /// Receives a single datagram message on the socket from the remote address
    /// to which it is connected. On success, returns the number of bytes read.
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
    /// let connected_sender = socket
    ///     .connect(receiver_addr)
    ///     .expect("connect function failed");
    /// let mut buf = [0; 10];
    /// match connected_sender.recv(&mut buf) {
    ///     Ok(received) => println!("received {} bytes {:?}", received, &buf[..received]),
    ///     Err(e) => println!("recv function failed: {:?}", e),
    /// }
    /// ```
    pub fn recv(&self, buf: &mut [u8]) -> io::Result<usize> {
        self.state.try_io(|inner| inner.recv(buf), &self.inner)
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
        self.state.try_io(|inner| inner.peek(buf), &self.inner)
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
        self.state
            .register(selector, token, interests, self.get_fd())
    }

    fn deregister(&mut self, _selector: &Selector) -> io::Result<()> {
        self.state.deregister()
    }

    fn get_fd(&self) -> Fd {
        self.inner.as_raw_socket()
    }
}

impl Source for ConnectedUdpSocket {
    fn register(
        &mut self,
        selector: &Selector,
        token: Token,
        interests: Interest,
    ) -> io::Result<()> {
        self.state
            .register(selector, token, interests, self.get_fd())
    }

    fn deregister(&mut self, _selector: &Selector) -> io::Result<()> {
        self.state.deregister()
    }

    fn get_fd(&self) -> Fd {
        self.inner.as_raw_socket()
    }
}

impl AsRawSocket for UdpSocket {
    fn as_raw_socket(&self) -> RawSocket {
        self.inner.as_raw_socket()
    }
}

impl AsRawSocket for ConnectedUdpSocket {
    fn as_raw_socket(&self) -> RawSocket {
        self.inner.as_raw_socket()
    }
}

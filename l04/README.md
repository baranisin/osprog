Lab 4
=====

echo server
-----------

*In this excercise you can assume that output to the console is immediate
and it won't block your program, i.e. you don't have to implement non-blocking
writes to stdout/stderr.*

Create a program that accepts TCP connections (on port 1234) ...

1.  ... one after another and  writes all incomming data to stdout. (Start
    each new connection with the line `New connection from [ipaddr]:[port]`
1.  ... and writes data out to stdout as they come in. Everytime the source
    (connection) of the data changes, prefix the first line with `[ipaddr]:[port]`
1.  and implements a **echo** server: when data arrives on a connection, write it
    back to that connection.


### Implementation

Most of the information needed can be found on the slides.

You can use `netcat` (`nc`) or `socat` to test your program:

```sh
nc localhost 1234
socat - TCP4:localhost:1234
```

Non-blocking mode:

- use `fcnt` to switch sockets to non-blocking mode,
- or use the `accept4` call that allows you to specify flags for the new socket
- or use the `MSG_DONTWAIT` flag with every `recv` invocation.

Don't forget that in #3 writes to clients may block too (if they don't read back,
or read slowly). Buffer some ammount of data for them (say couple of megabytes) in
case it's just temporary, then just drop it if your buffer fills (you'll need seperate
buffer for each client).

For the homework assignemt you can simulate such client by "piping" the output
from nc into a command that doesn't read anything (and doesn't finish) and use
another instance of nc to send data (unfortunately nc won't send data if it is
block on receiving):

```sh
nc localhost 1234 | while sleep 1; do true ; done
```
nc will read data from network and write it to the pipe until the pipe gets
full and then stop reading data (pipe capacity is usually 65536 bytes). Note
that the TCP/IP network stack will still buffer addintional data on the sending
and receiving size (usually around 4MB on each size), so you need to write enough
data to your server to have it happen (remember `/dev/zero` or `/dev/urandom` ?).
Remeber you can run strace to see what's happening:

```sh
strace nc localhost 1234 < /dev/urandom
```

Homework assignment 01
----------------------

Create a simple TCP "chat": multiple clients can connect to it, every time a
client sends data to the server it is "forwarded" (sent) to all the connected
clients.

Your program should buffer at least 1MB of data for each client. If a buffer
for a client fills, drop additional data and print a warning to the console.

Your program doesn't need to support more that 1000 simlutaneous connections
(or below `FD_SETSIZE`). If it can manage more then `FD_SETSIZE` clients, it might
be worth a bonus point...

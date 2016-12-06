Homework assignment 01
======================

*This assignment is due Tuesday Nov 15 23:59:59. See [Submitting](#submitting)
below for instructions*

Chat (30pt)
-----------

Create a simple TCP "chat": multiple clients can connect to it, every time a
client sends data to the server it is "forwarded" (sent) to all the connected
clients.

Your program should buffer at least 1MB of data for each client. If a buffer
for a client fills, drop additional data and print a warning to the console.

Your program doesn't need to support more that 1000 simlutaneous connections
(or below `FD_SETSIZE`). If it can manage more then `FD_SETSIZE` clients, it might
be worth a bonus point...

Submitting
----------

To get access to your private repository send an email to
`siska@ii.fmph.uniba.sk` with your **name**, **AIS login** and **GitHub
login**.  Use "osprog registration" as the subject of the email.

Submit yur solution by commiting  file named `chat.c` (or `chat.cpp`) in
the `as01` folder (of your fork of your private repository) and create a
pull request against the **`as01` branch** of you private repository.


Clients that send data, but read slowly
---------------------------------------

You can simulate clients that don't read / read data slowly by "piping" the
output from nc into a command that doesn't read anything (and doesn't finish)
and use another instance of nc to send data (unfortunately nc won't send data
if it is block on receiving):

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

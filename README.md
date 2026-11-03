*This project has been created as part of the 42 curriculum by alexfuen, tseche.*

# ft_irc

# Description

*ft_irc* is an IRC (Internet Relay Chat) server developed for the 42 `ft_irc` project.
It is written in C++98 and handles multiple clients simultaneously through non-blocking TCP/IP sockets and I/O multiplexing with `epoll`.

Clients can connect with any standard IRC client (HexChat, Irssi, LimeChat, ...), authenticate with a password, register a nickname, create or join channels, exchange private messages and use channel operator commands.

### Key concepts

- Non-blocking sockets and event-driven architecture
- I/O multiplexing with `epoll`
- TCP stream buffering and partial command reassembly
- Client registration and authentication
- Channel management and operator privileges
- IRC message formatting and reply codes

### External functions

Functions are grouped by the library they come from.

**libc**

* `socket`, `bind`, `listen`, `accept` — TCP server setup.
* `recv`, `send` — client communication.
* `close` — socket cleanup.
* `signal` — graceful interruption handling.
* `htons` — network byte order conversion.

**epoll (Linux)**

* `epoll_create1`, `epoll_ctl`, `epoll_wait` — event loop and file descriptor monitoring.

### Libraries

* `libc`
* `libstdc++`

## Team Information

| Member | 42 login | Role | Main responsibilities |
| --- | --- | --- | --- |
| Alexandre | `alexfuen` | Developer | server core, epoll event loop, channel management, MODE
| Titouan | `tseche` | Developer | command parsing, JOIN, USER, TOPIC, PRIVMSG, Makefile

## Supported Commands

| Command | Description |
| --- | --- |
| `PASS` | Provide the server password. |
| `NICK` | Set or change the client nickname. |
| `USER` | Register the client username. |
| `JOIN` | Join an existing channel or create a new one. |
| `PRIVMSG` | Send a private message to a user or a channel. |
| `TOPIC` | View or change the topic of a channel. |
| `KICK` | Eject a client from a channel. |
| `INVITE` | Invite a user to a channel. |
| `MODE` | Manage channel modes (`i`, `t`, `k`, `o`, `l`). |

---

# Instructions

## Requirements

The project requires:

- Make
- Clang Compiler
- Linux (`epoll`)
- An IRC client for testing (HexChat, Irssi, LimeChat, or `nc`)

---

## Configuration

Clone the repository:

```bash
git clone <repository-url>
cd ft_irc
```

---

## Start the Application

Build with:

```bash
make
```
Then launch with:
```bash
./ircserv <port> <password>
```
For example:
```bash
./ircserv 6667 mysecretpassword
```

You can also test raw TCP packets with `nc`:
```bash
nc -C 127.0.0.1 6667
```

---

# Resources

Main documentation and references used during development:

- RFC 1459 — Internet Relay Chat Protocol — https://www.rfc-editor.org/info/rfc1459/
- Classes in C++ — https://en.cppreference.com/cpp/language/classes
- Linux man pages — https://man7.org/linux/man-pages/

## AI Usage

AI tools were used as a supplementary resource during the project, mainly for technical research, troubleshooting, reviewing edge cases, checking project requirements and assisting with documentation.
The team remained responsible for the technical decisions, implementation and final validation of the project.

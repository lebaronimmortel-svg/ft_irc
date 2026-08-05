*This project has been created as part of the 42 curriculum by alexfuen and tseche.*

# ft_irc

## Description
**ft_irc** is a C++ 98 implementation of an IRC (Internet Relay Chat) server. The goal of this project is to create a fully functional text-based communication server capable of handling multiple clients simultaneously via non-blocking TCP/IP sockets and I/O multiplexing (`poll()` or equivalent).

Clients can connect using a standard IRC client (e.g., HexChat, Irssi, LimeChat), authenticate with a password, set their nickname/username, create or join channels, send private messages, and execute operator commands.

### Key Features
- **Non-blocking I/O Architecture:** Single-threaded socket handling using `poll()` (or `select`/`epoll`/`kqueue`) without blocking or excessive CPU usage.
- **Client Authentication & Registration:** Handled via standard `PASS`, `NICK`, and `USER` commands.
- **Channel Operations:**
  - Public and private messaging (`PRIVMSG`).
  - Joining (`JOIN`) and leaving (`PART`/`QUIT`) channels.
  - Automatic message broadcasting to channel members.
- **Channel Operator Commands:**
  - `KICK` — Eject a client from a channel.
  - `INVITE` — Invite a user to an invite-only channel.
  - `TOPIC` — View or change the channel topic.
  - `MODE` — Manage channel modes:
    - `i`: Set/remove Invite-only mode.
    - `t`: Set/remove Topic protection (operators only).
    - `k`: Set/remove Channel key (password).
    - `o`: Give/take Channel operator status.
    - `l`: Set/remove User limit for the channel.
- **Robust Buffer Handling:** Command aggregation to handle partial data packets and network lag (e.g., handling commands fragmented across multiple `recv()` calls).

---

## Instructions

### Requirements
- A C++ compiler with C++98 support (`g++` or `clang++`).
- `make` utility.
- An IRC client for testing (e.g., **HexChat**, **Irssi**, **LimeChat**, or `nc` / `netcat`).

### Compilation
To compile the project, run:
```bash
make
```

This generates the executable ircserv. Other standard Makefile rules include:

    make clean — Removes object files.
    make fclean — Removes object files and the ircserv executable.
    make re — Recompiles the entire project.

### Running the Server

Run the executable with a listening port and a connection password:

    ./ircserv <port> <password>

Example : 

    ./ircserv 6667 mysecretpassword

### Testing with nc (Netcat)

You can test raw TCP packet buffering using nc:

    nc -C 127.0.0.1 6667

# Resources

### IRC Standards

- **RFC 1459 — Internet Relay Chat Protocol**

    https://www.rfc-editor.org/info/rfc1459/

- **RFC 2812 — Internet Relay Chat: Client Protocol**

    https://www.rfc-editor.org/info/rfc2812/

### Artificial Intelligence Usage

In accordance with the 42 AI guidelines for this project, AI tools were utilized as follows:

- **Documentation:** Used AI to find some specific information about a function, tool, concept.

- **Architecture & Design:** Discussed non-blocking socket patterns, event loop design, and buffering strategy for fragmented TCP packets.

- **Edge Case Identification:** Prompted AI to suggest potential edge cases regarding signal handling, socket disconnects, and unexpected client input formatting.


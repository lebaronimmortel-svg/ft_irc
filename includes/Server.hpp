#pragma once

#include "Header.hpp"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_EVENT 10
#define TIMEOUT 180000

#if TIMEOUT == 0
	#define TIMEOUT 180000
#elif TIMEOUT < 0
	#if TIMEOUT != -1
 		#define TIMEOUT -1
	#endif
#endif

typedef int (* cmdfunc)(std::string &str, Client &);

int invite(std::string &str, Client &);
int kick(std::string &str, Client &);
int topic(std::string &str, Client &);
int mode(std::string &str, Client &);
int pass(std::string &str, Client &);
int nick(std::string &str, Client &);
int name(std::string &str, Client &);
int join(std::string &str, Client &);
int privmsg(std::string &str, Client &);
int unknowncmd(std::string &str, Client &);

enum cmdlistenum {
    INVITE,
    KICK,
    TOPIC,
    MODE,
    PASS,
    NICK,
    NAME,
    JOIN,
    PRIVMSG,
    UNKNOWN
};

struct cmdlist {
    char *name;
    cmdfunc call;
};

cmdlist cmdLU[] = {
    [INVITE] = {
        .name = "invite",
        .call = invite,
    },
    [KICK] = {
        .name = "kick",
        .call = kick,
    },
    [TOPIC] = {
        .name = "topic",
        .call = topic,
    },
    [MODE] = {
        .name = "mode",
        .call = mode,
    },
    [PASS] = {
        .name = "pass",
        .call = pass,
    },
    [NICK] = {
        .name = "nick",
        .call = nick,
    },
    [NAME] = {
        .name = "name",
        .call = name,
    },
    [JOIN] = {
        .name = "join",
        .call = join,
    },
    [PRIVMSG] = {
        .name = "privmsg",
        .call = privmsg,
    },
    [UNKNOWN] = {
        .name = "",
        .call = unknowncmd,
    }
};

class Server
{
    private :

        // channels
        std::map<std::string, Channel*> _channels;
        std::map<int, Client *> _clientlist;

        // password for the channel
        std::string _password;
        int _servsock;
        int _epollfd;
        sockaddr_in _servaddr;


    public :

        Server(int port, std::string pass);
        ~Server();

        // getters
        inline const Channel* getChannel() const;
        inline const std::string getPassword() const;
        inline const int getEpollFd() const;
        inline const int getSocket() const;
        inline const sockaddr_in getAddress() const;

        // setters
        void    setPassword(std::string password);
        cmdfunc   getcmd(std::string str);
};

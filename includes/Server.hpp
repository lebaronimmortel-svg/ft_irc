#pragma once


#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include "Reply.hpp"
#include "Header.hpp"
#include "Channel.hpp"

#define MAX_EVENT 10
#define TIMEOUT 180000

#if TIMEOUT == 0
	#define TIMEOUT 180000
#elif TIMEOUT < 0
	#if TIMEOUT != -1
 		#define TIMEOUT -1
	#endif
#endif

struct mode_flag{
	short i;
	short t;
	short k;
	short o;
	short l;
};

struct mode_value{
	std::string k;
	size_t l;
	std::vector<std::string> o;
};

struct mode_s{
	mode_value value;
	mode_flag flag;
};

class Server;

typedef void (Server::*cmdfunc)(std::string &str, size_t &i,Client &);

class Server
{
    private :

        // channels
        std::map<std::string, Channel*> _channels;
        std::map<std::string, Client*> _clients;
        
        // password for the channel
        std::string _password;
        int _servsock;
        int _epollfd;
        sockaddr_in _servaddr;

        std::vector<std::string> &getArgsparse(std::string &str, char sep, size_t &i);
        Channel *getChannelparse(std::string &str, size_t&i);
        std::vector<Channel *> *getChannelListparse(Client *c, std::string &str, size_t &i);
        mode_s *getflagmode(Channel *, Client *c, std::string str);

    public :

        Server(int port, std::string pass);
        ~Server();

        // getters
        std::map<std::string, Channel *> &getChannelList();
        inline const std::string getPassword() const;
        int getEpollFd();
        int getSocket();
        sockaddr_in getAddress();

        // setters
        void    setPassword(std::string password);
        Client	*find_client(int fd);

        // utils
        void	addClient(int client_fd, std::string nick, std::string user, std::string full);
        void	addChannel(Channel *channel);
        Client	*get_client(std::string username, int fd, int mode);
        cmdfunc	getcmd(std::string str);
        int		callcmd(std::string str, Client &);
        void    HandleClient(Client *c);

        //reply
        void reply(Client *c, reply_flag flag, std::string msg);
        void replyChannel(Channel *c, std::string msg);

        //command
        void invite(std::string &str, size_t &i, Client &);
        void kick(std::string &str, size_t &i, Client &);
        void topic(std::string &str, size_t &i, Client &);
        void mode(std::string &str, size_t &i, Client &);
        void pass(std::string &str, size_t &i, Client &);
        void nick(std::string &str, size_t &i, Client &);
        void user(std::string &str, size_t &i, Client &);
        void join(std::string &str, size_t &i, Client &);
        void privmsg(std::string &str, size_t &i, Client &);
};

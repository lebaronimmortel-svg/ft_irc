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

#define MAX_EVENT 10
#define TIMEOUT 180000

#if TIMEOUT == 0
	#define TIMEOUT 180000
#elif TIMEOUT < 0
	#if TIMEOUT != -1
 		#define TIMEOUT -1
	#endif
#endif

class Server;

typedef void (Server::*cmdfunc)(std::string &str, int &i,Client &);

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

        std::vector<std::string> &getArgsparse(std::string &str, char sep, int &i);
        Channel *getChannelparse(std::string &str, int &i);
        std::vector<Channel *> *getChannelListparse(Client *c, std::string &str, int &i);
        mode_t *getflagmode(Channel *, Client *c, std::string str);

    public :

        Server(int port, std::string pass);
        ~Server();

        // getters
        inline std::map<std::string, Channel *> &getChannelList();
        inline const std::string getPassword() const;
        inline  int getEpollFd() const;
        inline  int getSocket() const;
        inline const sockaddr_in getAddress() const;

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
        void invite(std::string &str, int &i, Client &);
        void kick(std::string &str, int &i, Client &);
        void topic(std::string &str, int &i, Client &);
        void mode(std::string &str, int &i, Client &);
        void pass(std::string &str, int &i, Client &);
        void nick(std::string &str, int &i, Client &);
        void user(std::string &str, int &i, Client &);
        void join(std::string &str, int &i, Client &);
        void privmsg(std::string &str, int &i, Client &);
        Client *get_client(std::string username, int fd, int mode);
        Channel *get_channel(std::string name);

        // setters
        void    setPassword(std::string password);

        Client *find_client(int fd);
        void addClient(int client_fd, std::string nick, std::string user, std::string full);
        void addChannel(Channel *channel, std::string name);

        // titouan
        cmdfunc   getcmd(std::string str);
        int        callcmd(std::string str, Client &);
};

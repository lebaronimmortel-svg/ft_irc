#pragma once

#include "Header.hpp"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Command.hpp"

#define MAX_EVENT 10
#define TIMEOUT 180000

#if TIMEOUT == 0
	#define TIMEOUT 180000
#elif TIMEOUT < 0
	#if TIMEOUT != -1
 		#define TIMEOUT -1
	#endif
#endif



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


    public :

        Server(int port, std::string pass);
        ~Server();

        // getters
        inline const Channel &getChannel() const;
        inline const Channel &getChannel(std::string str) const;
        inline const std::string getPassword() const;
        inline const int getEpollFd() const;
        inline const int getSocket() const;
        inline const sockaddr_in getAddress() const;

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define RESET   "\033[0m"      
#define GREEN   "\033[32m"          
#define BLUE    "\033[34m"          
#define BOLD    "\033[1m"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include "Reply.hpp"
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

struct mode_flag
{
	short i;
	short t;
	short k;
	short o;
	short l;
};

struct mode_value
{
	std::string k;
	size_t l;
	std::vector<std::string> o;
};

struct mode_s
{
	mode_value value;
	mode_flag flag;
};

class Server;
typedef void (Server::*cmdfunc)(std::string &str, size_t &i,Client &);

class Server
{
    private :

        // stores objects
        std::map<std::string, Channel*> _channels;
        std::map<int, Client*>      _clients;
        
        // server password
        std::string                 _password;

        // TCP
        int                         _servsock;
        int                         _epollfdserv;
        sockaddr_in                 _servaddr;
        std::vector<int>            _epollfd;

        // Parser
        std::vector<std::string>    getArgsparse(std::string str, char sep);
        Channel*                    getChannelparse(std::string &str, size_t i);
        std::vector<Channel *>*     getChannelListparse(Client *c, std::string &str, size_t &i);

        // Mode
        mode_s*                     getFlagMode(Channel *, Client *c, const std::string& str);

    public :

        // Constructor
        Server(int port, std::string pass);

        // Destructor
        ~Server();

        // getters
        inline const std::string getPassword() const;
        int                      getEpollFd();
        int                      getSocket();
        std::vector<int>&        getfdList();
        sockaddr_in              getAddress();
        Client*                  getClient(std::string username, int fd, int mode);
        cmdfunc	                 getcmd(std::string str);
        Channel*                 getChannel(std::string name);
        std::map<std::string, Channel *> &getChannelList();

        // setters
        void    setPassword(std::string password);
        Client* find_client(int fd);

        // utils
        void	addClient(int client_fd);
        void    HandleClient(Client *c);
        void    addChannel(Channel *channel, std::string name);
        void    addChannelName(Channel *chan);
        int	    kickParser(std::string names, std::string reason, std::string channel, Client& c, size_t& i, std::string& str, size_t cpy);
        void    removeClient(int fd);
        int		callcmd(std::string str, Client &);
        void    clean();

        //reply
        void    reply(Client *c, reply_flag flag, std::string msg);
        void    replyChannel(Channel *c, std::string msg);

        //command
        void    invite(std::string &str, size_t &i, Client &);
        void    kick(std::string &str, size_t &i, Client &);
        void    topic(std::string &str, size_t &i, Client &);
        void    mode(std::string &str, size_t &i, Client &);
        void    pass(std::string &str, size_t &i, Client &);
        void    nick(std::string &str, size_t &i, Client &);
        void    user(std::string &str, size_t &i, Client &);
        void    join(std::string &str, size_t &i, Client &);
        void    privmsg(std::string &str, size_t &i, Client &);
};

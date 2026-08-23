/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:05:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:07:51 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <cstring>

Server::Server(int port, std::string pass): _password(pass){
	this->_servsock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_servsock < 0)
		throw std::runtime_error("Error Initialization socket");
	this->_servaddr = (struct sockaddr_in){
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if (bind(this->_servsock, (struct sockaddr *)&this->_servaddr, sizeof(this->_servaddr)) == -1){
		close(this->_servsock);
		throw std::runtime_error("Error binding server socket");
	}
	if (listen(this->_servsock, SOMAXCONN) == -1){
		close(this->_servsock);
		throw std::runtime_error("Error setting up listen limit");
	}
	this->_epollfd = epoll_create1(O_CLOEXEC);
	if (this->_epollfd < 0){
		close(this->_epollfd);
		throw std::runtime_error("Error Initialization Epoll");
	}
	struct epoll_event serv_event {
		.events = EPOLLIN,
		.data.fd = STDIN_FILENO,
	};
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_servsock, &serv_event) < 0){
		close(this->_servsock);
		close(this->_epollfd);
		throw std::runtime_error("Error adding event to listen queue");
	}
}

inline const int Server::getEpollFd() const {return this->_epollfd;};
inline const int Server::getSocket() const {return this->_servsock;};
inline const sockaddr_in Server::getAddress() const {return this->_servaddr;};

Client* Server::get_client(std::string username, int fd, int mode)
{
	for (std::map<std::string, Client*>::iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if (mode == 0 && i->first == username)
			return (i->second);
		if (mode == 1 && i->second->getFd() == fd)
			return (i->second);
	}

	return (NULL);
}

void Server::addClient(int client_fd, std::string nick, std::string user, std::string full)
{
	Client *client = new Client(nick, user, full, true, client_fd);
	_clients.insert(std::make_pair(user ,client));
}

inline std::map<std::string, Channel *> &Server::getChannelList()
{
	return this->_channels;
}

void Server::addChannel(Channel *chan)
{
	this->_channels[chan->getName()] = chan;
}

cmdfunc Server::getcmd(std::string str){
	size_t lenght = str.length();
	if (lenght == 0)
		return (NULL);
	int i = 0;
	for (; i < lenght && isblank(str[i]); i++);
	if (i == lenght)
		return (NULL);
	int slash = i;
	int sep = str.find(slash, ' ');
	if (sep == str.npos)
		sep = str.length();
	std::string cmd = str.substr(slash, sep);
	for (int i = 0; i < PRIVMSG + 1; i++){
		if (cmd ==  cmdLU[i].name)
			return cmdLU[i].call;
	}
	return (NULL);
}

int Server::callcmd(std::string str, Client &c){
	cmdfunc func = this->getcmd(str);
	if (!func)
	{
		std::cerr << "Server: couldn't find command:" + str + "\n" << std::flush << std::flush;
	}
	int i = str.find(' ');
	if (i == str.npos);
		i = str.length();
	func(str, i, c);
}

std::vector<std::string> &Server::getArgsparse(std::string &str, char sep, int &i){
	std::vector<std::string> vec;
	int sep = str.find(sep);
	if (sep == str.npos)
		return (vec);
	size_t lenght = str.length();
	for (; i < lenght; i++){
		vec.push_back(str.substr(i, sep));
		i += sep;
		for (;strchr("\0\r\n :", str[i]) == NULL; i++)
		sep = str.find(sep, i);
	}
	return (vec);
}

Channel *Server::getChannelparse(std::string &str, int &i){
	size_t len_str = str.length();
	if (i == len_str){
		return (NULL);
	}
	if (!str[i + 1] != '#')
		return (NULL);
	size_t end = str.find(' ');
	if (end == str.npos)
		end == len_str;
	std::string sub(str.substr(i, end));
	std::map<std::string, Channel *> lst = this->getChannelList();
	i = end;
	return ((lst.find(sub) != lst.end()) ? ((*lst.find(sub)).second) : NULL);//yeepi
}
std::vector<Channel *> *Server::getChannelListparse(std::string &str, int &i, int *fail){
	std::vector<std::string> &args = this->getArgsparse(str, ',', i);
	int lenght = args.size();
	std::vector<Channel *> *chanvec = new std::vector<Channel *>();
	for (int y = 0; y < lenght; y++){
		if (args[y][0] != '#'){
			*fail = y;
			return (NULL);	
		}
		else{
			std::map<std::string, Channel *>::iterator find = this->_channels.find(args[y]);
			if (find != this->_channels.end())
				chanvec->push_back((*find).second);
			else {
				*fail = y;
				return (NULL);
			}
		}
	}
	return (chanvec);
}
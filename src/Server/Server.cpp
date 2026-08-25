/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:05:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 15:15:07 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include <cstring>


Server::Server(int port, std::string pass): _password(pass){
	this->_servsock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_servsock < 0)
		throw std::runtime_error("Error Initialization socket");
	this->_servaddr = (struct sockaddr_in){
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {
			.s_addr = INADDR_ANY,
		}
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
	struct epoll_event serv_event = {
		.events = EPOLLIN,
		.data = {
			.fd = STDIN_FILENO,
		}
	};
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_servsock, &serv_event) < 0){
		close(this->_servsock);
		close(this->_epollfd);
		throw std::runtime_error("Error adding event to listen queue");
	}
}

int Server::getEpollFd() {return this->_epollfd;};
int Server::getSocket() {return this->_servsock;};
sockaddr_in Server::getAddress() {return this->_servaddr;};

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

std::map<std::string, Channel *> &Server::getChannelList()
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
	size_t i = 0;
	for (; i < lenght && isblank(str[i]); i++);
	if (i == lenght)
		return (NULL);
	int slash = i;
	size_t sep = str.find(slash, ' ');
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
		this->reply(&c, ERR_UNKNOWNCOMMAND, "unknown command");
		return 0;
	}
	size_t i = str.find(' ');
	if (i == str.npos)
		i = str.length();
	(this->*func)(str, i, c);
	return 1;
}

std::vector<std::string> &Server::getArgsparse(std::string &str, char sep, size_t &i){
	std::vector<std::string> *vec = new std::vector<std::string>();
	size_t find = str.find(sep);
	if (find == str.npos)
		return (*vec);
	size_t lenght = str.length();
	for (; i < lenght; i++){
		vec->push_back(str.substr(i, find));
		i = find;
		for (;i < lenght && strchr("\0\r\n :", str[i]) == NULL; i++)
		find = str.find(sep, i);
	}
	return (*vec);
}

Channel *Server::getChannelparse(std::string &str, size_t &i){
	size_t len_str = str.length();
	if (i == len_str){
		return (NULL);
	}
	if (!(str[i + 1] != '#'))
		return (NULL);
	size_t end = str.find(' ');
	if (end == str.npos)
		end = len_str;
	std::string sub(str.substr(i, end));
	std::map<std::string, Channel *> lst = this->getChannelList();
	i = end;
	return ((lst.find(sub) != lst.end()) ? ((*lst.find(sub)).second) : NULL);//yeepi
}

std::vector<Channel *> *Server::getChannelListparse(Client *c, std::string &str, size_t &i){
	std::vector<std::string> &args = this->getArgsparse(str, ',', i);
	int lenght = args.size();
	std::vector<Channel *> *chanvec = new std::vector<Channel *>();
	for (int y = 0; y < lenght; y++){
		if (args[y][0] != '#'){
			this->reply(c, ERR_NOSUCHCHANNEL, "this channel doesn't exist");
		}
		else{
			std::map<std::string, Channel *>::iterator find = this->_channels.find(args[y]);
			if (find != this->_channels.end())
				chanvec->push_back((*find).second);
			else {
				chanvec->push_back(NULL);
			}
		}
	}
	return (chanvec);
}

void Server::reply(Client *c, reply_flag flag, std::string msg){
	std::string name = (c->getNickName().empty()) ? "*" : c->getNickName();
	std::string message = ":ircserv " + reply_flag_value[flag] + " " + name
	 + " " + msg + "\r\n";
	send (c->getFd(), message.c_str(), message.size(), 0);
}

void Server::replyChannel(Channel *chan, std::string msg){
	msg += "\r\n";
	std::map<std::string, Client *> mem = chan->getMembers();
	std::map<std::string, Client *>::iterator end = mem.end();
	for (std::map<std::string, Client *>::iterator it = mem.begin(); it != end; it++){
		send((*it).second->getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Server::HandleClient(Client *c){
	std::string &buff = c->getBuffer();
	size_t pos;
	while ((pos = buff.find("\r\n")) != buff.npos){
		std::string line = buff.substr(0, pos);
		buff.erase(0, pos + 2);
		this->callcmd(line, *c);
	}
}
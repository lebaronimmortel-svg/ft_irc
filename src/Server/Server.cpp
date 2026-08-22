/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:05:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 00:26:04 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

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

cmdfunc Server::getcmd(std::string str){
	int slash = str.find('/');
	if (!slash)
		return NULL;
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
		// return error
	}
	int i = str.find(' ');
	if (str[i + 1] != '#')
	{
		// return error
	}
	int end = str.find(' ', i + 1);
	if (end == str.npos)
		end = str.length() - i;
	std::string name = str.substr(i, end);
	const Channel &chan = this->getChannel(name);
	if (!&chan)
	{
		// return error
	}
	std::string sub = &str[end];
	func(sub, c, const_cast<Channel &>(chan));
}
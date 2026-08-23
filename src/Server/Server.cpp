/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:05:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/19 23:55:11 by tseche           ###   ########.fr       */
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

Client* Server::has_client(std::string username, int fd, int mode)
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
	Client *client = new Client();
	client->setFd(client_fd);
	client->setNickName(nick);
	client->setUserName(user);
	client->setFullName(full);
	client->setAuthenticated(1);
	client->setBuffer("");
	_clients.insert(std::make_pair(user ,client));


	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:01 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"

// parser
void print_client_quit(std::string user, std::string nick, int fd);
void print_new_connection(int client_fd);

/*
    new_client

        This function is meant to 
        handle a new client
        arrival
*/
int	new_client(Server *serv, int server_socket)
{
	int client_fd = accept(server_socket, NULL, NULL);
	if (client_fd == -1)
		return (1);
	print_new_connection(client_fd);
	
	struct epoll_event client_event = 
	{
		.events = EPOLLIN | EPOLLET,
		.data = 
		{
			.fd = client_fd,
		}
	};

	if (epoll_ctl(serv->getEpollFd(), EPOLL_CTL_ADD, client_fd, &client_event) < 0)
	{
		close(client_fd);
		throw std::runtime_error("Error adding client to the poll pool");
	}
	serv->getfdList().push_back(client_fd);
	serv->addClient(client_fd);
	return (0);
}

/*
    client_close

        This function is meant
        to handle a client 
        asking for leaving
*/
void client_close(Server *serv, Client *client, int fd)
{
	std::string user = client->getUserName();
	std::string nick = client->getNickName();	
	epoll_ctl(serv->getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	serv->removeClient(fd);
	client->leaveAllChannels();
	print_client_quit(user, nick, fd);
	serv->clean();
}

/*
    socket_close

        This function is meant
        to handle the case where
        a client loses 
        connection
*/
void socket_close(Server *serv, int fd)
{
	Client *client = serv->getClient("", fd, 1);
	std::string user = client->getUserName();
	std::string nick = client->getNickName();
	epoll_ctl(serv->getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	client->leaveAllChannels();
	print_client_quit(user, nick, fd);
	serv->removeClient(fd);
	serv->clean();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

#include <iostream>

// print
void print_header();

// events
int	new_client(Server *serv, int server_socket);
void socket_close(Server *serv, int fd);
void client_close(Server *serv, Client *client, int fd);

/*
	main

		This function is meant to
		execute an infinite loop,
		waiting for events to occur
		on the server, then call
		the appropriate function
*/
int main(int argc, char** argv)
{
	(void) argv;
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	size_t port;
	std::istringstream oss(argv[1]);
	oss >> port;
	if (oss.fail() || !oss.eof())
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	print_header();

	try
	{
		Server serv(port, argv[2]);
		int server_socket = serv.getSocket();
		struct epoll_event events[MAX_EVENT];
		
		while (1)
		{
			/*
				waiting for new data
				to be sent to server
			*/
			int ready = epoll_wait(serv.getEpollFd(), events, MAX_EVENT, TIMEOUT);
			if (ready == -1)
				break;

			for (int i = 0; i < ready; i++)
			{
				int fd = events[i].data.fd;

				/*
					case 1 :

						new socket has
						connected to the 
						server
				*/
				if (fd == server_socket)
				{
					if (new_client(&serv, server_socket))
						continue ;
				}

				/*
					case 2 :

						some data has been send
						on this file descriptor
				*/
				else if (events[i].events & EPOLLIN)
				{
					char buff[4097];
					Client *client = serv.getClient("", fd, 1);
					ssize_t bytes_read = recv(fd, buff, sizeof(buff), 0);

					/*
						case 1 :

							client has closed
							transmission
					*/
					if (bytes_read == 0)
					{
						client_close(&serv, client, fd);
						continue ;
					}

					/*
						case 2 :

							client sent
							some text
					*/
					else if (bytes_read > 0)
					{
						if (bytes_read < 4097)
							buff[bytes_read] = '\0';
						else
							buff[4096] = '\0';
						client->getBuffer().append(buff);
						serv.HandleClient(client);
					}
				}

				/*
					case 3 :

						client closes the socket
						before the end of the
						transmission
				*/
				else if (events[i].events & (EPOLLERR | EPOLLHUP))
					socket_close(&serv, fd);
			}
		}
	}
	
	catch (std::exception &e)
	{
		std::cerr << "Error :\n" << e.what() << std::endl;
		return (-1);
	}

	return (0);
}

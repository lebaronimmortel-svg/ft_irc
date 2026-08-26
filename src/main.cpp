#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include <iostream>

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
	if (oss.fail() || !oss.eof()){
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	Server serv(port, argv[2]);
	int server_socket = serv.getSocket();

	struct epoll_event events[MAX_EVENT];
	while (1)
	{
		int ready = epoll_wait(serv.getEpollFd(), events, MAX_EVENT, TIMEOUT);
		if (ready == -1)
			break;

		for (int i = 0; i < ready; i++)
		{
			int fd = events[i].data.fd;

			if (fd == server_socket) // new client
			{
				
			} 
			else if (events[i].events & EPOLLIN)
			{
				int client_fd = accept(server_socket, NULL, NULL);
				if (client_fd == -1)
					continue;

				struct epoll_event client_event = 
				{
					.events = EPOLLIN | EPOLLET,
					.data = {
						.fd = client_fd,
					}
				};

				if (epoll_ctl(serv.getEpollFd(), EPOLL_CTL_ADD, client_fd, &client_event) < 0)
				{
					close(client_fd);
					throw std::runtime_error("Error adding client to the poll pool"); // voir si throw ou pas
				}
				serv.getfdlist().push_back(client_fd);
				// // read socket client -> events[i].data.fd
				// // with \r\n
				// char buff[4096];
				// Client *client = serv.get_client("", events[i].data.fd, 1);
				// ssize_t bytes_read = recv(events[i].data.fd, buff, sizeof(buff), 0);
				// if (bytes_read < 0)
				// 	return 1;
				// client->getBuffer().append(buff);
				// serv.HandleClient(client);
			}
			else if (events[i].events & (EPOLLERR | EPOLLHUP)) // client close the socket before the end of the transmission
			{
				close(fd);
				epoll_ctl(serv.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
			}
		}
	}

	return (0);
}

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include <iostream>

std::string cmd_sfx(std::string str)
{
	unsigned long i = 0;
	unsigned long len = str.size();
	std::string res = "";
	while (i < len && str[i] != ' ')
		i++;
	if (i == len)
		return res;
	i++;
	while (i < len && str[i] != ' ')
			res += str[i++];
	return res;
}

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
				std::cout << "New client has connected !" << std::endl;
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
				serv.addClient(client_fd);

			} 
			else if (events[i].events & EPOLLIN)
			{
				char buff[4097];
				Client *client = serv.get_client("", fd, 1);
				ssize_t bytes_read = recv(fd, buff, sizeof(buff), 0);
				if (bytes_read == 0)
				{
					
					epoll_ctl(serv.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
					close(fd);
					serv.removeClient(fd);
					continue ;
				}
				else if (bytes_read > 0)
				{
					if (bytes_read < 4097)
						buff[bytes_read] = '\0';
					else
						buff[4097] = '\0';
					client->getBuffer().append(buff);
					serv.HandleClient(client);
				}
			}
			else if (events[i].events & (EPOLLERR | EPOLLHUP)) // client close the socket before the end of the transmission
			{
				
				epoll_ctl(serv.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
				close(fd);
				serv.removeClient(fd);
			}
		}
	}
	return (0);
}

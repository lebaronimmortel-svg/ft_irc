#include "../includes/Server.hpp"

int main(int argc, char** argv)
{
	(void) argv;

	if (argc != 3 || !parse_arg(argv[1]))
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (-1);
	}

	Server serv(atoi(argv[1]), argv[2]);
	int server_socket = serv.getSocket();
	sockaddr_in server_adrres = serv.getAddress();

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
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);

				int client_fd = accept4(server_socket, (sockaddr *)&server_adrres, &client_len, SOCK_NONBLOCK);
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
			} 

			else if (events[i].events & EPOLLIN)
			{
				// read socket client -> events[i].data.fd
				// with \r\n
				char buff[4096];
				Client *client = serv.get_client("", events[i].data.fd, 1);
				ssize_t bytes_read = recv(events[i].data.fd, buff, sizeof(buff), 0);
				if (bytes_read)
					return 1;
				client->getBuffer().append(buff);
				serv.HandleClient(client);
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

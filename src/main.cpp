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
			char *buf = NULL;
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
					.data.fd = client_fd,
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
				ssize_t bytes_read = recv(events[i].data.fd, buf, sizeof(buf), 0);
				Client *client = serv.get_client("", events[i].data.fd, 1);
				if (!client)
				{
					std::string nick = parse_auth(buf, 0);
					std::string user = parse_auth(buf, 1);
					std::string full = parse_auth(buf, 2);
					if (!user.empty() && !serv.get_client(user, 0, 0))
						serv.addClient(events[i].data.fd, nick, user, full);
					else
					{
						std::cerr << "Error adding user" << user << " to the server: ";
						if (user.empty())
							std::cerr << "Wrong authentification syntax";
						else if (serv.get_client(user, 0, 0))
							std::cerr << "Username already in use";
						std::cerr << std::endl;
					}
				}
				else // traiter la commande
				{
					// cmd channel
					std::string cmd = chan_join_cmd(buf);
					if (!cmd.empty())
					{
						Channel *chan = serv.get_channel(cmd);
						if (!chan)
						{
							chan = new Channel(cmd);
							serv.addChannel(chan, cmd);
						}
						chan->addUser(client);
						client->addChannel(chan);
					}

					if (client->is_chan_member())
					{

						if (parse_cmd(buf) == "PRV_MSG")
						{}

						else if ()
						{}


						else if ()
						{}

					}
				}
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

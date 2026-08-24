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
					if (!ft_strncmp(buf, "JOIN ", 5))
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
							if (chan->getInviteOnlyStatus() == 0 || (chan->getInviteOnlyStatus() == 1 && client->is_invited(chan)))
							{
								chan->addUser(client);
								client->addChannel(chan);
								if (client->is_invited(chan))
									client->delInvChan(chan);
							}
						}
						else
						{
							// Unknown command
						}
					}
					else if (!client->is_chan_member())
					{
						std::cerr << "Error executing command: " << client->getUserName() << " belongs to no channel" << std::endl;
						continue ;
					}
					else if (!ft_strncmp(buf, "NICK ", 5))
					{
						std::string nick = nick_cmd(buf);
						if (!nick.empty())
							client->setNickName(nick);
						else
						{
							// Unknown command
						}
					}
					else if (!ft_strncmp(buf, "TOPIC #", 7))
					{
						std::string chan = topic_cmd(buf, 0);
						std::string topic = topic_cmd(buf, 1);
						if (!chan.empty())
						{
							Channel *channel  = serv.get_channel(chan);

							if (!topic.empty())
							{
								if (channel)
								{
									channel->setTopic(topic);
									channel->setTopicLastModifierUsername(client->getUserName());
									// channel->setTopicLastModifDate( DATE );
								}
								else
									std::cerr << "Error modifying " << chan << "'s topic: no such channel" << std::endl;
							}
							else
							{
								// envoyer message au client : channel->getTopic()
							}
						}
						else
						{
							// Unknown command
						}
					}
					else if (!ft_strncmp(buf, "NAMES", 5))
					{
						if (!ft_strncmp(buf, "NAMES #", 7))
						{
							std::string chan = names_cmd(buf, 1);
							if (!chan.empty())
							{
								Channel *channel  = serv.get_channel(chan);
								if (channel)
								{
									// envoyer message au client : noms des membres de channel
								}
								else
									std::cerr << "Error displaying channel " << chan << "'s members names: no such channel" << std::endl;
							}
							else
							{
								// Unknown command
							}
						}
						else
						{
							std::string result = names_cmd(buf, 0);
							if (!result.empty())
							{
								// envoyer message au client : noms des membres de serv
							}
							else
							{
								// Unknown command
							}
						}
					}

					else if (!ft_strncmp("PRIVMSG ", buf, 8))
					{
						if (!ft_strncmp("PRIVMSG #", buf, 9))
						{
							std::string chan = prv_chan_cmd(buf, 0);
							std::string msg =  prv_chan_cmd(buf, 1);

							if (!chan.empty())
							{
								Channel *channel  = serv.get_channel(chan);
								if (channel)
								{
									if (channel->getMember(client->getUserName()))
									{
										// envoyer msg 'msg' aux membres du canal 'channel' de la part de 'client->getUserName()'
									}
									else
										std::cerr << "Error sending private message to " << chan << "'s members: " << client->getUserName() << "isn't a member of the channel" << std::endl;
								}
								else
									std::cerr << "Error sending private message to " << chan << "'s members: no such channel" << std::endl;
							}

							else
							{
								// unknown command
							} 
						}
						else
						{
							std::string user = prv_clt_cmd(buf, 0);
							std::string msg = prv_clt_cmd(buf, 1);

							if (!user.empty())
							{
								// envoyer msg 'msg' au client 'user' de la part de 'client->getUserName()'
							}
							else
							{
								// Unknown command
							}
						}
					}
					else if (!ft_strncmp("KICK ", buf, 8))
					{
						std::string chan = kick_cmd(buf, 0);
						std::string user = kick_cmd(buf, 1);
						std::string reason = kick_cmd(buf, 2);

						if (!chan.empty())
						{
							Channel* channel = serv.get_channel(user);
							if (!channel)
								std::cerr << "Error while kicking " << user << " from channel " << chan << ": no such channel" << std::endl;
							else 
							{
								Client *clt = channel->getMember(user);
								if (!clt)
									std::cerr << "Error while kicking " << user << " from channel " << chan << ": no such user" << std::endl;
								else
								{
									channel->delUser(clt);
									clt->delChannel(channel);
								}
							}
							if (!reason.empty())
							{
								// envoyer msg 'reason' à user 
							}
						}
						else
						{
							// Unknown command
						}
					}
					else if (!ft_strncmp("INVITE ", buf, 7))
					{
						std::string chan = inv_cmd(buf, 0);
						std::string user = inv_cmd(buf, 1);

						if (!chan.empty())
						{
							Channel* channel = serv.get_channel(user);
							if (!channel)
								std::cerr << "Error while inviting " << user << " to channel " << chan << ": no such channel" << std::endl;
							else 
							{
								Client *clt = serv.get_client(user, 0, 0);
								if (!clt)
									std::cerr << "Error while inviting " << user << " to channel " << chan << ": no such user" << std::endl;
								else
									clt->addChannelInv(channel);
							}
						}
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

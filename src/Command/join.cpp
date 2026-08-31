/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:27 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

std::string cmd_sfx_ref(std::string& str);
void print_new_channel(Client& c, Channel* nchan);
void print_join_channel(Client& c, Channel* chan);

void Server::join(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	std::vector<Channel *> *channels = this->getChannelListparse(&c, str, i);
	if (channels == NULL){
		return ;
	}

	std::vector<std::string> args = this->getArgsparse(str, ' ');


	int lenght = channels->size();
	int lenght_args = args.size();

	
	for (int i = 0; i < lenght; i++)
	{
		Channel *chan = channels->operator[](i);

		if (!chan)
		{
			std::string name = this->getArgsparse(cmd_sfx_ref(str), ',').at(i);
			if (name.empty() || name[0] != '#')
				continue ;
			Channel *nchan = new Channel(name);
			this->addChannel(nchan);
			nchan->addUser(&c);
			nchan->addModerator(&c);
			print_new_channel(c, nchan);
			this->reply(&c, RPL_WELCOME, ": welcome on channel: " + name);
		}

		else if (chan->getMember(c.getNickName()) != NULL)
			this->reply(&c, ERR_USERONCHANNEL, chan->getName() +  ": already on channel");

		else
		{
			if (chan->getInviteOnlyStatus()){
				if (chan->getInvited(c.getNickName()) == NULL)
				{
					this->reply(&c, ERR_INVITEONLYCHAN, chan->getName() + ": cannot join channel (+i)");
					continue;
				}	
			}
			if (chan->getUserLimit() != 0 && chan->getMembers().size() >= chan->getUserLimit()){
				this->reply(&c, ERR_CHANNELISFULL, chan->getName() + ": cannot join channel (+l)");
				continue;
			}
			if (chan->getPasswordRequirement()){
				if (i < lenght_args){
					std::string key = args[i];
					if (chan->getPassword() != key){
						this->reply(&c, ERR_PASSWDMISMATCH, chan->getName() + ": cannot join channel (+k)");
						continue;
					}
				} else { 
					this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() + ": cannot join channel (+k)");
					continue;
				}
			}
			chan->addUser(&c);
			chan->delInvited(&c);
			print_join_channel(c, chan);
			this->reply(&c, RPL_WELCOME, ": welcome on channel " + chan->getName());
		}
	}
}
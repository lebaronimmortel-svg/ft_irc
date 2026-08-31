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

// parsing
std::string cmd_sfx_ref(std::string& str);

// print
void print_new_channel(Client& c, Channel* nchan);
void print_join_channel(Client& c, Channel* chan);

/*
	join

		This function is meant to
		execute the JOIN command from
		an IRC client:

		JOIN #channel1,#channel2,#channel3
*/
void Server::join(std::string &str, size_t &i, Client &c)
{
	/*
		Lexing provided input
	*/
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

	/*
		Browsing channels 
		list names
	*/
	for (int i = 0; i < lenght; i++)
	{
		Channel *chan = channels->operator[](i);

		/*
			case 1 : submitted channel
			doesn't already exist
		*/
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
			nchan->broadcast(c.getNickName() + " has joined channel " + name + "\r\n", c.getFd());
			this->reply(&c, RPL_WELCOME, ": welcome on channel: " + name);
		}

		/*
			case 2 : submitted channel
			already exists and client
			is already member
		*/
		else if (chan->getMember(c.getNickName()) != NULL)
			this->reply(&c, ERR_USERONCHANNEL, chan->getName() +  ": already on channel");

		/*
			case 3 : submitted channel
			already exists and client
			isn't member yet
		*/
		else
		{
			if (chan->getInviteOnlyStatus())
			{
				if (chan->getInvited(c.getNickName()) == NULL)
				{
					this->reply(&c, ERR_INVITEONLYCHAN, chan->getName() + ": cannot join channel (+i)");
					continue;
				}	
			}
			if (chan->getUserLimit() != 0 && chan->getMembers().size() >= chan->getUserLimit())
			{
				this->reply(&c, ERR_CHANNELISFULL, chan->getName() + ": cannot join channel (+l)");
				continue;
			}
			if (chan->getPasswordRequirement())
			{
				if (i < lenght_args)
				{
					std::string key = args[i];
					if (chan->getPassword() != key)
					{
						this->reply(&c, ERR_PASSWDMISMATCH, chan->getName() + ": cannot join channel (+k)");
						continue;
					}
				} 
				else 
				{ 
					this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() + ": cannot join channel (+k)");
					continue;
				}
			}
			chan->addUser(&c);
			chan->delInvited(&c);
			print_join_channel(c, chan);
			chan->broadcast(c.getNickName() + " has joined channel " + chan->getName() + "\r\n", c.getFd());
			this->reply(&c, RPL_WELCOME, ": welcome on channel " + chan->getName());
		}
	}
}
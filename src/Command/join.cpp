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
std::string cmdSfxRef(std::string& str);

// print
void print_new_channel(Client& c, Channel* nchan);
void print_join_channel(Client& c, Channel* chan);

/*
	add_to_channel

		This function is meant to
		add a client to
		targeted channel
*/
void	Server::addToChan(Channel *chan, Client &c)
{
	chan->addUser(&c);
	chan->delInvited(&c);
	print_join_channel(c, chan);
	chan->broadcast(c.getNickName() + " has joined channel " + chan->getName() + "\r\n", c.getFd());
	this->reply(&c, RPL_WELCOME, ": welcome on channel " + chan->getName());	
}

/*
	new_channel

		This function is meant to
		create a new channel if
		the provided target
		doesn't exist already
*/
int Server::newChannel(int i, std::string str, Client &c)
{
	std::string name = this->getArgsparse(cmdSfxRef(str), ',').at(i);
	if (name.empty() || name[0] != '#')
		return 0;
	Channel *nchan = new Channel(name);
	this->addChannelName(nchan);
	nchan->addUser(&c);
	nchan->addModerator(&c);
	print_new_channel(c, nchan);
	nchan->broadcast(c.getNickName() + " has joined channel " + name + "\r\n", c.getFd());
	this->reply(&c, RPL_WELCOME, ": welcome on channel: " + name);
	return 1;
}

/*
	channel_check

		This function is meant to
		check if targeted channel
		fills conditions to be joined
*/
int Server::chanCheck(Channel *chan, Client &c, int i, std::vector<std::string> args, int lenght_args)
{
	if (chan->getInviteOnlyStatus())
	{
		if (chan->getInvited(c.getNickName()) == NULL)
		{
			this->reply(&c, ERR_INVITEONLYCHAN, chan->getName() + ": cannot join channel (+i)");
			return 0;
		}	
	}
	if (chan->getUserLimit() != 0 && chan->getMembers().size() >= chan->getUserLimit())
	{
		this->reply(&c, ERR_CHANNELISFULL, chan->getName() + ": cannot join channel (+l)");
		return 0;
	}
	if (chan->getPasswordRequirement())
	{
		if (i < lenght_args)
		{
			std::string key = args[i];
			if (chan->getPassword() != key)
			{
				this->reply(&c, ERR_PASSWDMISMATCH, chan->getName() + ": cannot join channel (+k)");
				return 0;
			}
		} 
		else 
		{ 
			this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() + ": cannot join channel (+k)");
			return 0;
		}
	}	
	return 1;
}

/*
	join

		This function is meant to
		execute the JOIN command from
		an IRC client:

		JOIN #channel1,#channel2,#channel3
*/
void Server::join(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}

	/*
		Parsing provided input
	*/
	std::vector<std::string> args = this->getArgsparse(str, ' ');
	std::vector<Channel *> *channels = this->getChannelListparse(&c, str, i);
	if (channels == NULL)
		return ;

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
			if (!newChannel(i, str, c))
				continue ;
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
			if (chanCheck(chan, c, i, args, lenght_args))
				continue ;
			addToChan(chan, c);
		}
	}
}
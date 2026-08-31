/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:00:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:39:50 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::kick(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	
	size_t cpy = i;
	
	std::vector<std::string> arg = this->getArgsparse(str, ' ');

	if (arg.size() < 3)
		return ;

	std::string reason = "";

	if (arg.size() >= 4 && arg.at(3)[0] == ':')
	{
		for (size_t w = 3; w < arg.size(); w++)
			reason += arg.at(w);
	}

	std::vector<std::string> channels = this->getArgsparse(arg.at(1), ',');
	std::vector<std::string> users = this->getArgsparse(arg.at(2), ',');

	size_t lenght = users.size();
	size_t lenght_chan = channels.size();

	if (lenght > 1 && lenght_chan > 1)
	{
		if (lenght != lenght_chan)
		{
			this->reply(&c, ERR_NEEDMOREPARAMS, ": invalid parameters number");
			return ;
		}
		else
		{
			for (size_t j = 0; j < lenght; j++) // correspondance 1 a 1
			{
				std::string names = users.at(j);

				if (names.size() > 0 && names[0] == ':')
					break ;						

				Channel *chan = this->getChannelparse(channels[j], 0);
				if (chan == NULL)
				{
					this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
					continue ;
				}
				if (chan->getModerator(c.getNickName()) == NULL)
				{
					this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
					continue ;
				}

				Client *user = chan->getMember(names);
				if (user == NULL)
					this->reply(&c, ERR_USERNOTINCHANNEL, chan->getName() +  ": no such nickname in channel");
				else 
				{
					chan->delMember(user);
					if (chan->getModerator(user->getNickName()) != NULL)
						chan->delModerator(user);
					std::string msg = user->getNickName();
					msg += " has been kicked from channel " + chan->getName() + "\r\n";
					chan->broadcast(msg, user->getFd());
					std::string msg_prv = "You have been kicked from channel " + chan->getName();
					if (!reason.empty())
						msg_prv += ", reason: " + reason;
					msg_prv += "\r\n";
					send(user->getFd(), msg_prv.c_str(), msg_prv.size(), 0);
				}
			}
			this->clean();
			return ;
		}
	}

	else if (lenght_chan >= 1 && lenght == 1) // un user a supprimer de un ou plusieurs canaux
	{
		for (size_t j = 0; j < lenght_chan; j++)
		{
			std::string names = users.at(0);

			if (names.size() > 0 && names[0] == ':')
				break ;						

			Channel *chan = this->getChannelparse(channels[j], 0);
			if (chan == NULL)
			{
				this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
				continue ;
			}
			if (chan->getModerator(c.getNickName()) == NULL)
			{
				this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
				continue ;
			}

			Client *user = chan->getMember(names);
			if (user == NULL)
				this->reply(&c, ERR_USERNOTINCHANNEL, chan->getName() +  ": no such nickname in channel");
			else 
			{
				chan->delMember(user);
				if (chan->getModerator(user->getNickName()) != NULL)
					chan->delModerator(user);
				std::string msg = user->getNickName();
				msg += " has been kicked from channel " + chan->getName() + "\r\n";
				chan->broadcast(msg, user->getFd());
				std::string msg_prv = "You have been kicked from channel " + chan->getName();
				if (!reason.empty())
					msg_prv += ", reason: " + reason;
				msg_prv += "\r\n";
				send(user->getFd(), msg_prv.c_str(), msg_prv.size(), 0);
			}
		}
		this->clean();
		return ;
		
	}


	// un ou plusieurs users a supprimer d'un canal

	Channel *chan = this->getChannelparse(channels[0], 0);
	if (chan == NULL)
	{
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL)
	{
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
		return ;
	}

	for (size_t j = 0; j < lenght; j++)
	{
		std::string names = users.at(j);

		if (names.size() > 0 && names[0] == ':')
			break ;						

		Client *user = chan->getMember(names);

		if (user == NULL)
			this->reply(&c, ERR_USERNOTINCHANNEL, chan->getName() +  ": no such nickname in channel");
		else 
		{
			chan->delMember(user);
			if (chan->getModerator(user->getNickName()) != NULL)
				chan->delModerator(user);
			std::string msg = user->getNickName();
			msg += " has been kicked from channel " + chan->getName() + "\r\n";
			chan->broadcast(msg, user->getFd());
			std::string msg_prv = "You have been kicked from channel " + chan->getName();
			if (!reason.empty())
				msg_prv += ", reason: " + reason;
			msg_prv += "\r\n";
			send(user->getFd(), msg_prv.c_str(), msg_prv.size(), 0);
		}
	}
	this->clean();
}
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

/*
	kick user

		This function is meant
		to remove the user from
		the provided channel
*/
void	kick_user(Channel* chan, Client *user, std::string reason)
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

/*
	kick lexer

		This function is meant to
		deduce targeted channel 
		and client from the 
		parsed provided input,
		then execute the kick
*/
int	Server::kickParser(std::string names, std::string reason, std::string channel, Client& c, size_t& i, std::string& str, size_t cpy)
{
	if (names.size() > 0 && names[0] == ':')
		return (1) ;						
	Channel *chan = this->getChannelparse(channel, 0);
	if (chan == NULL)
	{
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
		return (0) ;
	}
	if (chan->getModerator(c.getNickName()) == NULL)
	{
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
		return (0) ;
	}
	Client *user = chan->getMember(names);
	if (user == NULL)
		this->reply(&c, ERR_USERNOTINCHANNEL, chan->getName() +  ": no such nickname in channel");
	else 
		kick_user(chan, user, reason);
	return (0);
}

/*
	kick

		This function is meant
		to execute the KICK
		command from an IRC
		operator:

		KICK #channel user1,user2,user3 :reason

		KICK #channel1,#channel2,#channel3 user :reason

		KICK #channel1,#channel2,#channel3 user1,user2,user3 :reason
*/
void Server::kick(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}

	size_t cpy = i;

	/*
		Parsing the provided input
		into strings lists
	*/
	std::vector<std::string> arg = this->getArgsparse(str, ' ');
	if (arg.size() < 3)
		return ;
	std::vector<std::string> channels = this->getArgsparse(arg.at(1), ',');
	std::vector<std::string> users = this->getArgsparse(arg.at(2), ',');
	size_t lenght_user = users.size();
	size_t lenght_chan = channels.size();

	/*
		Parsing the reason
		of the kick (if provided)
	*/
	std::string reason = "";
	if (arg.size() >= 4 && arg.at(3)[0] == ':')
	{
		for (size_t j = 3; j < arg.size(); j++)
			reason += arg.at(j);
	}

	/*
		case 1:

			There are as much channels than 
			clients provided (> 1) : each client
			will be kicked from the channel
			corresponding to it's index 
	*/
	if (lenght_user > 1 && lenght_chan > 1)
	{
		if (lenght_user != lenght_chan)
		{
			this->reply(&c, ERR_NEEDMOREPARAMS, ": invalid parameters number");
			return ;
		}
		else
		{
			for (size_t j = 0; j < lenght_user; j++)
			{
				if (this->kickParser(users.at(j), reason, channels[j], c, i, str, cpy))
					break ;
			}
			this->clean();
		}
	}

	/*
		case 2:

			There is one client
			to be kicked from one 
			or more channels
	*/
	else if (lenght_chan >= 1 && lenght_user == 1)
	{
		for (size_t j = 0; j < lenght_chan; j++)
		{
			if (this->kickParser(users.at(0), reason, channels[j], c, i, str, cpy))
				break ;
		}
		this->clean();
		return ;
	}

	/*
		case 3:

			There is one or more
			clients to be kicked
			from one channel
	*/
	else 
	{
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
		for (size_t j = 0; j < lenght_user; j++)
		{
			std::string names = users.at(j);
			if (names.size() > 0 && names[0] == ':')
				break ;						
			Client *user = chan->getMember(names);
			if (user == NULL)
				this->reply(&c, ERR_USERNOTINCHANNEL, chan->getName() +  ": no such nickname in channel");
			else 
				kick_user(chan, user, reason);
		}
		this->clean();
	}
}
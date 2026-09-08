/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/09/08 17:13:51 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"
#include <ctime>

/*
	set_topic

		This function is 
		meant to set provided
		topic to targeted 
		channel
*/
void setTopic(Channel *chan, Client& c, std::string topic)
{
	chan->setTopic(topic);
	chan->setTopicLastModifierUsername(c.getNickName());
	std::ostringstream oss;
	oss << std::time(NULL);
	chan->setTopicLastModifDate(oss.str());
	std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
						  "@localhost TOPIC " + chan->getName() + " :" + topic + "\r\n";
	chan->broadcast(msg, -1);
}

/*
	topic

		This function is meant to
		execute the TOPIC command
		from an IRC client:

		TOPIC #channel :message
*/
void Server::topic(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}

	/*
		Parsing 
		the input
	*/
	size_t cpy = i;
	Channel *chan = this->getChannelparse(str, i);

	if (chan == NULL)
	{
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
		return ;
	}

	std::vector<std::string> arg = this->getArgsparse(str.substr(cpy + chan->getName().length() + 1), ' ');
	if (arg.size() == 0)
	{
		if (chan->getTopic().empty())
			this->reply(&c, RPL_NOTOPIC, chan->getName() +  ": no topic set");
		else 
		{
			this->reply(&c, RPL_TOPIC, chan->getName() + ": " + chan->getTopic());
			this->reply(&c, RPL_TOPICWHOTIME, chan->getName() + " " + chan->getTopicLastModifierUsername() + " " + chan->getTopicLastModifDate());
		}
		return ;
	}

	/*
		Permissions check
	*/
	if (chan->getTopicRestrictionStatus())
	{
		if (chan->getModerator(c.getNickName()) == NULL)
		{
			this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": requires to be operator");
			return ;
		}
	}

	if (chan->getMember(c.getNickName()) == NULL)
	{
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": requires to be member");
		return ;
	}

	/*
		Retrieving
		topic message
	*/
	std::string topic;
	if (arg.size() == 1)
	{
		size_t i = 0;
		for (; i < arg[0].size();){
			if (isspace(arg[0][i]))
				i++;
			else
				break;
		}
		if (arg.at(0)[i] == ':')
		{
			topic = arg[0].substr(i + 1);
		}
		else 
		{
			this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() +  ": no topic provided");
			return ;	
		}
	}
	else
	{
		this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() +  ": invalid parameters");
		return ;		
	}

	/*
		Setting channel
		attributes and
		broadcasting
	*/
	setTopic(chan, c, topic);
}
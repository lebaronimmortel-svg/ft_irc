/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:10 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"
#include <ctime>

void Server::topic(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL)
	{
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
		return ;
	}
	
	std::vector<std::string> arg = this->getArgsparse(str, ' ');
	
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
	
	if (chan->getTopicRestrictionStatus())
	{
		if (chan->getModerator(c.getNickName()) == NULL)
		{
			this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
			return ;
		}
	}
	chan->setTopic(arg[0]);
	chan->setTopicLastModifierUsername(c.getNickName());
	std::ostringstream oss;
	oss << std::time(NULL);
	chan->setTopicLastModifDate(oss.str());

	std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
						  "@localhost TOPIC " + chan->getName() + " :" + arg[0] + "\r\n";
	this->replyChannel(chan, msg);
}
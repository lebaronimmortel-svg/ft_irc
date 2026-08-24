/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 23:17:36 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::topic(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "require registration");
		return;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, "this channel doesn't exist");
		return ;
	}
	if (chan->getTopicRestrictionStatus()){
		if (chan->getModerator(c.getNickName()) == NULL){
			this->reply(&c, ERR_CHANOPRIVSNEEDED, "require to be operator");
			return ;
		}
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	chan->setTopic(arg[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:51:25 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:12 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::user(std::string &str, size_t &i, Client &c){
	if (c.getAuthenticated()){
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: already register");
		return ;
	}
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, chan->getName() +  ": this channel doesn't exist");
		return ;
	}
	std::vector<std::string> args = this->getArgsparse(str, ' ', i);
	if (args.size() == 0){
		this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() +  ": require more parameter");
		return ;
	}
	c.setUserName(args.at(0));
	c.getAuthLevel() |= (1 << USERNAME);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 22:21:23 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::pass(std::string &str, int &i, Client &c){
	if (c.getAuthenticated()){
		this->reply(&c, ERR_ALREADYREGISTRED, "is already register");
		return ;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, "this channel doesn't exist");
		return ;
	}
	std::vector<std::string> args = this->getArgsparse(str, ' ', i);
	if (args.size() == 0){
		this->reply(&c, ERR_NEEDMOREPARAMS, "require a parameter");
		return ;
	}
	if (args.at(0) != chan->getPassword()){
		this->reply(&c, ERR_PASSWDMISMATCH, "incorect password");
		return ;
	};
	c.getAuthLevel() |= (1 << PASSWORD);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	} 
}
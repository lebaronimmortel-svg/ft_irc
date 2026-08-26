/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:39:59 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

bool nicknameValid(std::string &str){
	size_t length = str.length();
	if (length == 0 || length > 9)
		return false;
	std::string spec("[]\\`_^{|}");
	for (size_t i = 0; i < length; i++){
		if (i == 0){
			if (!isalpha(str[i]) && spec.find(str[i]) == spec.npos)
				return false;
		}
		else{
			if (!isalnum(str[i]) && spec.find(str[i]) == spec.npos && str[i] != '-')
				return false;
		}
	}
	return (true);
}

void Server::nick(std::string &str, size_t &i, Client &c){
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) + ": this channel doesn't exist");
		return ;
	}
	std::vector<std::string> args = this->getArgsparse(str, ' ', i);
	if (args.size() == 0){
		this->reply(&c, ERR_NONICKNAMEGIVEN, chan->getName() +  ": require a nickname");
		return ;
	}
	
	if (!nicknameValid(args.at(0))){
		this->reply(&c, ERR_ERRONEUSNICKNAME, chan->getName() +  ": erroneous nickname");
		return ;
	};
	if (this->get_client(args.at(0), 0, 0) != NULL){
		this->reply(&c, ERR_NICKNAMEINUSE, chan->getName() +  ": nickname already in use");
		return ;
	}
	c.setNickName(args.at(0));
	if (c.getAuthenticated())
		return ;
	c.getAuthLevel() |= (1 << NICKNAME);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	}
}
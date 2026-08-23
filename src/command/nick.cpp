/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:19:37 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

bool nicknameValid(std::string &str){
	size_t length = str.length();
	if (length == 0 || length > 9)
		return false;
	std::string spec("[]\\`_^{|}");
	for (int i = 0; i < length; i++){
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

void Server::nick(std::string &str, int &i, Client &c){
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		std::cerr << "Server: unknown channel:" + str.substr(cpy, i) + "\n" << std::flush;
		return ;
	}
	std::vector<std::string> args = this->getArgsparse(str, ' ', i);
	if (args.size() != 1){
		std::cerr << "Pass: require only one argument\n" << std::flush;
		return ;
	}
	
	if (!nicknameValid(args.at(0))){
		std::cerr << "Pass: incorrect nickname provided\n" << std::flush;
		return ;
	};
	c.setNickName(args.at(0));
	if (c.getAuthenticated())
		return ;
	c.getAuthLevel() |= (1 << NICKNAME);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:51:25 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:19:34 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::name(std::string &str, int &i, Client &c){
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
	c.setUserName(args.at(0));
	if (c.getAuthenticated())
		return ;
	c.getAuthLevel() |= (1 << USERNAME);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	}
}
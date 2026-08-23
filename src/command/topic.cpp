/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:20:12 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::topic(std::string &str, int &i, Client &c){
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		std::cerr << "Server: unknown channel:" + str.substr(cpy, i) + "\n" << std::flush;
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL){
		std::cerr << "TOPIC: user [" + c.getNickName() + "] is not an operator\n" << std::flush;
		return ;
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	if (arg.size() != 0){
		std::cerr << "TOPIC: didn't expected argument\n" << std::flush;
		return ;
	}
	chan->setTopicRestrictionStatus(!chan->getTopicRestrictionStatus());
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:00:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:19:28 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void Server::kick(std::string &str, int &i, Client &c){
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		std::cerr << "Server: unknown channel:" + str.substr(cpy, i) + "\n" << std::flush;
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL){
		std::cerr << "KICK: user [" + c.getNickName() + "] is not an operator\n" << std::flush;
		return ;
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	if (arg.size() == 0)
		return ;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan->getMember(arg.at(i));
		if (user == NULL){
			std::cerr << "KICK: user [" + arg.at(i) + "] is not recognized\n" << std::flush;
		}
		else {
			chan->delMember(user);
			if (chan->getModerator(user->getNickName()) != NULL)
				chan->delModerator(user);
		}
	}
}
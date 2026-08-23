/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:46:30 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:19:20 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void Server::invite(std::string &str, int &i, Client &c){
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		std::cerr << "Server: unknown channel:" + str.substr(cpy, i) + "\n" << std::flush;
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL)
	{
		std::cerr << "Invite: user[" + c.getNickName() + "] is not a moderator\n" << std::flush;
		return ;
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	if (arg.size() == 0)
		return;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan->getMember(arg.at(i));
		if (user == NULL){
			std::cerr << "INVITE: user [" + arg.at(i) + "] is not recognized\n" << std::flush;
		}
		else {
			chan->addinvited(user);
		}
	}
}
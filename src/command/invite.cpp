/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:46:30 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 22:15:41 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void Server::invite(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "not register");
		return;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, "this channel doesn't exist");
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL)
	{
		this->reply(&c, ERR_CHANOPRIVSNEEDED, "require to be operator");
		return ;
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	if (arg.size() == 0)
		return;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan->getMember(arg.at(i));
		if (user == NULL){
			this->reply(&c, ERR_NOSUCHNICK, "no such nickname");
		}
		else {
			chan->addinvited(user);
		}
	}
}
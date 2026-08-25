/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:00:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 13:37:43 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void Server::kick(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) +  ": this channel doesn't exist");
		return ;
	}
	if (chan->getModerator(c.getNickName()) == NULL){
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
		return ;
	}
	std::vector<std::string> arg = this->getArgsparse(str, ' ', i);
	if (arg.size() == 0)
		return ;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan->getMember(arg.at(i));
		if (user == NULL){
			this->reply(&c, ERR_NOSUCHNICK, chan->getName() +  ": no such nickname");
		}
		else {
			chan->delMember(user);
			if (chan->getModerator(user->getNickName()) != NULL)
				chan->delModerator(user);
		}
	}
}
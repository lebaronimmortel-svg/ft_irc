/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 22:16:14 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::join(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "require registration");
		return;
	}
	int fail = 0;
	std::vector<Channel *> *channels = this->getChannelListparse(&c, str, i);
	if (channels == NULL){
		return ;
	}
	int lenght = channels->size();
	for (int i = 0; i < lenght; i++){
		Channel *chan = channels->operator[](i);
		if(chan->getMember(c.getNickName()) != NULL)
			this->reply(&c, ERR_USERONCHANNEL, "already on channel");
		else
			chan->addUser(&c);
	}
}
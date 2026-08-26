/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 14:59:20 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void Server::join(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	int cpy = i;
	std::vector<Channel *> *channels = this->getChannelListparse(&c, str, i);
	if (channels == NULL){
		return ;
	}
	std::vector<std::string> args = this->getArgsparse(str, ' ', i);
	int lenght = channels->size();
	int lenght_args = args.size();
	for (int i = 0; i < lenght; i++){
		Channel *chan = channels->operator[](i);
		if (!chan){
			std::string name = this->getArgsparse(str, ',', cpy).at(i);
			Channel *nchan = new Channel(name);
			this->addChannel(nchan);
			nchan->addUser(&c);
		}
		if (chan->getMember(c.getNickName()) != NULL)
			this->reply(&c, ERR_USERONCHANNEL, chan->getName() +  ": already on channel");
		else
		{
			if (chan->getInviteOnlyStatus()){
				if (chan->getInvited(c.getNickName()) == NULL)
				{
					this->reply(&c, ERR_INVITEONLYCHAN, chan->getName() + ": cannot join channel (+i)");
					continue;
				}	
			}
			if (chan->getUserLimit() != 0 && chan->getMembers().size() >= chan->getUserLimit()){
				this->reply(&c, ERR_CHANNELISFULL, chan->getName() + ": cannot join channel (+l)");
				continue;
			}
			if (chan->getPasswordRequirement()){
				if (i < lenght_args){
					std::string key = args[i];
					if (chan->getPassword() != key){
						this->reply(&c, ERR_PASSWDMISMATCH, chan->getName() + ": cannot join channel (+k)");
						continue;
					}
				} else { 
					this->reply(&c, ERR_NEEDMOREPARAMS, chan->getName() + ": cannot join channel (+k)");
					continue;
				}
			}
			chan->addUser(&c);
			chan->delInvited(&c);
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:00:46 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 13:34:24 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"
#include <cstring>

void Server::privmsg(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}
	std::vector<std::string> target;
	int lenght = str.length();
	for (; i < lenght; i++){
		int cpy = i;
		for (;i < lenght && strchr("\0\r\n :", str[i]) == NULL; i++)
		target.push_back(str.substr(cpy, i));
		if (i < lenght && str[i] == ':')
			break;
	}
	if (str[i] != ':')
		return ;// no message
	i++;
	std::string mess = str.substr(i);
	int size = target.size();
	for (int y = 0; y < size; y++){
		std::string name = target[y];
		if (!name.empty() && name[0] == '#'){
			std::map<std::string, Channel *>::iterator it = this->getChannelList().find(name);
			if (it == this->getChannelList().end()){
				this->reply(&c, ERR_NOSUCHCHANNEL, "PRIVMSG: this channel doesn't exist");
				continue;
			}
			Channel *chan = it->second;
			if (!chan->getMember(c.getNickName()) == NULL)
				continue;
			std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
							  "@localhost PRIVMSG " + chan->getName() + " :" + mess + "\r\n";
			// a broadcast
		}
		else {
			std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
							  "@localhost PRIVMSG " + name + " :" + mess + "\r\n";
			Client *dest = this->get_client(name, 0, 0);// mode is 0, so check with username;
			if (!dest)
				continue;
			send(dest->getFd(), msg.c_str(), msg.size(), 0);
		}
	}
}
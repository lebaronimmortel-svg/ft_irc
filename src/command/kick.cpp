/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:00:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 19:09:04 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void kick(std::string &str, Client &c, Channel &chan){
	if (chan.getModerator(c.getNickName()) == NULL){
		std::cerr << "KICK: user [" + c.getNickName() + "] is not an operator\n";
		return ;
	}
	std::vector<std::string> args = getArgs(str);
	std::vector<std::string> arg = getArgs(str);
	if (arg.size() == 0)
		return ;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan.getMember(arg.at(i));
		if (user == NULL){
			std::cerr << "KICK: user [" + arg.at(i) + "] is not recognized\n";
		}
		else {
			chan.delMember(user);
			if (chan.getModerator(user->getNickName()) != NULL)
				chan.delModerator(user);
		}
	}
}
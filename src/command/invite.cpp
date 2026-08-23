/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:46:30 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 18:15:18 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


void invite(std::string &str, Client &c, Channel &chan){
	if (chan.getModerator(c.getNickName()) == NULL)
	{
		std::cerr << "Invite: user[" + c.getNickName() + "] is not a moderator\n";
		return ;
	}
	std::vector<std::string> arg = getArgs(str);
	if (arg.size() == 0)
		return;
	size_t lenght = arg.size();
	for (int i = 0; i < lenght; i++){
		Client *user = chan.getMember(arg.at(i));
		if (user == NULL){
			std::cerr << "INVITE: user [" + arg.at(i) + "] is not recognized\n";
		}
		else {
			chan.addinvited(user);
		}
	}
}
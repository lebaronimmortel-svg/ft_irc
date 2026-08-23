/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 19:13:41 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void topic(std::string &str, Client &c, Channel &chan){
	if (chan.getModerator(c.getNickName()) == NULL){
		std::cerr << "TOPIC: user [" + c.getNickName() + "] is not an operator\n";
		return ;
	}
	std::vector<std::string> arg = getArgs(str);
	if (arg.size() != 0){
		std::cerr << "TOPIC: didn't expected argument\n";
		return ;
	}
	chan.setTopicRestrictionStatus(!chan.getTopicRestrictionStatus());
}
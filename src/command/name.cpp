/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:51:25 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 19:54:12 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void name(std::string &str, Client &c, Channel &chan){
	std::vector<std::string> args = getArgs(str);
	if (args.size() != 1){
		std::cerr << "Pass: require only one argument\n";
		return ;
	}
	c.setUserName(args.at(0));
	if (c.getAuthenticated())
		return ;
	c.getAuthLevel() |= (1 << USERNAME);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	}
}
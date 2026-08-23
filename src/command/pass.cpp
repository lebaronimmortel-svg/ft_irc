/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 19:39:25 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

void pass(std::string &str, Client &c, Channel &chan){
	if (c.getAuthenticated()){
		std::cerr << "PASS: user [" + c.getNickName() + "] is already register\n";
		return ;
	}
	std::vector<std::string> args = getArgs(str);
	if (args.size() != 1){
		std::cerr << "Pass: require only one argument\n";
		return ;
	}
	if (args.at(0) != chan.getPassword()){
		std::cerr << "Pass: incorrect password provided\n";
		return ;
	};
	c.getAuthLevel() |= (1 << PASSWORD);
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthenticated() & reqperm) == reqperm){
		c.setAuthenticated(true);
	} 
}
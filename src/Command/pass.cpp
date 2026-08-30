/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:01 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

std::string cmd_sfx(std::string str);
void reset_auth_level(Server* serv, Client& c, int mode);
bool nicknameValid(std::string str);
void check_auth(Server *serv, Client& c);

void Server::pass(std::string &str, size_t &i, Client &c)
{	
	(void) i;

	if (c.getAuthenticated()){
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: user already registered");
		return ;
	}
	if (cmd_sfx(str) == _password)
		c.setPassAuth(1);
	c.setAuthLevel(c.getAuthLevel() | (1 << PASSWORD));

	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
	{
		check_auth(this, c);
	}
}
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

// parsing
std::string cmdSfx(std::string str);

// authentification
void checkAuth(Server *serv, Client& c);
void resetAuthLevel(Server* serv, Client& c, int mode);

/*
	password

		This function is meant to execute
		the PASS command from an IRC client:

		PASS password
*/
void Server::pass(std::string &str, size_t &i, Client &c)
{	
	(void) i;

	if (c.getAuthenticated())
	{
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: user already registered");
		return ;
	}

	if (cmdSfx(str) == _password)
		c.setPassAuth(1);
	c.setAuthLevel(c.getAuthLevel() | (1 << PASSWORD));

	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
		checkAuth(this, c);
}
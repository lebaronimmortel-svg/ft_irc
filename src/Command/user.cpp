/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:51:25 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:12 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

// parsing 
std::string cmdSfx(std::string str);
std::string cmdSfxFull(std::string str);

// authentification
void resetAuthLevel(Server* serv, Client& c, int mode);
void checkAuth(Server *serv, Client& c);

/*
	username

		This function is meant to
		execute the USER command
		from an IRC client:

		USER username mode servername realname
*/
void Server::user(std::string &str, size_t &i, Client &c)
{
	(void) i;
	
	if (c.getAuthenticated())
	{
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: already registered");
		return ;
	}

	std::vector<std::string> args = this->getArgsparse(str, ' ');
	if (args.size() >= 5)
		c.setUserAuthTmp(1);
	c.setUserAuth(cmdSfxFull(str));
	c.setAuthLevel(c.getAuthLevel() | (1 << USERNAME));

	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
		checkAuth(this, c);
}
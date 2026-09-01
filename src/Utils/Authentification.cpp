/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentification.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

// parsing 
bool nicknameValid(std::string str);

/*
	reset_authentification_levels :

		This function is meant to reinitialise
		authentification fields statuses
		in case of authentification failure 
*/
void resetAuthLevel(Server* serv, Client& c, int mode)
{
	c.setAuthLevel(c.getAuthLevel() & ~(1 << PASSWORD));
	c.setAuthLevel(c.getAuthLevel() & ~(1 << USERNAME));
	c.setAuthLevel(c.getAuthLevel() & ~(1 << NICKNAME));
	c.setNickAuth("");
	c.setUserAuth("");
	c.setUserAuthTmp(0);
	c.setNickAuthTmp(0);
	c.setPassAuth(0);

	if (mode == 0)
		serv->reply(&c, ERR_PASSWDMISMATCH, "IRCServer: wrong password");
	else if (mode == 1)
		serv->reply(&c, ERR_NEEDMOREPARAMS, c.getUserAuth() +  ": username requires 4 parameters");
	else if (mode == 2)
		serv->reply(&c, ERR_NICKNAMEINUSE, c.getNickAuth() +  ": nickname already in use");
	else if (mode == 3)
		serv->reply(&c, ERR_ERRONEUSNICKNAME, c.getNickAuth() +  ": erroneous nickname");	
}

/*
	Check_authentification :

		This function is meant
		to reinitialize client's
		authentification levels
		in case of an invalid 
		authentification input
*/
void	checkAuth(Server *serv, Client& c)
{
	if (c.getPassAuth() == 0)
	{
		resetAuthLevel(serv, c, 0);
		return ;
	}
	else if (c.getUserAuthTmp() == 0)
	{
		resetAuthLevel(serv, c, 1);
		return ;
	}
	else if (c.getNickAuthTmp() == 0)
	{
		if (!nicknameValid(c.getNickAuth()))
			resetAuthLevel(serv, c, 3);
		else if (serv->getClient(c.getNickAuth(), 0, 0) != NULL)
			resetAuthLevel(serv, c, 2);
		return ;
	}
	else
	{
		serv->reply(&c, RPL_WELCOME, ": welcome on server: IRCserver");
		c.setNickName(c.getNickAuth());
		c.setUserName(c.getUserAuth());
		c.setAuthenticated(true);
	}
}
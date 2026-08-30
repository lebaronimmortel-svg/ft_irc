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

std::string cmd_sfx(std::string str);
std::string cmd_sfx_full(std::string str);
void reset_auth_level(Server* serv, Client& c, int mode);
bool nicknameValid(std::string str);

/*
	reset_authentification_levels :

		This function is meant to reinitialise
		authentification fields statuses
		in case of authentification failure 

	mode == 0 :
		Wrong password has been submitted

	mode == 1 :
		Wrong username has been submitted

	mode == 2 :
		Nickname already in use
	
	mode == 3 :
		Erroneous nickname
*/
void reset_auth_level(Server* serv, Client& c, int mode)
{
	c.setAuthLevel(c.getAuthLevel() & ~(1 << PASSWORD));
	c.setAuthLevel(c.getAuthLevel() & ~(1 << USERNAME));
	c.setAuthLevel(c.getAuthLevel() & ~(1 << NICKNAME));
	c.setNickAuth("");
	c.setUserAuth("");
	c.setUserAuthString(0);
	c.setNickAuthString(0);
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

void Server::user(std::string &str, size_t &i, Client &c)
{
	(void) i;
	
	if (c.getAuthenticated()){
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: already register");
		return ;
	}

	std::vector<std::string> args = this->getArgsparse(str, ' ');
	if (args.size() >= 5)
		c.setUserAuthString(1);
	c.setUserAuth(cmd_sfx_full(str));
	c.setAuthLevel(c.getAuthLevel() | (1 << USERNAME));

	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
	{
		if (c.getPassAuth() == 0)
		{
			reset_auth_level(this, c, 0);
			return ;
		}
		else if (c.getUserAuthString() == 0)
		{
			reset_auth_level(this, c, 1);
			return ;
		}
		else if (c.getNickAuthString() == 0)
		{
			if (!nicknameValid(c.getNickAuth()))
				reset_auth_level(this, c, 3);
			else if (this->get_client(c.getNickAuth(), 0, 2) != NULL)
				reset_auth_level(this, c, 2);
			return ;
		}
		else
		{
			this->reply(&c, RPL_WELCOME, ": welcome on server: IRCserver");
			c.setNickName(c.getNickAuth());
			c.setUserName(c.getUserAuth());
			c.setAuthenticated(true);
		}
	}
}
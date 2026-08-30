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
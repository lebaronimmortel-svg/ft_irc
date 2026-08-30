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

std::string cmd_sfx_bis(std::string str)
{
	unsigned long i = 0;
	unsigned long len = str.size();
	std::string res = "";
	while (i < len && str[i] != ' ')
		i++;
	if (i == len)
		return res;
	i++;
	while (i < len && str[i])
			res += str[i++];
	return res;
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
	c.setUserAuth(cmd_sfx_bis(str));
	c.setAuthLevel(c.getAuthLevel() | (1 << USERNAME));

	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
	{
		if (c.getPassAuth() == 0)
		{
			this->reply(&c, ERR_PASSWDMISMATCH, "IRCServer: wrong password");
			c.setAuthLevel(c.getAuthLevel() & ~(1 << PASSWORD));
			c.setAuthLevel(c.getAuthLevel() & ~(1 << USERNAME));
			c.setAuthLevel(c.getAuthLevel() & ~(1 << NICKNAME));
			c.setUserAuth("");
			c.setNickAuth("");
			c.setUserAuthString(0);
			return ;
		}
		else if (c.getUserAuthString() == 0)
		{
			this->reply(&c, ERR_NEEDMOREPARAMS, c.getUserAuth() +  ": username requires 4 parameters");
			c.setAuthLevel(c.getAuthLevel() & ~(1 << PASSWORD));
			c.setAuthLevel(c.getAuthLevel() & ~(1 << USERNAME));
			c.setAuthLevel(c.getAuthLevel() & ~(1 << NICKNAME));
			c.setNickAuth("");
			c.setUserAuth("");
			c.setPassAuth(0);
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
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
void check_auth(Server *serv, Client& c);

void Server::user(std::string &str, size_t &i, Client &c)
{
	(void) i;
	
	if (c.getAuthenticated()){
		this->reply(&c, ERR_ALREADYREGISTRED, "IRCServer: already registered");
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
		check_auth(this, c);
	}
}
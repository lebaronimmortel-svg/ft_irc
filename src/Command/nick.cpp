/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 19:09:18 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:39:59 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

std::string cmd_sfx(std::string str);
void reset_auth_level(Server* serv, Client& c, int mode);
void check_auth(Server *serv, Client& c);

bool nicknameValid(std::string str){
	size_t length = str.length();
	if (length == 0 || length > 9)
		return false;
	std::string spec("[]\\`_^{|}");
	for (size_t i = 0; i < length; i++){
		if (i == 0){
			if (!isalpha(str[i]) && spec.find(str[i]) == spec.npos)
				return false;
		}
		else{
			if (!isalnum(str[i]) && spec.find(str[i]) == spec.npos && str[i] != '-')
				return false;
		}
	}
	return (true);
}

void Server::nick(std::string &str, size_t &i, Client &c)
{
	(void) i;
	std::vector<std::string> args = this->getArgsparse(str, ' ');
	std::string name = cmd_sfx(str);
	if (nicknameValid(name) && this->get_client(name, 0, 0) == NULL)
	{
		if (c.getAuthenticated())
		{
			c.setNickName(name);
			return ;
		}
		else
			c.setNickAuthString(1);
	}
	if (c.getAuthenticated())
	{
		if (this->get_client(name, 0, 0) != NULL)
			this->reply(&c, ERR_NICKNAMEINUSE, name +  ": nickname already in use");
		else if (!nicknameValid(name))
			this->reply(&c, ERR_ERRONEUSNICKNAME, name +  ": erroneous nickname");
		return ;
	}
	c.setNickAuth(name);
	c.setAuthLevel(c.getAuthLevel() | (1 << NICKNAME));
	size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
	if ((c.getAuthLevel() & reqperm) == reqperm)
	{
		check_auth(this, c);
	}
}
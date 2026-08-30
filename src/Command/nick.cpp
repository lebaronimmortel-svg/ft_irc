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
	if (args.size() == 0){
		this->reply(&c, ERR_NONICKNAMEGIVEN, str +  ": require a nickname");
		return ;
	}
	std::string name = cmd_sfx(str);
	if (nicknameValid(name) && this->get_client(name, 0, 2) == NULL)
		c.setNickAuthString(1);
	c.setNickAuth(name);
	if (c.getAuthenticated())
		return ;
	c.setAuthLevel(c.getAuthLevel() | (1 << NICKNAME));
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
			if (!nicknameValid(name))
				reset_auth_level(this, c, 3);
			else if (this->get_client(name, 0, 2) != NULL)
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
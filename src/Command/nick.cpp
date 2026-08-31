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

// parsing
std::string cmd_sfx(std::string str);
bool nicknameValid(std::string str);

// authentification
void reset_auth_level(Server* serv, Client& c, int mode);
void check_auth(Server *serv, Client& c);

/*
	nickname

		This function is meant to
		execute the NICK command 
		from an IRC client:

		NICK nickname
*/
void Server::nick(std::string &str, size_t &i, Client &c)
{
	(void) i;

	/*
		Parsing the provided input
	*/
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

	/*
		case 1 :

			Client is already
			registered in the 
			server
	*/
	if (c.getAuthenticated())
	{
		if (this->get_client(name, 0, 0) != NULL)
			this->reply(&c, ERR_NICKNAMEINUSE, name +  ": nickname already in use");
		else if (!nicknameValid(name))
			this->reply(&c, ERR_ERRONEUSNICKNAME, name +  ": erroneous nickname");
	}

	/*
		case 2 :

			Client isn't
			registered in 
			the server yet
	*/
	else
	{
		c.setNickAuth(name);
		c.setAuthLevel(c.getAuthLevel() | (1 << NICKNAME));
		size_t reqperm = (1 << PASSWORD) | (1 << NICKNAME) | (1 << USERNAME);
		if ((c.getAuthLevel() & reqperm) == reqperm)
			check_auth(this, c);
	}
}
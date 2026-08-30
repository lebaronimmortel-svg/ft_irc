#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

bool nicknameValid(std::string str);

/*
	reset_authentification_levels :

		This function is meant to reinitialise
		authentification fields statuses
		in case of authentification failure 

		mode == 0 :
			Wrong password has been submitted

		mode == 1 :
			Wrong username format

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

/*
	Check_authentification :

		This function


*/
void	check_auth(Server *serv, Client& c)
{
	if (c.getPassAuth() == 0)
	{
		reset_auth_level(serv, c, 0);
		return ;
	}
	else if (c.getUserAuthString() == 0)
	{
		reset_auth_level(serv, c, 1);
		return ;
	}
	else if (c.getNickAuthString() == 0)
	{
		if (!nicknameValid(c.getNickAuth()))
			reset_auth_level(serv, c, 3);
		else if (serv->get_client(c.getNickAuth(), 0, 2) != NULL)
			reset_auth_level(serv, c, 2);
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
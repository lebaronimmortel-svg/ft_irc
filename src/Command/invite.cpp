/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:46:30 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:38:58 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/*
	invite

		This function is meant to
		execute the INVITE 
		command from an IRC 
		operator:

		INVITE #channel user
*/
void Server::invite(std::string &str, size_t &i, Client &c)
{
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require registration");
		return;
	}

	/*
		Parsing provided input
	*/
	std::vector<std::string> arg = this->getArgsparse(str, ' ');

	if (arg.size() == 0)
		return;
		
	size_t cpy = i;

	/*
		Extracting targeted channel
		from parsed input
	*/
	Channel *chan = this->getChannel(arg.at(1));
	if (chan == NULL)
	{
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) + ": this channel doesn't exist");
		return ;
	}
	
	if (chan->getModerator(c.getNickName()) == NULL)
	{
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": require to be operator");
		return ;
	}

	/*
		Extracting targeted client
		from parsed input
	*/
	Client *user = this->getClient(arg.at(2), 0, 2);
	if (user == NULL)
		this->reply(&c, ERR_NOSUCHNICK, chan->getName() +  ": no such nickname");
	else
	{
		/*
			Inviting
		*/
		chan->addInvited(user);
		std::string msg = "You have been invited by " + c.getNickName() + " to join channel " + chan->getName() + "\r\n"; 
		send(user->getFd(), msg.c_str(), msg.size(), 0);
	}
}
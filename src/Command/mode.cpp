/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:32:51 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 13:38:10 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"
#include <cstring>
#include <sstream>

void add_order(bool state, std::string &order, char c){
	size_t find = order.find(c); 
	if (!state){
		if (c == 'k')
			return ;
		if (find != order.npos)
			order.erase(find);
	} else {
		if (find == order.npos)
			order.append(&c);
	}
}

std::string get_word(std::string str, size_t &i, bool &end){
	size_t lenght = str.length();
	for (; i < lenght && str[i] == ' '; i++);
	int start = i;
	bool word = false;
	for (; i < lenght && str[i] != ' '; i++)
		word = true;
	if (!word)
	{
		end = true;
		return "";
	}
	end = true;
	return (str.substr(start, i));
}

mode_s *Server::getflagmode(Channel *chan, Client *c, std::string str){
	mode_s *args = new mode_s();
	args->flag.i = -1;
	args->flag.t = -1;
	args->flag.k = -1;
	args->flag.o = -1;
	args->flag.l = -1;
	size_t lenght = str.length();
	short state = 0;
	bool init_state = false;
	std::string order = "";
	size_t i = 0;
	for (; i < lenght; i++){
		if (str[i] == '+')
		{
			state = 1;
			init_state = true;
		}
		else if (str[i] == '-')
		{
			state = 0;
			init_state = true;
		}
		else if (str[i] == ' ')
			break;
		else if (!init_state || strchr("itkol", str[i]) == NULL)
		{
			this->reply(c, ERR_UMODEUNKNOWNFLAG, chan->getName() +  "unknown mode flag: " + str[i]);
			continue;
		}
		else {
			switch (str[i]){
				case 'i':{
					args->flag.i = state;
				}
				case 't':{
					args->flag.t = state;
				}
				case 'k':{
					args->flag.k = state;
					add_order(state, order, 'k');
				}
				case 'o':{
					args->flag.o = state;
					add_order(state, order, 'o');
				}
				case 'l':{
					args->flag.l = state;
					add_order(state, order, 'l');
				}
			}
		}
	}
	if (!init_state){
		this->reply(c, ERR_NEEDMOREPARAMS, chan->getName() +  "need more parameter");
		return (NULL);
	}
	std::vector<std::string> wvec;
	size_t lenght_vec = 0;
	bool end = false;
	for (; i < lenght; i++){
		std::string word = get_word(str, i, end);
		if (end)
			break;
		wvec.push_back(word);
		lenght_vec++;
	};
	if (lenght_vec == 0)
		return (args);
	size_t order_l = order.length();
	size_t vec_i = 0;
	for (size_t y = 0; y < order_l; y++){
		switch (order[y]){
			case 'k':{
				args->value.k = wvec.at(vec_i);
			}
			case 'o':{
				for (; vec_i < lenght_vec - y; vec_i++){
					args->value.o.push_back(wvec.at(vec_i));
				}
			}
			case 'l':{
				std::istringstream is(wvec.at(vec_i));
				is >> args->value.l;
				if (is.fail() || !is.eof()){
					this->reply(c, ERR_UNKNOWNMODE, chan->getName() +  ": error conversion");
				}
			}
		}
	}
	return (args);
}

void Server::mode(std::string &str, int &i, Client &c){
	if (!c.getAuthenticated())
	{
		this->reply(&c, ERR_NOTREGISTERED, "IRCServer: require to be registered");
		return;
	}
	int cpy = i;
	Channel *chan = this->getChannelparse(str, i);
	if (chan == NULL){
		this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) + ": this channel doesn't exist");
		return ;
	}
	if (chan->getModerator(c.getNickName()) != NULL){
		this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() +  ": not an operator");
		return ;
	}
	mode_s *args = this->getflagmode(chan, &c, str);
	if (args->flag.i == 1){
		chan->setInviteOnlyStatus(true);
	}
	else if (args->flag.i == 0)
		chan->setInviteOnlyStatus(false);
	if (args->flag.t == 1){
		chan->setTopicRestrictionStatus(true);
	}
	if (args->flag.t == 0){
		chan->setTopicRestrictionStatus(false);
	}
	if (args->flag.k == 1){
		chan->setPasswordRequirement(true);
		chan->setPassword(args->value.k);
	}
	else if (args->flag.k == 0){
		chan->setPasswordRequirement(false);
	}
	if (args->flag.l == 1){
		chan->setUserLimit(args->value.l);
	}
	else if  (args->flag.l == 0)
		chan->setUserLimit(0);
	if (args->flag.o == 1){
		for (size_t i = 0; i < args->value.o.size(); i++){
			Client *client = chan->getMember(args->value.o.at(i));
			if (client != NULL)
				chan->addModerator(client);
			else
			{
				this->reply(&c, ERR_NOSUCHNICK, chan->getName() +  ": no such nickname");
			}
		}
	}
	else if (args->flag.o == 0){
		for (size_t i = 0; i < args->value.o.size(); i++){
			Client *client = chan->getMember(args->value.o.at(i));
			if (client != NULL)
			{
				chan->delMember(client);
				if (chan->getModerator(args->value.o.at(i)) != NULL)
					chan->delModerator(client);
			}
			else
			{
				this->reply(&c, ERR_NOSUCHNICK, chan->getName() +  ": no such nickname");
			}
		}
	}
	delete args;
}
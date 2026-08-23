/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:35:33 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 01:21:49 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>

typedef void (Server::*cmdfunc)(std::string &str, int &i,Client &);

enum cmdlistenum {
    INVITE,
    KICK,
    TOPIC,
    MODE,
    PASS,
    NICK,
    NAME,
    JOIN,// to do
    PRIVMSG,// to do
};

struct cmdlist {
    char *name;
    cmdfunc call;
};

cmdlist cmdLU[] = {
    [INVITE] = {
        .name = "invite",
        .call = Server::invite,
    },
    [KICK] = {
        .name = "kick",
        .call =  Server::kick,
    },
    [TOPIC] = {
        .name = "topic",
        .call =  Server::topic,
    },
    [MODE] = {
        .name = "mode",
        .call =  Server::mode,
    },
    [PASS] = {
        .name = "pass",
        .call =  Server::pass,
    },
    [NICK] = {
        .name = "nick",
        .call =  Server::nick,
    },
    [NAME] = {
        .name = "name",
        .call =  Server::name,
    },
    [JOIN] = {
        .name = "join",
        .call =  Server::join,
    },
    [PRIVMSG] = {
        .name = "privmsg",
        .call =  Server::privmsg,
    },
};

struct mode_flag{
	short i;
	short t;
	short k;
	short o;
	short l;
};

struct mode_value{
	std::string k;
	size_t l;
	std::vector<std::string> o;
};

struct mode_s{
	mode_value value;
	mode_flag flag;
};
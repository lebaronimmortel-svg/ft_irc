/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:35:33 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 21:47:50 by tseche           ###   ########.fr       */
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
        .name = "INVITE",
        .call = Server::invite,
    },
    [KICK] = {
        .name = "KICK",
        .call =  Server::kick,
    },
    [TOPIC] = {
        .name = "TOPIC",
        .call =  Server::topic,
    },
    [MODE] = {
        .name = "MODE",
        .call =  Server::mode,
    },
    [PASS] = {
        .name = "PASS",
        .call =  Server::pass,
    },
    [NICK] = {
        .name = "NICK",
        .call =  Server::nick,
    },
    [NAME] = {
        .name = "USER",
        .call =  Server::user,
    },
    [JOIN] = {
        .name = "JOIN",
        .call =  Server::join,
    },
    [PRIVMSG] = {
        .name = "PRIVMSG",
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
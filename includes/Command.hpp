/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:35:33 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 19:55:14 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>

typedef void (* cmdfunc)(std::string &str, Client &, Channel &);

void invite(std::string &str, Client &, Channel &);
void kick(std::string &str, Client &, Channel &);
void topic(std::string &str, Client &, Channel &);
void mode(std::string &str, Client &, Channel &);
void pass(std::string &str, Client &, Channel &);
void nick(std::string &str, Client &, Channel &);
void name(std::string &str, Client &, Channel &);
void join(std::string &str, Client &, Channel &);
void privmsg(std::string &str, Client &, Channel &);
void unknowncmd(std::string &str, Client &, Channel &);

std::vector<std::string> &getArgs(std::string &str);

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
        .call = invite,
    },
    [KICK] = {
        .name = "kick",
        .call = kick,
    },
    [TOPIC] = {
        .name = "topic",
        .call = topic,
    },
    [MODE] = {
        .name = "mode",
        .call = mode,
    },
    [PASS] = {
        .name = "pass",
        .call = pass,
    },
    [NICK] = {
        .name = "nick",
        .call = nick,
    },
    [NAME] = {
        .name = "name",
        .call = name,
    },
    [JOIN] = {
        .name = "join",
        .call = join,
    },
    [PRIVMSG] = {
        .name = "privmsg",
        .call = privmsg,
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
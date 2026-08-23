/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:35:33 by tseche            #+#    #+#             */
/*   Updated: 2026/08/22 23:32:59 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef int (* cmdfunc)(std::string &str, Client &, Channel &);

int invite(std::string &str, Client &, Channel &);
int kick(std::string &str, Client &, Channel &);
int topic(std::string &str, Client &, Channel &);
int mode(std::string &str, Client &, Channel &);
int pass(std::string &str, Client &, Channel &);
int nick(std::string &str, Client &, Channel &);
int name(std::string &str, Client &, Channel &);
int join(std::string &str, Client &, Channel &);
int privmsg(std::string &str, Client &, Channel &);
int unknowncmd(std::string &str, Client &, Channel &);

enum cmdlistenum {
    INVITE,
    KICK,
    TOPIC,
    MODE,
    PASS,
    NICK,
    NAME,
    JOIN,
    PRIVMSG,
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
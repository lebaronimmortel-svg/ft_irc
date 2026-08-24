/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 20:36:01 by tseche            #+#    #+#             */
/*   Updated: 2026/08/24 20:55:37 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#pragma once

#include <string>

enum reply_flag{
	/* ——— Welcome / registration (RPL 001-004) ————————————————————————————————— */
	RPL_WELCOME,
	RPL_YOURHOST,
	RPL_CREATED,
	RPL_MYINFO,

	/* ——— Channel / user info replies —————————————————————————————————————————— */
	RPL_UMODEIS, // current user = modes,
	RPL_CHANNELMODEIS, // <channel> <mode> <mode = params>,
	RPL_NOTOPIC, // <channel> :No topic is = set,
	RPL_TOPIC, // <channel> = :<topic>,
	RPL_TOPICWHOTIME, // <channel> <nick> = <setat>,
	RPL_INVITING, // <channel> = <nick>,
	RPL_NAMREPLY, // = <channel> = :<nicks>,
	RPL_ENDOFNAMES, // <channel> :End of /NAMES = list,

	/* ——— Error replies (4xx) —————————————————————————————————————————————————— */
	ERR_NOSUCHNICK, // <nick> :No such = nick/channel,
	ERR_NOSUCHCHANNEL, // <channel> :No such = channel,
	ERR_CANNOTSENDTOCHAN, // <channel> :Cannot send to = channel,
	ERR_TOOMANYCHANNELS, // <channel> :You have joined too many = channels,
	ERR_NORECIPIENT, // :No recipient given = (<command>),
	ERR_NOTEXTTOSEND, // :No text to = send,
	ERR_UNKNOWNCOMMAND, // <command> :Unknown = command,
	ERR_NONICKNAMEGIVEN, // :No nickname = given,
	ERR_ERRONEUSNICKNAME, // <nick> :Erroneous = nickname,
	ERR_NICKNAMEINUSE, // <nick> :Nickname is already in = use,
	ERR_USERNOTINCHANNEL, // <nick> <channel> :They aren't on that = channel,
	ERR_NOTONCHANNEL, // <channel> :You're not on that = channel,
	ERR_USERONCHANNEL, // <user> <channel> :is already on = channel,
	ERR_NEEDMOREPARAMS, // <command> :Not enough = parameters,
	ERR_ALREADYREGISTRED, // :You may not = reregister,
	ERR_PASSWDMISMATCH, // :Password = incorrect,
	ERR_CHANNELISFULL, // <channel> :Cannot join channel = (+l),
	ERR_UNKNOWNMODE, // <char> :is unknown mode char to = me,
	ERR_INVITEONLYCHAN, // <channel> :Cannot join channel = (+i),
	ERR_BADCHANNELKEY, // <channel> :Cannot join channel = (+k),
	ERR_NOPRIVILEGES, // :Permission Denied- You're not an IRC = operator,
	ERR_CHANOPRIVSNEEDED, // <channel> :You're not channel = operator,
	ERR_UMODEUNKNOWNFLAG, // :Unknown MODE = flag,
	ERR_USERSDONTMATCH, // :Cannot change mode for other = users,

	/* ——— Not registered ——————————————————————————————————————————————————————— */
	ERR_NOTREGISTERED, // :You have not = registered,
};

std::string replay_flag_value[ERR_NOTREGISTERED + 1] = {
	[RPL_WELCOME] = "001",
	[RPL_YOURHOST] ="002",
	[RPL_CREATED] = "003",
	[RPL_MYINFO] = "004",
	[RPL_UMODEIS] = "221",
	[RPL_CHANNELMODEIS] = "324",
	[RPL_NOTOPIC] = "331",
	[RPL_TOPIC] = "332",
	[RPL_TOPICWHOTIME] = "333", 
	[RPL_INVITING] = "341",
	[RPL_NAMREPLY] = "353",
	[RPL_ENDOFNAMES] = "366",
	[ERR_NOSUCHNICK] = "401",
	[ERR_NOSUCHCHANNEL] = "403",
	[ERR_CANNOTSENDTOCHAN] = "404",
	[ERR_TOOMANYCHANNELS] = "405",
	[ERR_NORECIPIENT] = "411",
	[ERR_NOTEXTTOSEND] = "412",
	[ERR_UNKNOWNCOMMAND] = "421",
	[ERR_NONICKNAMEGIVEN] = "431",
	[ERR_ERRONEUSNICKNAME] = "432",
	[ERR_NICKNAMEINUSE] = "433",
	[ERR_USERNOTINCHANNEL] = "441",
	[ERR_NOTONCHANNEL] = "442", 
	[ERR_USERONCHANNEL] = "443",
	[ERR_NEEDMOREPARAMS] = "461",
	[ERR_ALREADYREGISTRED] = "462",
	[ERR_PASSWDMISMATCH] = "464", 
	[ERR_CHANNELISFULL] = "471",
	[ERR_UNKNOWNMODE] = "472",
	[ERR_INVITEONLYCHAN] = "473",
	[ERR_BADCHANNELKEY] = "475",
	[ERR_NOPRIVILEGES] = "481",
	[ERR_CHANOPRIVSNEEDED] = "482",
	[ERR_UMODEUNKNOWNFLAG] = "501",
	[ERR_USERSDONTMATCH] = "502",

	[ERR_NOTREGISTERED] = "451",
};
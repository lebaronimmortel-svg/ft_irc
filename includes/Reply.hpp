/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 20:36:01 by tseche            #+#    #+#             */
/*   Updated: 2026/08/25 22:42:21 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

extern const std::string reply_flag_value[ERR_NOTREGISTERED + 2];

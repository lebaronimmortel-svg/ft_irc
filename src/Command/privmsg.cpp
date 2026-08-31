/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:00:46 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 14:40:59 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"
#include <cstring>

/*
    private message

        this function is meant to
        execute the PRIVMSG
        command from an IRC client:

        PRIVMSG #channel1,user1,#channel2,user2,user3,#channel3 :message
*/
void Server::privmsg(std::string &str, size_t &i, Client &c)
{
    if (!c.getAuthenticated())
    {
        this->reply(&c, ERR_NOTREGISTERED, ":You have not registered");
        return;
    }

    /*
        Parsing provided input to 
        reach targets
    */
    size_t length = str.length();
    while (i < length && (str[i] == ' ' || str[i] == '\r' || str[i] == '\n'))
        i++;
    if (i >= length || str[i] == ':')
    {
        this->reply(&c, ERR_NORECIPIENT, ":No recipient given (PRIVMSG)");
        return;
    }

    /*
        Storing targets list
    */
    size_t target_start = i;
    while (i < length && strchr("\r\n :", str[i]) == NULL)
        i++;
    std::string raw_targets = str.substr(target_start, i - target_start);

    /*
        Parsing provided input to 
        reach message
    */
    while (i < length && str[i] == ' ')
        i++;
    if (i >= length)
    {
        this->reply(&c, ERR_NOTEXTTOSEND, ":No text to send");
        return;
    }
    if (str[i] == ':')
        i++;
   
    /*
        Storing message
    */
    std::string mess = str.substr(i);
    size_t end = mess.find_last_not_of("\r\n");
    if (end != std::string::npos)
        mess = mess.substr(0, end + 1);
    else
        mess.clear();
    if (mess.empty())
    {
        this->reply(&c, ERR_NOTEXTTOSEND, ":No text to send");
        return;
    }

    /*
        Splitting the target list
        into a list of targets
    */
    std::vector<std::string> targets = getArgsparse(raw_targets, ',');

    /*
        Browsing the targets list
    */
    for (size_t y = 0; y < targets.size(); y++)
    {
        std::string name = targets[y];
        if (name.empty())
            continue;

        /*
            case 1 :

                target is a channel
        */
        if (name[0] == '#')
        {
            std::map<std::string, Channel *>::iterator it = this->getChannelList().find(name);
            if (it == this->getChannelList().end())
            {
                this->reply(&c, ERR_NOSUCHCHANNEL, name + " :No such channel");
                continue;
            }
            Channel *chan = it->second;
            if (chan->getMember(c.getNickName()) == NULL)
            {
                this->reply(&c, ERR_CANNOTSENDTOCHAN, name + " :Cannot send to channel");
                continue;
            }
            std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
                              "@localhost PRIVMSG " + chan->getName() + " :" + mess + "\r\n";
            chan->broadcast(msg, c.getFd());
        }

        /*
            case 2 :

                target is a client
        */
        else 
        {
            Client *dest = this->get_client(name, 0, 0);
            if (!dest)
            {
                this->reply(&c, ERR_NOSUCHNICK, name + " :No such nick/channel");
                continue;
            }
            std::string msg = ":" + c.getNickName() + "!" + c.getUserName() +
                              "@localhost PRIVMSG " + dest->getNickName() + " :" + mess + "\r\n";
            send(dest->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}
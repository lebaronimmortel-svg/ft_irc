/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include <iostream>
#include <string>

// print
void print_del_member(std::string name, std::string nick);

/*
    Destructor
*/
Channel::~Channel() {}

/*
    Constructor
*/
Channel::Channel(std::string name) : _name(name), _requires_password(0), _invite_only_mod(0), _user_limit(0) {}

/*
    get_name
*/
std::string Channel::getName()
{
    return _name;
}

/*
    get_password
*/
std::string Channel::getPassword()
{
    return _password;
}

/*
    get_member
*/
Client* Channel::getMember(std::string username)
{
    try 
    {
        Client *user = _members.at(username);
        return (user);
    }
    catch (std::exception &e)
    {
        return (NULL);
    }
}

/*
    get_members
*/
std::map<std::string, Client*> Channel::getMembers()
{
    return this->_members;
}

/*
    get_moderator
*/
Client* Channel::getModerator(std::string username)
{
    try 
    {
        Client *user = _moderators.at(username);
        return (user);
    }
    catch (std::exception &e)
    {
        return (NULL);
    }
}

/*
    get_invited
*/
Client* Channel::getInvited(std::string username)
{
    try 
    {
        Client *user = _invited.at(username);
        return (user);
    }
    catch (std::exception &e){
        return (NULL);
    }
    
}

/*
    get_password_requirement
*/
bool Channel::getPasswordRequirement()
{
    return _requires_password;
}

/*
    get_invite_only_status
*/
bool Channel::getInviteOnlyStatus()
{
    return _invite_only_mod;
}

/*
    get_topic_restriction_status
*/
bool Channel::getTopicRestrictionStatus()
{
    return _topic_restriction; 
}

/*
    get_topic
*/
std::string Channel::getTopic()
{
    return _topic;
}

/*
    get_topic_last_modification_date
*/
std::string Channel::getTopicLastModifDate()
{
    return _topic_last_modif_date;
}

/*
    get_topic_last_modifier_username
*/
std::string Channel::getTopicLastModifierUsername()
{
    return _topic_last_modifier_username;
}

/*
    get_user_limit
*/
unsigned int Channel::getUserLimit()
{
    return _user_limit;
}

/*
    set_name
*/
void    Channel::setName(std::string name)
{
    _name = name;
}

/*
    set_password
*/
void    Channel::setPassword(std::string password)
{
    _password = password;
}

/*
    set_password_requirement
*/
void    Channel::setPasswordRequirement(bool status)
{
    _requires_password = status;
}

/*
    set_invite_only_status
*/
void    Channel::setInviteOnlyStatus(bool status)
{
    _invite_only_mod = status;
}

/*
    set_topic_restriction_status
*/
void    Channel::setTopicRestrictionStatus(bool status)
{
    _topic_restriction = status;
}

/*
    set_topic
*/
void    Channel::setTopic(std::string topic)
{
    _topic = topic;
}

/*
    set_topic_last_modification_date
*/
void    Channel::setTopicLastModifDate(std::string date)
{
    _topic_last_modif_date = date;
}

/*
    set_topic_last_modifier_username
*/
void    Channel::setTopicLastModifierUsername(std::string username)
{
    _topic_last_modifier_username = username;
}

/*
    set_user_limit
*/
void    Channel::setUserLimit(unsigned int limit)
{
    _user_limit = limit;
}

/*
    add_user
*/
void Channel::addUser(Client *client)
{
    _members.insert(std::make_pair(client->getNickName(), client));
    client->addChannel(this);
} 

/*
    add_moderator
*/
void Channel::addModerator(Client *c)
{
    this->_moderators[c->getNickName()] = c; 
}

/*
    add_invited
*/
void Channel::addInvited(Client  *c)
{
    this->_invited[c->getNickName()] = c;
}

/*
    delete_moderator
*/
void Channel::delModerator(Client *c)
{
    try 
    {
        this->_moderators.at(c->getNickName());
        this->_moderators.erase(c->getNickName());
    } 
    catch (std::exception &e)
    {
        return ;
    }
}

/*
    delete_member
*/
void Channel::delMember(Client *c)
{
    try 
    {
        this->_members.at(c->getNickName());
        this->_members.erase(c->getNickName());
        if (this->_moderators.find(c->getNickName()) != this->_moderators.end())
            delModerator(c);
        print_del_member(_name, c->getNickName());
    } 
    catch (std::exception &e)
    {
        return ;
    }
}

/*
    delete_invited
*/
void Channel::delInvited(Client *c)
{
    try {
        this->_invited.at(c->getNickName());
        this->_invited.erase(c->getNickName());
    }
    catch (std::exception &e){
        return ;
    }
}

/*
    broadcast
*/
void Channel::broadcast(std::string msg, int fd)
{
    for (std::map<std::string, Client *>::iterator i = _members.begin(); i != _members.end(); i++)
    {
        if (i->second->getFd() != fd)
            send(i->second->getFd(), msg.c_str(), msg.size(), 0);
    }
}
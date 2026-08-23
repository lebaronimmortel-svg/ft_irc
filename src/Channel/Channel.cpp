#include "../../includes/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

std::string Channel::getName()
{
    return _name;
}

std::string Channel::getPassword()
{
    return _password;
}

Client* Channel::getMember(std::string username)
{
    return _members.at(username);
}

Client* Channel::getModerator(std::string username)
{
    return _moderators.at(username);
}

Client* Channel::getInvited(std::string username)
{
    return _invited.at(username);
}

bool Channel::getPasswordRequirement()
{
    return _requires_password;
}

bool Channel::getInviteOnlyStatus()
{
    return _invite_only_mod;
}

bool Channel::getTopicRestrictionStatus()
{
    return _topic_restriction; 
}

std::string Channel::getTopic()
{
    return _topic;
}

std::string Channel::getTopicLastModifDate()
{
    return _topic_last_modif_date;
}

std::string Channel::getTopicLastModifierUsername()
{
    return _topic_last_modifier_username;
}

unsigned int Channel::getUserLimit()
{
    return _user_limit;
}

void    Channel::setName(std::string name)
{
    _name = name;
}

void    Channel::setPassword(std::string password)
{
    _password = password;
}

void    Channel::setPasswordRequirement(bool status)
{
    _requires_password = status;
}

void    Channel::setInviteOnlyStatus(bool status)
{
    _invite_only_mod = status;
}

void    Channel::setTopicRestrictionStatus(bool status)
{
    _topic_restriction = status;
}

void    Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void    Channel::setTopicLastModifDate(std::string date)
{
    _topic_last_modif_date = date;
}

void    Channel::setTopicLastModifierUsername(std::string username)
{
    _topic_last_modifier_username = username;
}

void    Channel::setUserLimit(unsigned int limit)
{
    _user_limit = limit;
}

std::string Channel::belongs_to_channel(int fd)
{
    for (std::map<std::string, Client *>::iterator i = _members.begin(); i != _members.end(); i++)
    {
        if (i->second->getFd() == fd)
            return i->second->getUserName();
    }
    return ("");
}
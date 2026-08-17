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

bool getPasswordRequirement()
{
    return _requires_password;
}

bool getInviteOnlyStatus()
{
    return _invite_only_mod;
}

bool getTopicRestrictionStatus()
{
    return _topic_restriction; 
}

std::string getTopic()
{
    return _topic;
}

std::string getTopicLastModifDate()
{
    return _topic_last_modif_date;
}

std::string getTopicLastModifierUsername()
{
    return _topic_last_modifier_username;
}

void    setName(std::string name)
{
    _name = name;
}

void    setPassword(std::string password)
{
    _password = password;
}

void    setPasswordRequirement(bool status)
{
    _requires_password = status;
}

void    setInviteOnlyStatus(bool status)
{
    _invite_only_mod = status;
}

void    setTopicRestrictionStatus(bool status)
{
    _topic_restriction = status;
}

void    setTopic(std::string topic)
{
    _topic = topic;
}

void    setTopicLastModifDate(std::string date)
{
    _topic_last_modif_date = date;
}

void    setTopicLastModifierUsername(std::string username)
{
    _topic_last_modifier_username = username;
}
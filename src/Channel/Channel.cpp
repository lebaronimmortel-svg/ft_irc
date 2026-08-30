#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include <iostream>
#include <string>

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(std::string name) : _name(name) {}

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
     try {
        Client *user = _members.at(username);
        return (user);
    }
    catch (std::exception &e){
        return (NULL);
    }
}

std::map<std::string, Client*> Channel::getMembers(){
    return this->_members;
}

Client* Channel::getModerator(std::string username)
{
    try {
        Client *user = _moderators.at(username);
        return (user);
    }
    catch (std::exception &e){
        return (NULL);
    }
}

Client* Channel::getInvited(std::string username)
{
    try {
        Client *user = _invited.at(username);
        return (user);
    }
    catch (std::exception &e){
        return (NULL);
    }
    
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

void Channel::addUser(Client *client)
{
    _members.insert(std::make_pair(client->getNickName(), client));
    client->addChannel(this);
} 
void Channel::addModerator(Client *c){
    this->_moderators[c->getNickName()] = c; 
}

void Channel::delModerator(Client *c){
    try {
        this->_moderators.at(c->getNickName());
        this->_moderators.erase(c->getNickName());
    } catch (std::exception &e){
        return ;
    }
}

void Channel::addinvited(Client  *c){
    this->_invited[c->getNickName()] = c;
}

void Channel::delMember(Client *c)
{
    try 
    {
        this->_members.at(c->getNickName());
        this->_members.erase(c->getNickName());
        std::cout << std::endl << BLUE << "╔═════════════════════╗" << RESET << std::endl;
        std::cout << BLUE << "║ Client quit channel ║" << std::endl;
        std::cout << BLUE << "╚═════════════════════╝" << RESET << std::endl;
        std::cout << BLUE << "nickname: " << RESET << c->getNickName() << std::endl;
        std::cout << BLUE << "quit: " << RESET << _name << std::endl << std::endl;
    } 
    catch (std::exception &e)
    {
        return ;
    }
}

void Channel::delInvited(Client *c){
    try {
        this->_invited.at(c->getNickName());
        this->_invited.erase(c->getNickName());
    }
    catch (std::exception &e){
        return ;
    }
}
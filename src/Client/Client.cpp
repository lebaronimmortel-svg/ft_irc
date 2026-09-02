/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include <iostream>

// print
void print_auth(std::string username, std::string nickname, int fd);

/*
    Destructor
*/
Client::~Client() {}

/*
    Constructor
*/
Client::Client(int fd) : 
_fd(fd), _user_auth_tmp(0), _nick_auth_tmp(0), _pass_auth(0), _authenticated(0), _auth_level(0)
{}

/*
    get_nickname
*/
std::string Client::getNickName()
{
    return _nickname;
}

/*
    get_nick_authentification
*/
std::string Client::getNickAuth()
{
    return _nick_auth;
}

/*
    get_nick_authentification
*/
int Client::getNickAuthTmp()
{
    return _nick_auth_tmp;
}

/*
    get_user_authentification
*/
std::string Client::getUserAuth()
{
    return _user_auth;
}

/*
    get_user_authentification
*/
int Client::getUserAuthTmp()
{
    return _user_auth_tmp;
}

/*
    get_pass_auth
*/
int Client::getPassAuth()
{
    return _pass_auth;
}

/*
    get_username
*/
std::string Client::getUserName()
{
    return _username;
}

/*
    get_authenticated
*/
bool Client::getAuthenticated()
{
    return _authenticated;
}

/*
    get_authentification_level
*/
size_t &Client::getAuthLevel()
{ 
    return this->_auth_level;
}

/*
    get_file_descriptor
*/
int Client::getFd()
{
    return _fd;
}

/*
    get_buffer
*/
std::string &Client::getBuffer()
{
    return _buffer;
}

/*
    set_nickname
*/
void Client::setNickName(std::string nickname)
{
    _nickname = nickname;
}

/*
    set_username
*/
void Client::setUserName(std::string username)
{
    _username = username;
}

/*
    set_authenticated
*/
void Client::setAuthenticated(bool authenticated)
{
    _authenticated = authenticated;
    if (authenticated == true)
        print_auth(_username, _nickname, _fd);
}

/*
    set_authentification_level
*/
void   Client::setAuthLevel(size_t level)
{
    _auth_level = level;
}

/*
    set_file_descriptor
*/
void Client::setfd(int fd)
{
    _fd = fd;
}

/*
    set_password_authentification
*/
void Client::setPassAuth(int i)
{
    _pass_auth = i;
}

/*
    set_user_authentification
*/
void Client::setUserAuth(std::string str)
{
    _user_auth = str;
}

/*
    set_nick_authentification
*/
void Client::setNickAuth(std::string str) 
{
    _nick_auth = str; 
}

/*
    set_user_authentification_temporary
*/
void Client::setUserAuthTmp(int i)
{ 
    _user_auth_tmp = i; 
}

/*
    set_nick_authentification_temporary
*/
void Client::setNickAuthTmp(int i) 
{ 
    _nick_auth_tmp = i;
}

/*
    set_buffer
*/
void Client::setBuffer(std::string buffer)
{
    _buffer = buffer;
}

/*
    add_channel
*/
void Client::addChannel(Channel* chan)
{
    if (_channels.find(chan->getName()) == _channels.end())
        _channels.insert(std::make_pair(chan->getName(), chan));
}

/*
    add_channel_invited
*/
void Client::addChannelInv(Channel* chan)
{
    if (_channels_inv.find(chan->getName()) == _channels_inv.end())
        _channels_inv.insert(std::make_pair(chan->getName(), chan));
}

/*
    delete_channel_invited
*/
void    Client::delInvChan(Channel *channel)
{
    try 
    {
        this->_channels_inv.at(channel->getName());
        this->_channels_inv.erase(channel->getName());
    } 
    catch (std::exception &e)
    {
        return ;
    }
}

/*
    leave_all_channels
*/
void Client::leaveAllChannels()
{
    std::map<std::string, Channel*> chans_copy = this->_channels;
    std::map<std::string, Channel*>::iterator it;

    for (it = chans_copy.begin(); it != chans_copy.end(); ++it)
    {
        Channel *chan = it->second;
        if (chan != NULL)
            chan->delMember(this);
    }
    this->_channels.clear();
}

/*
    delete_channel
*/
void Client::delChannel(Channel* chan)
{
    try 
    {
        this->_channels.at(chan->getName());
        this->_channels.erase(chan->getName());
    } 
    catch (std::exception &e)
    {
        return ;
    }
}

/*
    is_invited
*/
bool Client::isInvited(Channel* channel)
{
    if (_channels_inv.find(channel->getName()) == _channels.end())
        return (0);
    return (1);
}
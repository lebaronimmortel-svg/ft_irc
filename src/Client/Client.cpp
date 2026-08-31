#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include <iostream>

Client::Client() {}

Client::~Client() {}

Client::Client(int fd) : 
_authenticated(0), _fd(fd), _user_auth_string(0), _nick_auth_string(0), _pass_auth(0), authLevel(0)
{}

std::string Client::getNickName()
{
    return _nickname;
}

std::string Client::getFullName()
{
    return _fullname;
}

std::string Client::getUserName()
{
    return _username;
}

bool Client::getAuthenticated()
{
    return _authenticated;
}

size_t &Client::getAuthLevel(){ return this->authLevel;}

int Client::getFd()
{
    return _fd;
}

std::string &Client::getBuffer()
{
    return _buffer;
}

void Client::setNickName(std::string nickname)
{
    _nickname = nickname;
}

void Client::setFullName(std::string fullname)
{
    _fullname = fullname;
}

void Client::setUserName(std::string username)
{
    _username = username;
}

static void print_auth(std::string username, std::string nickname, int fd)
{
    std::cout << std::endl << BLUE << "╔══════════════════════════╗" << RESET << std::endl;
    std::cout << BLUE << "║ New client joined server ║" << std::endl;
    std::cout << BLUE << "╚══════════════════════════╝" << RESET << std::endl;
    std ::cout << BLUE << "username: " << RESET << username << std::endl << BLUE << "nickname: " << RESET << nickname << std::endl;
    std::cout << BLUE << "fil_desc: " << RESET << fd << std::endl << std::endl;    
}

void Client::setAuthenticated(bool authenticated)
{
    _authenticated = authenticated;
    if (authenticated == true)
        print_auth(_username, _nickname, _fd);
}

void   Client::setAuthLevel(size_t level)
{
    authLevel = level;
}

void Client::setFd(int fd)
{
    _fd = fd;
}

void Client::setBuffer(std::string buffer)
{
    _buffer = buffer;
}

void Client::addChannel(Channel* chan)
{
    if (_channels.find(chan->getName()) == _channels.end())
    {
        _channels.insert(std::make_pair(chan->getName(), chan));
    }
}

void Client::leaveAllChannels()
{
    std::map<std::string, Channel*> chans_copy = this->_channels;
    std::map<std::string, Channel*>::iterator it;

    for (it = chans_copy.begin(); it != chans_copy.end(); ++it)
    {
        Channel *chan = it->second;
        if (chan != NULL)
        {
            chan->delMember(this);
        }
    }
    this->_channels.clear();
}

void Client::addChannelInv(Channel* chan)
{
    if (_channels_inv.find(chan->getName()) == _channels_inv.end())
        _channels_inv.insert(std::make_pair(chan->getName(), chan));
}

void Client::delChannel(Channel* chan)
{
    try {
        this->_channels.at(chan->getName());
        this->_channels.erase(chan->getName());
    } catch (std::exception &e){
        return ;
    }
}

int     Client::is_invited(Channel* channel)
{
    if (_channels_inv.find(channel->getName()) == _channels.end())
        return (0);
    return (1);
}

void    Client::delInvChan(Channel *channel)
{
    try {
        this->_channels_inv.at(channel->getName());
        this->_channels_inv.erase(channel->getName());
    } catch (std::exception &e){
        return ;
    }
}
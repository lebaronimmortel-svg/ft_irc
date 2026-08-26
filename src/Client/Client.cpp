#include "../../includes/Header.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(std::string nick, std::string user, std::string full, bool auth, int fd) : 
_nickname(nick), _username(user), _fullname(full), _authenticated(auth), _fd(fd), authLevel(0){}

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

void Client::setAuthenticated(bool authenticated)
{
    _authenticated = authenticated;
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
        std::cout << _username << " joined channel " << chan->getName() << std::endl;
    }
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
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

std::string Client::getBuffer()
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

int Client::is_chan_member()
{
    if (_channels.empty())
        return (0);
    return (1);
}

void Client::addChannel(Channel* chan)
{
    /// if (std::find(<chan->getName(), chan>) != _channels.end())
     _channels.insert(std::make_pair(chan->getName(), chan));
}
#include "../../includes/Client.hpp"

Client::Client() {}

Client::~Client {}

Client::getNickName()
{
    return _nickname;
}

Client::getFullName()
{
    return _fullname;
}

Client::getUserName()
{
    return _username;
}

Client::getAuthenticated()
{
    return _authenticated;
}

Client::getFd()
{
    return _fd;
}

Client::getBuffer()
{
    return _buffer;
}

Client::setNickName(std::string nickname)
{
    _nickname = nickname;
}

Client::setFullName(std::string fullname)
{
    _fullname = fullname;
}

Client::setUserName(std::string username)
{
    _username = username;
}

Client::setAuthenticated(bool authenticated)
{
    _authenticated = authenticated;
}

Client::setFd(int fd)
{
    _fd = fd;
}

Client::setBuffer(std::string buffer)
{
    _buffer = buffer;
}
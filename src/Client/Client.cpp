#include "../../includes/Header.hpp"

Client::Client() {}

Client::~Client() {}

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
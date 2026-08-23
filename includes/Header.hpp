#pragma once

#include <map>
#include <iostream>
#include <utility>
#include <stdlib.h>
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

// Utils/Parser.cpp
int parse_arg(char *arg);
std::string chan_join_cmd(char *buf);
std::string nick_cmd(char *buf);
std::string parse_auth(std::string str, int mode);
std::string parse_cmd(char *buf);
int ft_strncmp(const char *s1, const char *s2, int len);
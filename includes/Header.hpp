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
std::string kick_cmd(char *buf, int mode);
std::string inv_cmd(char *buf, int mode);
std::string names_cmd(char *buf, int mode);
std::string prv_chan_cmd(char *buf, int mode);
std::string prv_clt_cmd(char *buf, int mode);
int ft_strncmp(const char *s1, const char *s2, int len);
std::string topic_cmd(char *buf, int mode);
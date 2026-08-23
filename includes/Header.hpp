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
int chan_join_cmd(char *buf);
std::string parse_auth(std::string str, int mode);

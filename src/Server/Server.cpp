/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:05:09 by tseche            #+#    #+#             */
/*   Updated: 2026/08/26 16:12:06 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"
#include <cstring>

// parsing
std::string cmdSfxRef(std::string& str);
std::string cmdPfx(std::string str);

// print
void print_channel_deleted(std::map<std::string, Channel*>::iterator it);

/*
	reply_flag_value

		List of IRC norm based
		error codes
*/
const std::string reply_flag_value[ERR_NOTREGISTERED + 2] = 
{
	/* RPL_WELCOME */ "001",
	/*RPL_WELCOME*/"001",
	/*RPL_YOURHOST*/"002",
	/*RPL_CREATED*/"003",
	/*RPL_MYINFO*/"004",
	/*RPL_UMODEIS*/"221",
	/*RPL_CHANNELMODEIS*/"324",
	/*RPL_NOTOPIC*/"331",
	/*RPL_TOPIC*/"332",
	/*RPL_TOPICWHOTIME*/"333", 
	/*RPL_INVITING*/"341",
	/*RPL_NAMREPLY*/"353",
	/*RPL_ENDOFNAMES*/"366",
	/*ERR_NOSUCHNICK*/"401",
	/*ERR_NOSUCHCHANNEL*/"403",
	/*ERR_CANNOTSENDTOCHAN*/"404",
	/*ERR_TOOMANYCHANNELS*/"405",
	/*ERR_NORECIPIENT*/"411",
	/*ERR_NOTEXTTOSEND*/"412",
	/*ERR_UNKNOWNCOMMAND*/"421",
	/*ERR_NONICKNAMEGIVEN*/"431",
	/*ERR_ERRONEUSNICKNAME*/"432",
	/*ERR_NICKNAMEINUSE*/"433",
	/*ERR_USERNOTINCHANNEL*/"441",
	/*ERR_NOTONCHANNEL*/"442", 
	/*ERR_USERONCHANNEL*/"443",
	/*ERR_NEEDMOREPARAMS*/"461",
	/*ERR_ALREADYREGISTRED*/"462",
	/*ERR_PASSWDMISMATCH*/"464", 
	/*ERR_CHANNELISFULL*/"471",
	/*ERR_UNKNOWNMODE*/"472",
	/*ERR_INVITEONLYCHAN*/"473",
	/*ERR_BADCHANNELKEY*/"475",
	/*ERR_NOPRIVILEGES*/"481",
	/*ERR_CHANOPRIVSNEEDED*/"482",
	/*ERR_UMODEUNKNOWNFLAG*/"501",
	/*ERR_USERSDONTMATCH*/"502",
	/*ERR_NOTREGISTERED*/"451",
};

/*
	Commands

		List of commands
		and their corresponding
		server functions
*/
cmdlist cmdLU[] = 
{
    /*[INVITE]*/
	{
        .name = "INVITE",
        .call = &Server::invite,
    },
    /*[KICK]*/
	{
        .name = "KICK",
        .call =  &Server::kick,
    },
    /*[TOPIC]*/
	{
        .name = "TOPIC",
        .call =  &Server::topic,
    },
    /*[MODE]*/
	{
        .name = "MODE",
        .call =  &Server::mode,
    },
    /*[PASS]*/
	{
        .name = "PASS",
        .call =  &Server::pass,
    },
    /*[NICK]*/
	{
        .name = "NICK",
        .call =  &Server::nick,
    },
    /*[NAME]*/
	{
        .name = "USER",
        .call =  &Server::user,
    },
    /*[JOIN]*/
	{
        .name = "JOIN",
        .call =  &Server::join,
    },
    /*[PRIVMSG]*/
	{
        .name = "PRIVMSG",
        .call =  &Server::privmsg,
    },
};

/*
	Constructor
*/
Server::Server(int port, std::string pass): _password(pass)
{
	this->_servsock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_servsock < 0)
		throw std::runtime_error("Error Initialization socket");
	this->_servaddr = (struct sockaddr_in){
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {
			.s_addr = INADDR_ANY,
		}
	};

	if (bind(this->_servsock, (struct sockaddr *)&this->_servaddr, sizeof(this->_servaddr)) == -1){
		close(this->_servsock);
		throw std::runtime_error("Error binding server socket");
	}
	if (listen(this->_servsock, SOMAXCONN) == -1){
		close(this->_servsock);
		throw std::runtime_error("Error setting up listen limit");
	}
	this->_epollfdserv = epoll_create1(O_CLOEXEC);
	if (this->_epollfdserv < 0){
		close(this->_epollfdserv);
		throw std::runtime_error("Error Initialization Epoll");
	}
	struct epoll_event serv_event = {
		.events = EPOLLIN,
		.data = {
			.fd = this->_servsock,
		}
	};
	if (epoll_ctl(this->_epollfdserv, EPOLL_CTL_ADD, this->_servsock, &serv_event) < 0){
		close(this->_servsock);
		close(this->_epollfdserv);
		throw std::runtime_error("Error adding event to listen queue");
	}
	this->_epollfd.push_back(this->_epollfdserv);
}

/*
	Destructor
*/
Server::~Server()
{
	size_t size = this->_epollfd.size();
	for (size_t i = 0; i < size; i++)
		close(this->_epollfd[i]);
}

/*
	get_client
	(by nickname or
	by file descriptor)
*/
Client* Server::getClient(std::string nickname, int fd, int mode)
{
	for (std::map<int, Client*>::iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if (mode == 0 && (i->second->getNickName() == nickname || i->second->getNickAuth() == nickname))
			return (i->second);
		if (mode == 1 && i->first == fd)
			return (i->second);
		if (mode == 2 && (i->second->getNickName() == nickname))
			return (i->second);
	}
	return (NULL);
}

/*
	add_client
*/
void Server::addClient(int client_fd)
{
	Client *client = new Client(client_fd);
	_clients.insert(std::make_pair(client_fd ,client));
}

/*
	remove_client
*/
void	Server::removeClient(int fd)
{
	_clients.erase(fd);
}

/*
	get_channels_list
*/
std::map<std::string, Channel *> &Server::getChannelList()
{
	return this->_channels;
}

/*
	add_channel_name
*/
void Server::addChannelName(Channel *chan)
{
	this->_channels[chan->getName()] = chan;
}

/*
	get_command
*/
cmdfunc Server::getcmd(std::string str)
{
	size_t lenght = str.length();
	if (lenght == 0)
		return (NULL);
	size_t i = 0;
	for (; i < lenght && isblank(str[i]); i++);
	if (i == lenght)
		return (NULL);
	int slash = i;
	size_t sep = str.find(slash, ' ');
	if (sep == str.npos)
		sep = str.length();
	std::string cmd = str.substr(slash, sep);
	for (int i = 0; i < PRIVMSG + 1; i++){
		if (cmdPfx(cmd) ==  cmdLU[i].name)
			return cmdLU[i].call;
	}
	return (NULL);
}

/*
	call_command
*/
int Server::callcmd(std::string str, Client &c)
{
	cmdfunc func = this->getcmd(str);
	if (!func)
	{
		if (c.getAuthenticated() == 1)
			this->reply(&c, ERR_UNKNOWNCOMMAND, "unknown command");
		return 0;
	}
	size_t i = str.find(' ');
	if (i == str.npos)
		i = str.length();
	(this->*func)(str, i, c);
	return 1;
}

/*
	get_arguments_parsed
*/
std::vector<std::string> Server::getArgsparse(std::string str, char sep)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream ss(str);

    while (std::getline(ss, token, sep))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

/*
	get_channel_parsed
*/
Channel *Server::getChannelparse(std::string &str, size_t i)
{
    size_t len_str = str.length();

    while (i < len_str && str[i] == ' ')
        i++;

    if (i >= len_str || str[i] != '#')
        return (NULL);

    size_t end = str.find(' ', i);
    if (end == std::string::npos)
        end = len_str;

    std::string sub = str.substr(i, end - i);
    i = end;

    std::map<std::string, Channel *> &lst = this->getChannelList();
    std::map<std::string, Channel *>::iterator it = lst.find(sub);

    if (it != lst.end())
        return (it->second);

    return (NULL);
}

/*
	get_channels_list_parsed
*/
std::vector<Channel *> *Server::getChannelListparse(Client *c, std::string &str, size_t &i)
{
    (void) i;
    std::string params = cmdSfxRef(str);
    
    size_t space_pos = params.find(' ');
    std::string chan_list_str = (space_pos != std::string::npos) ? params.substr(0, space_pos) : params;

    std::vector<std::string> chan_names = this->getArgsparse(chan_list_str, ',');
    std::vector<Channel *> *chanvec = new std::vector<Channel *>();

    for (size_t y = 0; y < chan_names.size(); ++y)
    {
        std::string chan_name = chan_names[y];

        size_t crlf = chan_name.find_first_of("\r\n");
        if (crlf != std::string::npos)
            chan_name = chan_name.substr(0, crlf);

        if (chan_name.empty() || chan_name[0] != '#')
            this->reply(c, ERR_NOSUCHCHANNEL, chan_name + " :No such channel");

        std::map<std::string, Channel *>::iterator it = this->_channels.find(chan_name);
        if (it != this->_channels.end())
            chanvec->push_back(it->second);
        else
            chanvec->push_back(NULL);
    }

    return chanvec;
}

/*
	get_channel
*/
Channel* Server::getChannel(std::string name)
{
    try 
    {
        Channel* chan = _channels.at(name);
        return (chan);
    }
    catch (std::exception &e)
    {
        return (NULL);
    }
}

/*
	get_epoll_fd
*/
int Server::getEpollFd()
{
	return this->_epollfdserv;
};

/*
	get_socket
*/
int Server::getSocket() 
{
	return this->_servsock;
};

/*
	get_fd_list
*/
std::vector<int> &Server::getfdList()
{
	return this->_epollfd;
};

sockaddr_in Server::getAddress() 
{
	return this->_servaddr;
};

/*
	clean
	(closes empty channels)
*/
void Server::clean()
{
    std::map<std::string, Channel*>::iterator it = _channels.begin();

    while (it != _channels.end())
    {
        Channel *chan = it->second;
        if (chan != NULL && chan->getMembers().empty())
        {
			print_channel_deleted(it);
            _channels.erase(it++);
        }
        else
            ++it;
    }
}

/*
	reply
*/
void Server::reply(Client *c, reply_flag flag, std::string msg)
{
	std::string name = (c->getNickName().empty()) ? "*" : c->getNickName();
	std::string message = ":ircserv " + reply_flag_value[flag] + " " + name
	 + " " + msg + "\r\n";
	send (c->getFd(), message.c_str(), message.size(), 0);
}

/*
	reply_channel
*/
void Server::replyChannel(Channel *chan, std::string msg)
{
	msg += "\r\n";
	std::map<std::string, Client *> mem = chan->getMembers();
	std::map<std::string, Client *>::iterator end = mem.end();
	for (std::map<std::string, Client *>::iterator it = mem.begin(); it != end; it++)
		send((*it).second->getFd(), msg.c_str(), msg.size(), 0);
}

/*
	handle_client
*/
void Server::HandleClient(Client *c)
{
	std::string &buff = c->getBuffer();
	size_t pos;
	while ((pos = buff.find("\r\n")) != buff.npos)
	{
		std::string line = buff.substr(0, pos);
		buff.erase(0, pos + 2);
		this->callcmd(line, *c);
	}
}
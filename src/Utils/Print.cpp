#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/*
    print <event> :

    These functions are meant to
	output a log of occuring
	events in the server 
*/

void print_header()
{
	std::cout << std::endl << GREEN BOLD << "			╔════════════════════════════╗" << std::endl;
	std::cout << "			║ FT_IRC    tseche, alexfuen ║" << std::endl;
	std::cout << "			╚════════════════════════════╝" << RESET << std::endl;
}

void print_channel_deleted(std::map<std::string, Channel*>::iterator it)
{
	std::cout << std::endl << BLUE << "╔═════════════════╗" << RESET << std::endl;
	std::cout << BLUE << "║ Channel deleted ║" << std::endl;
	std::cout << BLUE << "╚═════════════════╝" << RESET << std::endl;
	std::cout << BLUE << "channel: " << RESET << it->first << std::endl << std::endl;	
}

void print_client_quit(std::string user, std::string nick, int fd)
{
	std::cout << std::endl << BLUE << "╔════════════════════╗" << RESET << std::endl;
	std::cout << BLUE << "║ Client quit server ║" << std::endl;
	std::cout << BLUE << "╚════════════════════╝" << RESET << std::endl;
	std::cout << BLUE << "username: " << RESET << user << std::endl;
	std::cout << BLUE << "nickname: " << RESET << nick << std::endl;
	std::cout << BLUE << "fil_desc: " << RESET << fd << std::endl << std::endl;	
}

void print_new_connection(int client_fd)
{
	std::cout << std::endl << BLUE << "╔═════════════════════════╗" << RESET << std::endl;
	std::cout << BLUE << "║ New connection received ║" << std::endl;
	std::cout << BLUE << "╚═════════════════════════╝" << RESET << std::endl;
	std::cout << BLUE << "fil_desc: " << RESET << client_fd << std::endl << std::endl;	
}

void print_new_channel(Client& c, Channel* nchan)
{
	std::cout << std::endl << BLUE << "╔════════════════════════════╗" << RESET << std::endl;
	std::cout << BLUE << "║ Client created new channel ║" << std::endl;
	std::cout << BLUE << "╚════════════════════════════╝" << RESET << std::endl;
	std::cout << BLUE << "nickname: " << RESET << c.getNickName() << std::endl;
	std::cout << BLUE << "created: " << RESET << nchan->getName() << std::endl << std::endl;	
}

void print_join_channel(Client& c, Channel* chan)
{
	std::cout << std::endl << BLUE << "╔═══════════════════════════╗" << RESET << std::endl;
	std::cout << BLUE << "║ Client joined new channel ║" << std::endl;
	std::cout << BLUE << "╚═══════════════════════════╝" << RESET << std::endl;
	std::cout << BLUE << "nickname: " << RESET << c.getNickName() << std::endl;
	std::cout << BLUE << "joined: " << RESET << chan->getName() << std::endl << std::endl;	
}
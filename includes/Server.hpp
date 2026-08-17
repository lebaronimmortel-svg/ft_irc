#pragma once

#include "Header.hpp"

class Server
{
    private :

        // channels
        std::map<std::string, Channel*> _channels;
        
        // password for the channel
        std::string _password;

    public :

        Server();
        ~Server();

        // getters
        Channel* getChannel();
        std::string getPassword();

        // setters
        void    setPassword(std::string password);
};

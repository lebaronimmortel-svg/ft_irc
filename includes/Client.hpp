#pragma once

#include "Header.hpp"

class Client
{
    private :
    
    	// identifier in the server
        std::string _nickname;
        
        // used to authenticate
        std::string _username;
        std::string _fullname;
        
        // status in the server 
        bool _authenticated;

        // where to read/write
        int _fd;

        // stores current command
        std::string _buffer;

    public :

        // constructor/destructor
        Client();
        ~Client();

        // getters
        std::string getNickName();
        std::string getFullName();
        std::string getUserName();
        bool        getAuthenticated();
        int         getFd();
        std::string getBuffer();

        // setters
        void        setNickName(std::string nickname);
        void        setFullName(std::string fullname);
        void        setUserName(std::string username);
        void        setAuthenticated(bool authenticated); 
        void        setFd(int fd);
        void        setBuffer(std::string);
};

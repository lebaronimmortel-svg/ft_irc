#pragma once

#include "Channel.hpp"

enum authentification{
    PASSWORD,
    USERNAME,
    NICKNAME,
};


class Client
{
    private :
    
        std::map<std::string, Channel*> _channels;
        std::map<std::string, Channel*> _channels_inv;

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

        size_t   authLevel;

    public :

        // constructor/destructor
        Client();
        ~Client();
        Client(int fd);

        // getters
        std::string getNickName();
        std::string getFullName();
        std::string getUserName();
        bool        getAuthenticated();
        int         getFd();
        std::string &getBuffer();
        size_t      &getAuthLevel();

        // setters
        void        setNickName(std::string nickname);
        void        setFullName(std::string fullname);
        void        setUserName(std::string username);
        void        setAuthenticated(bool authenticated); 
        void        setFd(int fd);
        void        setAuthLevel(size_t level);
        void        setBuffer(std::string);

        //int     is_chan_member();
        void    addChannel(Channel* channel);
        void    addChannelInv(Channel* channel);
        void    delChannel(Channel* channel);
        int     is_invited(Channel* channel);
        void    delInvChan(Channel *channel);
};

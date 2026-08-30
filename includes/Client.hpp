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
        std::string _user_auth;
        std::string _nick_auth;
        
        // status in the server 
        bool _authenticated;

        // where to read/write
        int _fd;
        int _user_auth_string;
        int _nick_auth_string;

        // stores current command
        std::string _buffer;

        int _pass_auth;

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
        const std::map<std::string, Channel*>& getChannels() const { return _channels; }
        int getPassAuth() { return _pass_auth; }
        int getUserAuthString() { return _user_auth_string; }
        int getNickAuthString() { return _nick_auth_string; }

        std::string    getUserAuth() { return _user_auth; }
        std::string    getNickAuth() { return _nick_auth; }

        // setters
        void        setNickName(std::string nickname);
        void        setFullName(std::string fullname);
        void        setUserName(std::string username);
        void        setAuthenticated(bool authenticated); 
        void        setFd(int fd);
        void        setAuthLevel(size_t level);
        void        setBuffer(std::string);
        void        setPassAuth(int i) { _pass_auth = i; }
        void        setUserAuthString(int i) { _user_auth_string = i; }
        void        setNickAuthString(int i) { _nick_auth_string = i; }

        //int     is_chan_member();
        void    addChannel(Channel* channel);
        void    addChannelInv(Channel* channel);
        void    delChannel(Channel* channel);
        int     is_invited(Channel* channel);
        void    delInvChan(Channel *channel);
        void    leaveAllChannels();

        void    setUserAuth(std::string str) { _user_auth = str; }
        void    setNickAuth(std::string str) { _nick_auth = str; }
};

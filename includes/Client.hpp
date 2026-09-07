/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Channel.hpp"

enum authentification
{
    PASSWORD,
    USERNAME,
    NICKNAME,
};

class Client
{
    private :
    
        // channels registered in
        std::map<std::string, Channel*> _channels;
        std::map<std::string, Channel*> _channels_inv;

        // identifier
        int                             _fd;

        // used to authenticate
        std::string                     _nickname;
        std::string                     _username;
        std::string                     _user_auth;
        std::string                     _nick_auth;
        bool                            _user_auth_tmp;
        bool                            _nick_auth_tmp;
        bool                            _pass_auth;
        bool                            _authenticated;
        size_t                          _auth_level;

        // stores current command
        std::string                     _buffer;

    public :

        // constructor
        Client(int fd);

        // destructor
        ~Client();

        // getters
        std::map
        <std::string, Channel*>&        getChannels();
        std::string                     getNickName();
        std::string                     getFullName();
        std::string                     getUserName();
        bool                            getAuthenticated();
        int                             getFd();
        std::string                     &getBuffer();
        size_t                          &getAuthLevel();
        int                             getPassAuth();
        int                             getUserAuthTmp();
        int                             getNickAuthTmp();
        std::string                     getUserAuth();
        std::string                     getNickAuth();
  
        // setters
        void                            setNickName(std::string nickname);
        void                            setFullName(std::string fullname);
        void                            setUserName(std::string username);
        void                            setAuthenticated(bool authenticated); 
        void                            setfd(int fd);
        void                            setAuthLevel(size_t level);
        void                            setBuffer(std::string);
        void                            setPassAuth(int i);
        void                            setUserAuthTmp(int i);
        void                            setNickAuthTmp(int i);
        void                            setUserAuth(std::string str);
        void                            setNickAuth(std::string str);

        // utils
        void                            addChannel(Channel* channel);
        void                            addChannelInv(Channel* channel);
        void                            delChannel(Channel* channel);
        void                            delInvChan(Channel *channel);
        bool                            isInvited(Channel* channel);
        void                            leaveAllChannels();
};

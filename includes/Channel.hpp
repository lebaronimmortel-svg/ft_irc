/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
class Client;

class Channel
{
    private :

        // stores clients of the channel
        std::map<std::string, Client*> _members;
        std::map<std::string, Client*> _moderators;
        std::map<std::string, Client*> _invited;

        // channel name and password
        std::string                    _name;
        std::string                    _password;

        // channel modes
        bool                           _requires_password;
        bool                           _invite_only_mod;
        bool                           _topic_restriction;

        // topic-related infos 
        std::string                    _topic;
        std::string                    _topic_last_modif_date;
        std::string                    _topic_last_modifier_username;

        // max number of users allowed (0 == no limit)
        unsigned int _user_limit;

    public :

        // constructor
        Channel(std::string name);

        // destructor
        ~Channel();

        // getters
        std::string  getName();
        std::string  getPassword();
        Client*      getMember(std::string username);
        std::map<std::string, Client*> getMembers();
        Client*      getModerator(std::string username);
        Client*      getInvited(std::string username);
        bool         getPasswordRequirement();
        bool         getInviteOnlyStatus();
        bool         getTopicRestrictionStatus();
        std::string  getTopic();
        std::string  getTopicLastModifDate();
        std::string  getTopicLastModifierUsername();
        unsigned int getUserLimit();
        std::string  getModesString();

        // setters
        void         setName(std::string name);
        void         setPassword(std::string password);
        void         setPasswordRequirement(bool status);
        void         setInviteOnlyStatus(bool status);
        void         setTopicRestrictionStatus(bool status);
        void         setTopic(std::string topic);
        void         setTopicLastModifDate(std::string date);
        void         setTopicLastModifierUsername(std::string username);
        void         setUserLimit(unsigned int limit);

        // utils
        void         addUser(Client *client);
        void         addModerator(Client *);
        void         addInvited(Client *);
        void         delModerator(Client *);
        void         delMember(Client *);
        void         delInvited(Client *);
        void         broadcast(std::string msg, int fd);
};


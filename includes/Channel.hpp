#pragma once

#include "Header.hpp"

class Client;

class Channel
{
    private :

        // stores clients of the channel
        std::map<std::string, Client*> _members;
        std::map<std::string, Client*> _moderators;
        std::map<std::string, Client*> _invited;

        // channel name and password
        std::string _name;
        std::string _password;

        // channel modes
        bool _requires_password;
        bool _invite_only_mod;
        bool _topic_restriction;

        // topic-related infos 
        std::string _topic;
        std::string _topic_last_modif_date;
        std::string _topic_last_modifier_username;

        // max number of users allowed (0 == no limit)
        unsigned int _user_limit;

    public :

        // constructor/destructor
        Channel();
        ~Channel();

        // getters
        std::string  getName();
        std::string  getPassword();
        Client*      getMember(std::string username);
        Client*      getModerator(std::string username);
        Client*      getInvited(std::string username);
        bool         getPasswordRequirement();
        bool         getInviteOnlyStatus();
        bool         getTopicRestrictionStatus();
        std::string  getTopic();
        std::string  getTopicLastModifDate();
        std::string  getTopicLastModifierUsername();
        unsigned int getUserLimit();

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

        std::string belongs_to_channel(int fd);
};


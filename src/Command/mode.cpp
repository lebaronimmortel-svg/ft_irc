#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"
#include <cstring>
#include <sstream>

// parsing
std::string cmdSfxRefWord(std::string& str);
int nb_first_sp(std::string str);

/*
    get_flag_mode

        This function is meant to 
        extract from a string and 
        store into a dedicated 
        structure modes and targets 
        on which it should be applied  
*/
mode_s *Server::getFlagMode(Channel *chan, Client *c, const std::string &params)
{

    /*
        Initializing 
        storing
        structure
    */
    mode_s *args = new mode_s();
    args->flag.i = -1;
    args->flag.t = -1;
    args->flag.k = -1;
    args->flag.o = -1;
    args->flag.l = -1;

    /*
        Initializing 
        browsing
        parameters
    */
    size_t length = params.length();
    short state = 1;
    size_t i = 0;
    bool has_sign = false;
    std::string order = "";

    /*
        Skipping beginning
        whitespaces
    */
    for (; i < length && params[i] == ' '; i++);

    /*
        Parsing part of
        the input that
        contains flags,
        storing flags
        implying a target
    */
    for (; i < length; i++)
    {
        if (params[i] == ' ')
            break;

        if (params[i] == '+')
        {
            state = 1;
            has_sign = true;
        }
        else if (params[i] == '-')
        {
            state = 0;
            has_sign = true;
        }
        else if (strchr("itkol", params[i]) == NULL)
            this->reply(c, ERR_UNKNOWNMODE, std::string(1, params[i]) + " :is unknown mode char to me");
        else 
        {
            char flag = params[i];
            switch (flag)
            {
                case 'i':
                    args->flag.i = state;
                    break;
                case 't':
                    args->flag.t = state;
                    break;
                case 'k':
                    args->flag.k = state;
                    if (state == 1)
                        order.push_back('k');
                    break;
                case 'o':
                    args->flag.o = state;
                    order.push_back('o');
                    break;
                case 'l':
                    args->flag.l = state;
                    if (state == 1)
                        order.push_back('l');
                    break;
            }
        }
    }

    if (!has_sign && order.empty() && args->flag.i == -1 && args->flag.t == -1)
    {
        delete args;
        this->reply(c, ERR_NEEDMOREPARAMS, chan->getName() + " :Not enough parameters");
        return NULL;
    }
    
    /*
        Retrieving targets
    */
    std::vector<std::string> wvec;
    std::istringstream iss(params.substr(i));
    std::string word;
    while (iss >> word)
        wvec.push_back(word);

    /*
        Attributing each 
        mode that implies
        a target to it
    */
    size_t vec_i = 0;
    for (size_t y = 0; y < order.length(); y++)
    {
        if (vec_i >= wvec.size())
            break;

        char current = order[y];
        if (current == 'k')
            args->value.k = wvec.at(vec_i++);
        else if (current == 'o')
            args->value.o.push_back(wvec.at(vec_i++));
        else if (current == 'l')
        {
            std::istringstream is(wvec.at(vec_i++));
            is >> args->value.l;
            if (is.fail() || !is.eof())
                this->reply(c, ERR_UNKNOWNMODE, chan->getName() + " :invalid limit value");
        }
    }
    return (args);
}

/*
    mode

    This function is meant to
    execute the MODE commande
    from an IRC operator:

    MODE #channel +t-i+oo user1 user2
*/
void Server::mode(std::string &str, size_t &i, Client &c)
{
    if (!c.getAuthenticated())
    {
        this->reply(&c, ERR_NOTREGISTERED, "IRCServer: requires to be registered");
        return;
    }

    /*
        Extracting targeted channel
        from provided input
    */
    size_t cpy = i;
	std::string names = cmdSfxRefWord(str);

    Channel *chan = this->getChannelparse(names, 0);

    if (chan == NULL)
    {
        this->reply(&c, ERR_NOSUCHCHANNEL, str.substr(cpy, i - cpy) + ": this channel doesn't exist");
        return;
    }

    /*
        Extracting targeted users
        from provided input
    */
    size_t param_start = names.size() + 5 + nb_first_sp(str); //
    if (param_start == std::string::npos)
    {
        this->reply(&c, RPL_CHANNELMODEIS, chan->getName() + ": no target provided");
        return;
    }

    if (chan->getModerator(c.getNickName()) == NULL)
    {
        this->reply(&c, ERR_CHANOPRIVSNEEDED, chan->getName() + ": You're not channel operator");
        return;
    }

    /*
        Extracting modes and 
        corresponding targets
        into a structure
    */
    mode_s *args = this->getFlagMode(chan, &c, str.substr(param_start)); 
    if (args == NULL)
        return;

    /*
        Channel-related
        modes
    */
    if (args->flag.i == 1)
        chan->setInviteOnlyStatus(true);

    else if (args->flag.i == 0)
        chan->setInviteOnlyStatus(false);

    if (args->flag.t == 1)
        chan->setTopicRestrictionStatus(true);

    else if (args->flag.t == 0)
        chan->setTopicRestrictionStatus(false);

    if (args->flag.k == 1)
    {
        chan->setPasswordRequirement(true);
        chan->setPassword(args->value.k);
    }

    else if (args->flag.k == 0)
    {
        chan->setPasswordRequirement(false);
        chan->setPassword("");
    }

    if (args->flag.l == 1)
        chan->setUserLimit(args->value.l);

    else if (args->flag.l == 0)
        chan->setUserLimit(0);

    /*
        User-related
        modes
    */
    if (args->flag.o == 1)
    {
        for (size_t idx = 0; idx < args->value.o.size(); idx++)
        {
            Client *client = chan->getMember(args->value.o.at(idx));
            if (client != NULL)
                chan->addModerator(client);
            else
                this->reply(&c, ERR_NOSUCHNICK, args->value.o.at(idx) + " :No such nick");
        }
    }

    else if (args->flag.o == 0)
    {
        for (size_t idx = 0; idx < args->value.o.size(); idx++)
        {
            Client *client = chan->getMember(args->value.o.at(idx));
            if (client != NULL)
            {
                if (chan->getModerator(args->value.o.at(idx)) != NULL)
                    chan->delModerator(client);
            }
            else
                this->reply(&c, ERR_NOSUCHNICK, args->value.o.at(idx) + " :No such nick");
        }
    }

    /*
        Broadcast
    */
    std::string broadcast_msg = ":" + c.getNickName() + "!" + c.getUserName() +
                                "@localhost " + str + "\r\n";
    chan->broadcast(broadcast_msg, -1);

    delete (args);
}
#include "../../includes/Header.hpp"

void unknown(char *buf, Client *client)
{
    std::string msg = "Error: Unknown command: ";

    for (int i = 0; i < ft_strlen(buf) - 2; i++)
        msg += buf[i];

    // envoyer msg à client

    (void) client; //temporary
}

void join(char *buf, Server* serv, Client* client)
{
    std::string cmd = chan_join_cmd(buf);
    if (!cmd.empty())
    {
        Channel *chan = serv->get_channel(cmd);
        if (!chan)
        {
            chan = new Channel(cmd);
            serv->addChannel(chan, cmd);
        }
        if (chan->getInviteOnlyStatus() == 0 || (chan->getInviteOnlyStatus() == 1 && client->is_invited(chan)))
        {
            chan->addUser(client);
            client->addChannel(chan);
            if (client->is_invited(chan))
                client->delInvChan(chan);
        }
    }
    else
        unknown(buf, client);
}

void    nick(char* buf, Client* client)
{
    std::string nick = nick_cmd(buf);
    if (!nick.empty())
        client->setNickName(nick);
    else
        unknown(buf, client);
}

std::string date() 
{
    std::time_t now = std::time(NULL);
    std::tm* structTime = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", structTime);

    return std::string(buffer);
}

void    topic(char *buf, Server* serv, Client* client)
{
    std::string chan = topic_cmd(buf, 0);
    std::string topic = topic_cmd(buf, 1);
    if (!chan.empty())
    {
        Channel *channel  = serv->get_channel(chan);

        if (!topic.empty())
        {
            if (channel)
            {
                channel->setTopic(topic);
                channel->setTopicLastModifierUsername(client->getUserName());
                channel->setTopicLastModifDate(date());
            }
            else
                std::cerr << "Error modifying " << chan << "'s topic: no such channel" << std::endl;
        }
        else
        {
            // envoyer message au client : channel->getTopic()
        }
    }
    else
        unknown(buf, client);
}

void    names(char *buf, Server* serv, Client* client)
{
    if (!ft_strncmp(buf, "NAMES #", 7))
    {
        std::string chan = names_cmd(buf, 1);
        if (!chan.empty())
        {
            Channel *channel  = serv->get_channel(chan);
            if (channel)
            {
                // envoyer message au client : noms des membres de channel
            }
            else
                std::cerr << "Error displaying channel " << chan << "'s members names: no such channel" << std::endl;
        }
        else
            unknown(buf, client);
    }
    else
    {
        std::string result = names_cmd(buf, 0);
        if (!result.empty())
        {
            // envoyer message au client : noms des membres de serv
        }
        else
            unknown(buf, client);
    }
}

void    prvmsg(char *buf, Server* serv, Client* client)
{
    if (!ft_strncmp("PRIVMSG #", buf, 9))
    {
        std::string chan = prv_chan_cmd(buf, 0);
        std::string msg =  prv_chan_cmd(buf, 1);

        if (!chan.empty())
        {
            Channel *channel  = serv->get_channel(chan);
            if (channel)
            {
                if (channel->getMember(client->getUserName()))
                {
                    // envoyer msg 'msg' aux membres du canal 'channel' de la part de 'client->getUserName()'
                }
                else
                    std::cerr << "Error sending private message to " << chan << "'s members: " << client->getUserName() << "isn't a member of the channel" << std::endl;
            }
            else
                std::cerr << "Error sending private message to " << chan << "'s members: no such channel" << std::endl;
        }

        else
            unknown(buf, client);
    }
    else
    {
        std::string user = prv_clt_cmd(buf, 0);
        std::string msg = prv_clt_cmd(buf, 1);

        if (!user.empty())
        {
            // envoyer msg 'msg' au client 'user' de la part de 'client->getUserName()'
        }
        else
            unknown(buf, client);
    }    
}

void    kick(char *buf, Server* serv, Client* client)
{
    std::string chan = kick_cmd(buf, 0);
    std::string user = kick_cmd(buf, 1);
    std::string reason = kick_cmd(buf, 2);

    if (!chan.empty())
    {
        Channel* channel = serv->get_channel(user);
        if (!channel)
            std::cerr << "Error while kicking " << user << " from channel " << chan << ": no such channel" << std::endl;
        else 
        {
            Client *clt = channel->getMember(user);
            if (!clt)
                std::cerr << "Error while kicking " << user << " from channel " << chan << ": no such user" << std::endl;
            else
            {
                channel->delUser(clt);
                clt->delChannel(channel);
            }
        }
        if (!reason.empty())
        {
            // envoyer msg 'reason' à user 
        }
    }
    else
        unknown(buf, client); 
}

void    invite(char *buf, Server* serv, Client* client)
{
    std::string chan = inv_cmd(buf, 0);
    std::string user = inv_cmd(buf, 1);

    if (!chan.empty())
    {
        Channel* channel = serv->get_channel(user);
        if (!channel)
            std::cerr << "Error while inviting " << user << " to channel " << chan << ": no such channel" << std::endl;
        else 
        {
            Client *clt = serv->get_client(user, 0, 0);
            if (!clt)
                std::cerr << "Error while inviting " << user << " to channel " << chan << ": no such user" << std::endl;
            else
                clt->addChannelInv(channel);
        }
    }    
}

void handle_cmd(char *buf, Server* serv, Client* client)               
{
    if (!ft_strncmp(buf, "JOIN ", 5))
        join(buf, serv, client);
    else if (!client->is_chan_member())
        std::cerr << "Error executing command: " << client->getUserName() << " belongs to no channel" << std::endl;
    else if (!ft_strncmp(buf, "NICK ", 5))
        nick(buf, client);
    else if (!ft_strncmp(buf, "TOPIC #", 7))
        topic(buf, serv, client);
    else if (!ft_strncmp(buf, "NAMES", 5))
        names(buf, serv, client);
    else if (!ft_strncmp("PRIVMSG ", buf, 8))
        prvmsg(buf, serv, client);
    else if (!ft_strncmp("KICK ", buf, 8))
        kick(buf, serv, client);
    else if (!ft_strncmp("INVITE ", buf, 7))
        invite(buf, serv, client);
    else
        unknown(buf, client);
}
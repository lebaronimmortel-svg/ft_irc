#include "../../includes/Header.hpp"

int ft_strlen(const char *str)
{
    unsigned int i = 0;

    while (str[i])
        i++;

    return (i);
}

int ft_strncmp(const char *s1, const char *s2, int len)
{
    if ((s1 && !s2) || (s2 && !s1))
        return (1);
    
    if (!s1 && !s2)
        return (0); 

    for (int i = 0; i < len; i++)
    {
        if (!s1[i] && !s2[i])
            return (0);
        if (s1[i] != s2[i])
            return (1);
    }
    return (0);
}

int parse_arg(char *arg)
{
    if (ft_strlen(arg) != 4)
        return (0);

    for (unsigned int i = 0; i < 4; i++)
    {
        if (!isdigit(arg[i]))
            return (0);
    }

    return (1);
}

static int is_valid_char(char c)
{
    if (c < 32 || c == 127)
        return (0);
    if (c == ',' || c == ':')
        return (0);
    return (1);
}

/*

JOIN #*********\r\n

*/
std::string chan_join_cmd(char *buf)
{
    std::string chan;

    if (ft_strncmp(buf, "JOIN #", 6) || ft_strlen(buf) < 9)
        return ("");
    int len = ft_strlen(buf);
    if (buf[len - 2] != '\r' || buf[len - 1] != '\n')
        return ("");
    for (int i = 6; i < len - 2; i++)
    {
        if (!is_valid_char(buf[i]))
            return ("");
        chan += buf[i];
    }
    return (chan);
}

std::string nick_cmd(char *buf)
{
    std::string nick;

    if (ft_strncmp(buf, "NICK ", 5) || ft_strlen(buf) < 8)
        return ("");
    int len = ft_strlen(buf);
    if (buf[len - 2] != '\r' || buf[len - 1] != '\n')
        return ("");
    for (int i = 5; i < len - 2; i++)
    {
        if (!is_valid_char(buf[i])) // même conditions que channel ?
            return ("");
        nick += buf[i];
    }
    return (nick);
}

std::string topic_cmd(char *buf, int mode)
{
    std::string channel = "";
    std::string topic = "";

    if (ft_strncmp(buf, "TOPIC #", 7) || ft_strlen(buf) < 10)
        return ("");

    int len = ft_strlen(buf);
    if (buf[len - 2] != '\r' || buf[len - 1] != '\n')
        return ("");
        
    int i = 7;
    while (i < len - 2)
    {
        if (buf[i] == ' ')
            break ;
        if (!is_valid_char(buf[i]))
            return ("");
        channel += buf[i++];
    }

    if (i < len - 2)
    {
        if (!ft_strncmp(&buf[i], " :", 2))
            return ("");
        i += 2;
        while (i < len - 2)
            topic += buf[i++];
    }

    if (mode == '0')
        return (channel);
    else
        return (topic); 
}

std::string names_cmd(char *buf, int mode)
{
    int length = ft_strlen(buf);

    if (ft_strncmp(buf, "NAMES", 5) || length < 7 || buf[length - 1] != '\n' || buf[length - 2] != '\r')
        return ("");
    
    std::string channel = "";

    if (mode == 1) // avec arg
    {
        if (length < 10 || ft_strncmp(buf, "NAMES #", 7))
            return ("");
        for (int i = 7; i < length - 2; i++)
        {
            if (!is_valid_char(buf[i]))
                return ("");
            channel += buf[i];
        }
        return (channel);
    }

    else // sans arg
    {
        if (ft_strncmp(buf, "NAMES\r\n", 7))
            return ("");
        else
            return ("ok");
    }
}

std::string prv_chan_cmd(char *buf, int mode)
{
    std::string channel = "";
    std::string msg = "";
    int len = ft_strlen(buf);

    if (ft_strncmp("PRIVMSG #", buf, 9) || len < 15 || buf[len - 1] != '\n' || buf[len - 2] != '\r')
        return ("");

    int i = 9;
    while (buf[i] != ' ' && i < len - 2)
    {
        if (!is_valid_char(buf[i]))
            return ("");
        channel += buf[i++];
    }
    
    if (ft_strncmp(&buf[i], " :", 2))
        return ("");

    i += 2;
    while (i < len - 2)
        msg += buf[i++];
    
    if (mode == 0)
        return (channel);
    else
        return (msg);
}

std::string prv_clt_cmd(char *buf, int mode)
{
    std::string dest = "";
    std::string msg = "";
    int len = ft_strlen(buf);

    if (ft_strncmp("PRIVMSG ", buf, 8) || len < 14 || buf[len - 1] != '\n' || buf[len - 2] != '\r')
        return ("");

    int i = 8;
    while (buf[i] != ' ' && i < len - 2)
        dest += buf[i++];

    if (ft_strncmp(&buf[i], " :", 2))
        return ("");

    i += 2;
    while (i < len - 2)
        msg += buf[i++];

    if (mode == 0)
        return (dest);
    else
        return (msg);
}

std::string kick_cmd(char *buf, int mode)
{
    std::string user = "";
    std::string channel = "";
    std::string reason = "";
    int len = ft_strlen(buf);

    if (ft_strncmp("KICK #", buf, 6) || len < 11 || buf[len - 1] != '\n' || buf[len - 2] != '\r')
        return ("");

    int i = 6;
    while (buf[i] != ' ' && i < len - 2)
    {
        if (!is_valid_char(buf[i]))
            return ("");
        channel += buf[i++]; 
    }

    if (buf[i] != ' ')
        return ("");

    i++;
    while (buf[i] != ' ' && i < len - 2)
        user += buf[i++];

    if (i == len - 2)
    {
        if (mode == 0)
            return (channel);
        else if (mode == 1)
            return (user);
        else
            return (reason);
    }

    if (ft_strncmp(" :", &buf[i], 2))
        return ("");

    i += 2;
    while (i < len - 2)
        reason += buf[i++];

    if (mode == 0)
        return (channel);
    else if (mode == 1)
        return (user);
    else
        return (reason);
}

std::string inv_cmd(char *buf, int mode)
{
    std::string user = "";
    std::string chan = "";
    int len = ft_strlen(buf);

    if (ft_strncmp("INVITE ", buf, 7) || len < 13 || buf[len - 1] != '\n' || buf[len - 2] != '\r')
        return ("");

    int i = 7;
    while (i < len - 2 && buf[i] != ' ')
    {
        if (!is_valid_char(buf[i])) // memes conditions que pour channel ?
            return ("");
        user += buf[i++];
    }

    if (ft_strncmp(" #", &buf[i], 2))
        return ("");

    i += 2;
    while (i < len - 2)
    {
        if (!is_valid_char(buf[i]))
            return ("");
        chan += buf[i++];
    }

    if (mode == 0)
        return (chan);
    else
        return (user);
}

/*

PASS ********** NICK ********** USER **********( FULL **********)\r\n

    version temporaire tant que les messages 
    ne sont pas automatiquement concaténés

*/
std::string parse_auth(std::string str, int mode)
{
    const char *str1 = str.c_str();
    int i = 5;
    int len = ft_strlen(str1);

    if (len < 26)
        return "";

    if (ft_strncmp(str1, " PASS ", 6))
        return "";

    int j = 0;
    while (i < len && is_valid_char(str1[i]))
    {
        j++;
        i++;
    }

    if (!j || i >= len || ft_strncmp(&str1[i], " NICK ", 6))
        return "";
    
    std::string nick = "";
    i += 6;
    while (i < len && is_valid_char(str1[i]))
    {
        nick += str1[i];
        i++;
    }

    if (nick.empty() || i >= len || ft_strncmp(&str1[i], " USER ", 6))
        return "";

    std::string user = "";
    i += 6;

    while (i < len && is_valid_char(str1[i]))
    {
        user += str1[i];
        i++;
    }

    if (user.empty() || (i < len && ft_strncmp(&str1[i], " FULL ", 6)))
        return "";

    std::string full = "";
    i += 6;

    while (i < len)
    {
        full += str1[i];
        i++;
    }

    if (full.empty() || str1[i - 2] != '\r' || str1[i - 1] != '\n')
        return "";
    
    if (mode == 0)
        return (nick);
    if (mode == 1)
        return (user);
    if (mode == 2)
        return (full);
}


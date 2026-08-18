#include "../../includes/Header.hpp"

static unsigned int ft_strlen(char *str)
{
    unsigned int i = 0;

    while (str[i])
        i++;

    return (i);
}

unsigned int parse_arg(char *arg)
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

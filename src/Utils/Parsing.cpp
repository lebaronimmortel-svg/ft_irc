#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/*
    command_suffix :

    This function is meant to extract
    the second word of a string 
    separated by spaces
*/
std::string cmd_sfx(std::string str)
{
	unsigned long i = 0;
	unsigned long len = str.size();
	std::string res = "";
	while (i < len && str[i] != ' ')
		i++;
	if (i == len)
		return res;
	i++;
	while (i < len && str[i] != ' ')
			res += str[i++];
	return res;
}

/*
    command_suffix_full :

    This function is meant to extract
    the following of the first word in 
    a string separated by spaces
*/
std::string cmd_sfx_full(std::string str)
{
	unsigned long i = 0;
	unsigned long len = str.size();
	std::string res = "";
	while (i < len && str[i] != ' ')
		i++;
	if (i == len)
		return res;
	i++;
	while (i < len && str[i])
			res += str[i++];
	return res;
}

/*
    command_suffix_reference :

    This function is meant to extract
    the second word of a string given
    by reference and separated by spaces
*/
std::string cmd_sfx_ref(std::string& str)
{
	unsigned long i = 0;
	unsigned long len = str.size();
	std::string res = "";
	while (i < len && str[i] != ' ')
		i++;
	if (i == len)
		return res;
	i++;
	while (i < len)
			res += str[i++];
	return res;
}
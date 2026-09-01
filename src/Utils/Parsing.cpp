/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexfuen <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 21:56:22 by alexfuen          #+#    #+#             */
/*   Updated: 2026/09/01 21:56:37 by alexfuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

/*
    command_suffix :

		This function is meant to extract
		the second word of a string 
		separated by spaces
*/
std::string cmdSfx(std::string str)
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
std::string cmdSfxFull(std::string str)
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
		the following of the first word
		of a string given by reference
		and separated by spaces
*/
std::string cmdSfxRef(std::string& str)
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

/*
    command_suffix_reference_word :

		This function is meant to 
		extract the second word
		of a string given by reference
		and separated by spaces
*/
std::string cmdSfxRefWord(std::string& str)
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
	command_prefix

		This function is meant 
		to extract the first word
		of a string separated 
		by spaces
*/
std::string cmdPfx(std::string str)
{
	unsigned long i = 0;
	std::string res = "";
	while (i < str.size() && str[i] != ' ')
		res += str[i++];
	return res;
}

/*
	nick_name_validity

		This function is meant
		to check if the provided
		string is valid as an IRC
		user nickname
*/
bool nicknameValid(std::string str)
{
	std::string spec("[]\\`_^{|}");
	size_t length = str.length();
	if (length == 0 || length > 9)
		return false;

	for (size_t i = 0; i < length; i++)
	{
		if (i == 0)
		{
			if (!isalpha(str[i]) && spec.find(str[i]) == spec.npos)
				return false;
		}
		else
		{
			if (!isalnum(str[i]) && spec.find(str[i]) == spec.npos && str[i] != '-')
				return false;
		}
	}
	return (true);
}


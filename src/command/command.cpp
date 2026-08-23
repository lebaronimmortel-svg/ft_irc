/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:50:16 by tseche            #+#    #+#             */
/*   Updated: 2026/08/23 17:56:50 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Command.hpp"

std::vector<std::string> &getArgs(std::string &str){
	std::vector<std::string> vec;
	int sep = str.find(' ');
	if (sep == str.npos)
		return (vec);
	size_t lenght = str.length();
	for (int i = 0; i < lenght; i++){
		vec.push_back(str.substr(i, sep));
		i += sep;
		sep = str.find(' ', i);
	}
	return (vec);
}
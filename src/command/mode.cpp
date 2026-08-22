/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:32:51 by tseche            #+#    #+#             */
/*   Updated: 2026/08/22 14:49:45 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"
#include <cstring>
#include <sstream>

void add_order(bool state, std::string &order, char c){
	int find = order.find(c); 
	if (!state){
		if (find != order.npos)
			order.erase(find);
	} else {
		if (find == order.npos)
			order.append(&c);
	}
}

std::string get_word(std::string str, int &i, bool &end){
	int lenght = str.length();
	for (; i < lenght && str[i] == ' '; i++);
	int start = i;
	bool word = false;
	for (; i < lenght && str[i] != ' '; i++)
		word = true;
	if (!word)
	{
		end = true;
		return "";
	}
	end = true;
	return (str.substr(start, i));
}

mode_s *get_arg(std::string str){
	mode_s *args = new mode_s();
	size_t lenght = str.length();
	bool state = false;
	bool init_state = false;
	std::string order = "";
	int i = 0;
	for (; i < lenght; i++){
		if (str[i] == '+')
			state = true;
		else if (str[i] == '-')
			state = false;
		else if (str[i] == ' ')
			break;
		else if (!init_state || strchr("itkol", str[i]) == NULL)
			return NULL;
		else {
			switch (str[i]){
				case 'i':{
					args->flag.i = state;
				}
				case 't':{
					args->flag.t = state;
				}
				case 'k':{
					args->flag.k = state;
					add_order(state, order, 'k');
				}
				case 'o':{
					args->flag.o = state;
					add_order(state, order, 'o');
				}
				case 'l':{
					args->flag.l = state;
					add_order(state, order, 'l');
				}
			}
		}
	}
	int start = i;
	std::vector<std::string> wvec;
	size_t lenght_vec = 0;
	bool end = false;
	for (; i < lenght; i++){
		std::string word = get_word(str, i, end);
		if (end)
			break;
		wvec.push_back(word);
		lenght_vec++;
	};
	if (lenght_vec)
		return (args);
	size_t order_l = order.length();
	size_t vec_i = 0;
	for (size_t y = 0; y < order_l; i++){
		switch (order[y]){
			case 'k':{
				args->value.k = wvec.at(vec_i);
			}
			case 'o':{
				for (; vec_i < lenght_vec - y; vec_i++){
					args->value.o.push_back(wvec.at(vec_i));
				}
			}
			case 'l':{
				std::istringstream is(wvec.at(vec_i));
				is >> args->value.l;
				if (is.fail() || !is.eof()){
					// return error
				}
			}
		}
	}
	return (args);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tseche <tseche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:35:33 by tseche            #+#    #+#             */
/*   Updated: 2026/08/22 12:54:40 by tseche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct mode_flag{
	bool i;
	bool t;
	bool k;
	bool o;
	bool l;
};

struct mode_value{
	std::string k;
	size_t l;
	std::vector<std::string> o;
};

struct mode_s{
	mode_value value;
	mode_flag flag;
};
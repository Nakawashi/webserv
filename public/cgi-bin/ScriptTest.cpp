/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScripTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldervode <ldervode@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 07:07:23 by ldervode          #+#    #+#             */
/*   Updated: 2023/07/30 07:07:23 by ldervode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../../include/webserv.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::ofstream fileout("data-records.txt");
		fileout << argv[1];
		fileout.close();
	}
	return 0;
}
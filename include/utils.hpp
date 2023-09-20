#pragma once

#include "webserv.hpp"

std::string		ft_tostring(int var);
bool			is_dir(std::string path);
bool			is_file(std::string path);
std::string		get_current_time_in_GMT_format();

typedef			std::map<std::string, std::vector<std::string> > Locations;
void			printMapContent(const Locations& Locations);

typedef			std::map<std::string, std::vector<std::string> > InnerMap;
typedef			std::map<std::string, InnerMap> OuterMap;
void			printLocations(const OuterMap& locations);

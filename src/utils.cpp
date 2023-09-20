#include "../include/webserv.hpp"

std::string	ft_tostring(int var)
{
	std::stringstream ss;
	ss << var;
	std::string str = ss.str();
	return str;
}

bool	is_dir(std::string path)
{
	DIR *dir;
	dir = opendir(("." + path).c_str());
	if (dir != NULL)
	{
		closedir(dir);
		return true;
	}
	return false;
}

bool	is_file(std::string path)
{
	std::ifstream file(("." + path).c_str());
	if (file.is_open() && is_dir(path) == false)
	{
		file.close();
		return true;
	}
	return false;
}

std::string	get_current_time_in_GMT_format()
{
	std::time_t	currentTime;
	std::time(&currentTime);

	struct std::tm	timeInfo;
	gmtime_r(&currentTime, &timeInfo);

	char buffer[64];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &timeInfo);

	return buffer;
}

void	sig_handler(int sig);

void	printMapContent(const Locations& locations)
{
	for (Locations::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		for (std::vector<std::string>::const_iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
		{
			std::cout << " " << *vecIt;
		}

		std::cout << std::endl;
	}
}

void printLocations(const OuterMap& locations) {
	for (OuterMap::const_iterator itOuter = locations.begin(); itOuter != locations.end(); ++itOuter)
	{
		std::cout << "location : " << RED << itOuter->first << RESET << std::endl;

		// Parcourir chaque élément de la map interne
		const InnerMap& innerMap = itOuter->second;
		for (InnerMap::const_iterator itInner = innerMap.begin(); itInner != innerMap.end(); ++itInner)
		{
			std::cout << "location attributes: " << itInner->first << ", Valeurs :";

			// Parcourir le vecteur de valeurs associé à chaque clé interne
			const std::vector<std::string>& vec = itInner->second;
			for (std::vector<std::string>::const_iterator vecIt = vec.begin(); vecIt != vec.end(); ++vecIt)
			{
				std::cout << " " << YELLOW << *vecIt << RESET;
			}

			std::cout << std::endl;
		}
	}
}

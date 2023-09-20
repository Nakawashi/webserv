#include "../include/webserv.hpp"

/*
	There are three types of attributes :

	1 - socket : everything to open the door and how go threw it
	2 - configuration file : the "one per server"'s directives (name, port, body size, error file given by user)
	3 - configuration file : the possibly "multiple ones per server"'s directives (locations: auto index, methods, root, index, cgi ext and path)

	1 socket
		sockaddr_in structure is gonna be initialized in ft_bind(), for better understanding of the routine order.
		others are in
	2 one per server
		initialized in the Server constructor, to give a default value in every cases
	3. multiple per server
		we check it in InitServers class, because we can loop in the _servers vector with values given

*/
Server::Server()
: _domain(AF_INET), _service_type(SOCK_STREAM), _protocol(0), _socket_fd(-2), _interface(INADDR_ANY)
{

	_name = "localhost";
	_clientMaxBodySize = 1000; // 1M is the nginx defalut
	_error_page_404 = "public/error/404/404.html";
}

// Getter functions implementation
std::string Server::getName() const
{
	return _name;
}

int Server::getPort() const
{
	return _port;
}

int Server::getSocketFd() const
{
	return _socket_fd;
}

unsigned long	Server::getclientMaxBodySize() const
{
	return _clientMaxBodySize;
}

std::map<std::string, std::map<std::string, std::vector<std::string> > > Server::getLocation() const
{
	return _locations;
}

std::vector<std::string> &Server::getLocationAttributes(std::string path, std::string attribute)
{
	return _locations[path][attribute];
}

struct sockaddr_in	&Server::getAddress()
{
	return _address;
}

std::vector<std::string> Server::get_index(std::string path)
{
	for (OuterMap::const_iterator itOuter = _locations.begin(); itOuter != _locations.end(); ++itOuter)
	{
		if (itOuter->first == path)
		{
			const InnerMap& innerMap = itOuter->second;
			for (InnerMap::const_iterator itInner = innerMap.begin(); itInner != innerMap.end(); ++itInner)
			{
				if (itInner->first == "index")
					return itInner->second;
			}
		}
	}
	std::vector<std::string> default_index;
	default_index.push_back("/public/index.html");
	return default_index;
}

std::vector<std::string> Server::get_root(std::string path)
{
	for (OuterMap::const_iterator itOuter = _locations.begin(); itOuter != _locations.end(); ++itOuter)
	{
		if (itOuter->first == path)
		{
			const InnerMap& innerMap = itOuter->second;
			for (InnerMap::const_iterator itInner = innerMap.begin(); itInner != innerMap.end(); ++itInner)
			{
				if (itInner->first == "root")
					return itInner->second;
			}
			setLocationAttributes(path, "root", path);
		}
	}
	return get_root(path);
}



// Setter functions implementation
void Server::setName(std::string value)
{
	_name = value;
}

void Server::setPort(int value)
{
	_port = value;
}

void	Server::setSocket_fd(int fd)
{
	_socket_fd = fd;
}

void Server::setLocations(std::string path)
{
	_locations[path];
}

void Server::setclientMaxBodySize(unsigned long value)
{
	_clientMaxBodySize = value;
}

void Server::setLocationAttributes(std::string path, std::string attribute, std::string value)
{
	_locations[path][attribute].push_back(value);
}

/*
	We only want this scenario.
*/
void	Server::setAddress()
{
	memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
}
// End setter functions implementation


// Base to run a server functions
bool Server::create_socket()
{
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error in socket" << std::endl;
		return false;
	}
	const int enable = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	std::cout << "setsockopt(SO_REUSEADDR) failed\n";	
	return true;
}

bool Server::ft_bind()
{
	setAddress();
	if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		std::cerr << "Error in binding" << std::endl;
		return false;
	}
	return true;
}

bool Server::ft_listen()
{
	if (listen(_socket_fd, 10) < 0)
	{
		std::cerr << "Error in listening" << std::endl;
		return false;
	}
	return true;
}

std::vector<std::string> Server::get_CGI_ext(std::string path)
{
	for (OuterMap::const_iterator itOuter = _locations.begin(); itOuter != _locations.end(); ++itOuter)
	{
		if (itOuter->first == path)
		{

			const InnerMap& innerMap = itOuter->second;
			for (InnerMap::const_iterator itInner = innerMap.begin(); itInner != innerMap.end(); ++itInner)
			{
				if (itInner->first == "cgi_extension")
				{
					return itInner->second;
				}
			}
		}
	}
	std::vector<std::string> cgi_ext_default;
	cgi_ext_default.push_back(".sh");
	return cgi_ext_default;
}

std::vector<std::string> Server::get_CGI_path(std::string path)
{
	for (OuterMap::const_iterator itOuter = _locations.begin(); itOuter != _locations.end(); ++itOuter)
	{
		if (itOuter->first == path)
		{

			const InnerMap& innerMap = itOuter->second;
			for (InnerMap::const_iterator itInner = innerMap.begin(); itInner != innerMap.end(); ++itInner)
			{
				if (itInner->first == "cgi_path")
					return itInner->second;
			}
		}
	}
	std::vector<std::string> cgi_path_default;
	cgi_path_default.push_back("/bin/bash");
	return cgi_path_default;
}

void	Server::set_default_error_404_path(std::string value)
{
	_error_page_404 = value;
}

std::string					Server::get_default_error_404_path() const
{
	return _error_page_404;
}

void	Server::setRedirection(std::string value)
{
	_redirection.push_back(value);
}

std::vector<std::string>	Server::getRedirection() const
{
	return _redirection;
}

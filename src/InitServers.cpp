#include "../include/webserv.hpp"

void	sig_handler(int sig)
{
	(void)sig;
	exit (0);
}

InitServers::InitServers(std::ifstream& fileStream)
{
	set_servers(fileStream);
	launch_servers();
	run_servers();
}

void	InitServers::run_servers()
{
	_numConn = 1;
	_timeout = 3000;
	int serv_num = 0;
	struct pollfd	_fds[1001];

	while (true)
	{	
		for (unsigned int i = 0; i < _servers.size(); i++)
		{
			socklen_t addressLength = sizeof(_servers[i]->getAddress());
			_fds[0].fd = _servers[i]->getSocketFd();
			_fds[0].events = POLLIN;
			int ready = poll(_fds, _numConn, _timeout);
			if (ready == -1)
			{
				std::cerr << "Erreur lors de l'appel à poll" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (ready == 0)
				continue;
			for(int j = 0; j < _numConn; j++)
			{
				if (_fds[j].revents & POLLIN)
				{
					if(_fds[j].fd == _servers[i]->getSocketFd())
					{
						if ((_socket = accept(_servers[i]->getSocketFd(), (struct sockaddr*)&_servers[i]->getAddress(), &addressLength)) < 0)
						{
							std::cerr << "Error in Accept" << std::endl;
							std::cerr << strerror(errno);
							exit(EXIT_FAILURE);
						}
						if (_numConn == 1000)
						{
							std::cout << "\t\tNombre maximal de connexions atteint [" << _numConn << "]. Nouvelle connexion refusée." << std::endl;
							close(_socket);
							_numConn = 1;
						}
						else
						{
							const int enable = 1;
							if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    						std::cout << "setsockopt(SO_REUSEADDR) failed\n";
							_fds[_numConn].fd = _socket;
							_fds[_numConn].events = POLLIN;
							_numConn++;
							serv_num = i;
						}
					}
					else 
					{
						char	buffer[1024];
						ssize_t	bytesRead = 1024;
						ssize_t	total = 0;
						char	request[1024];
						bzero(request, 1024);
						request[0] = '\0';
						
			
						std::ofstream request_bin("./tmp");
						while (bytesRead == 1024)
						{
							bzero(buffer, 1024);
							bytesRead = recv(_socket, &buffer, sizeof(buffer), 0);
							if (bytesRead == -1) 
							{
								close(_socket);
								break ;
							}
							else if (bytesRead == 0 && total == 0)
							{
								close(_socket);
								break ;
							}
							else
								total += bytesRead;
							if (request[0] == '\0')
							{
								strncpy(request, buffer, bytesRead);
							}
							if (request_bin.is_open())
								request_bin.write(buffer, bytesRead);
						}
						request_bin.close();
						if (total > 0)
						{
							int sav;
							HttpRequest message(request, *_servers[serv_num]);
							RequestHandler processing(message, *_servers[serv_num]);
							std::string response = processing.getResponse();
							sav = send(_socket, response.c_str(), response.length(), 0);
							if (sav == -1)
							{
								std::cout << "Erreur lors de la l'envoi des données'";
								std::cout << strerror(errno);
								_fds[j].fd = -1;
							}
							else if (sav == 0)
							{
								std::cout << "send return 0\n";
								_fds[j].fd = -1;
							}
							close(_socket);
						}
					}
				}
			}
		}
	}
}


InitServers::~InitServers() {}

/*
	Make each server alive, open and listen
*/
void	InitServers::launch_servers()
{
	signal(SIGINT, sig_handler);
	for (unsigned int i = 0; i < _servers.size(); ++i)
	{
		_servers[i]->create_socket();
		_servers[i]->ft_bind();
		_servers[i]->ft_listen();
	}
}

/*
	Give each server config file datas
*/
void InitServers::set_servers(std::ifstream& fileStream)
{
	std::string line;
	int i = -1;
	std::string currentLocation;
	std::string previous_line;

	while (std::getline(fileStream, line))
	{
		int brackets = 0;
		std::string directive;
		if (line.compare(0, 6, "server") == 0)
		{
			i++;
			_servers.push_back(new Server);
			if (line.find("{") != std::string::npos)
				brackets++;
			else
			{
				std::getline(fileStream, line);
				if (line.find("{") != std::string::npos)
					brackets++;
			}
			while (brackets != 0 && std::getline(fileStream, line))
			{

				std::istringstream iss(line);
				std::string value;

				if (line.find("{") != std::string::npos)
					brackets++;
				if (line.find("}") != std::string::npos)
					brackets--;

				if (!(iss >> directive))
					continue;
				else if (directive == "name")
				{
					iss >> value;
					_ft_trim(value);
					_servers[i]->setName(value);
				}
				else if (directive == "error_page_404")
				{
					iss >> value;
					_ft_trim(value);
					_servers[i]->set_default_error_404_path(value);
				}
				else if (directive == "redirection")
				{
					std::string redir;
					std::getline(iss, redir);

					std::istringstream iss_redir(redir);
					while (iss_redir >> value)
					{
						_ft_trim(value);
						_servers[i]->setRedirection(value);
					}
					std::cout << "redirection server [" << i << "] = " << RED << _servers[i]->getRedirection()[0] << " " << _servers[i]->getRedirection()[1] << RESET << std::endl;
				}
				else if (directive == "port")
				{
					iss >> value;
					_ft_trim(value);
					_servers[i]->setPort(atoi(value.c_str()));
					if (_servers[i]->getPort() <= 0)
						throw std::runtime_error("\033[0;31mError : configuration file 's directive missing : port\033[0m");

				}
				else if (directive == "client_max_body_size")
				{
					iss >> value;
					_ft_trim(value);
					_servers[i]->setclientMaxBodySize(atoi(value.c_str()));
				}
				else if (directive == "location")
				{
					std::string path;
					iss >> path;
					_ft_trim(path);
					currentLocation = path;
					_servers[i]->setLocations(path);
					if (_servers[i]->getLocation().empty())
						throw std::runtime_error("\033[0;31mError : configuration file 's directive missing : location\033[0m");
				}
				else if (directive == "autoindex")
				{
					iss >> value;
					_ft_trim(value);
					if (value.empty())
						value = "off";
					_servers[i]->setLocationAttributes(currentLocation, "autoindex", value);
				}
				else if (directive == "root")
				{
					iss >> value;
					_ft_trim(value);
					if (value.empty())
						_servers[i]->get_root(currentLocation);
					else
						_servers[i]->setLocationAttributes(currentLocation, "root", value);
				}
				else if (directive == "index")
				{
					iss >> value;
					_ft_trim(value);
					_servers[i]->setLocationAttributes(currentLocation, "index", value);
				}
				else if (directive == "method")
				{
					std::string methods;
					std::getline(iss, methods);

					std::istringstream iss_methods(methods);

					while (iss_methods >> value)
					{
						_ft_trim(value);
						if (value.empty())
							throw std::runtime_error("\033[0;31mError : configuration file 's directive value missing : method\033[0m");
						else
							_servers[i]->setLocationAttributes(currentLocation, "method", value);
					}
				}
				else if (directive == "cgi_extension")
				{
					std::string ext;
					std::getline(iss, ext);

					std::istringstream iss_extensions(ext);
					while (iss_extensions >> value)
					{
						_ft_trim(value);
						if (value.empty())
							throw std::runtime_error("\033[0;31mError : configuration file 's directive value missing : cgi_extension\033[0m");
						else
							_servers[i]->setLocationAttributes(currentLocation, "cgi_extension", value);
					}
				}
				else if (directive == "cgi_path")
				{
					std::string paths;
					std::getline(iss, paths);

					std::istringstream iss_paths(paths);
					while (iss_paths >> value)
					{
						_ft_trim(value);
						if (value.empty())
							throw std::runtime_error("\033[0;31mError : configuration file 's directive value missing : cgi_path\033[0m");
						else
							_servers[i]->setLocationAttributes(currentLocation, "cgi_path", value);
					}
				}
			}
			previous_line = line;
		}
	}
}

// getters
std::vector<Server*>	InitServers::get_servers() const
{
	return _servers;
}


void	InitServers::_ft_trim(std::string& value)
{
	size_t pos = value.find(";");
	if (pos != std::string::npos)
	{
		value.erase(value.end()-1);
	}
}

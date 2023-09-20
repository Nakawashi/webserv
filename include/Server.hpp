#pragma once

#include "webserv.hpp"

typedef		std::map<std::string, std::vector<std::string> > InnerMap;
typedef		std::map<std::string, InnerMap> OuterMap;

class Server
{
public:
	Server();

	bool						create_socket();
	bool						ft_bind();
	bool						ft_listen();
	std::vector<std::string>	get_root(std::string path);
	std::vector<std::string>	get_index(std::string path);

	// Getter function
	std::string					getName() const;
	int							getPort() const;
	unsigned long				getclientMaxBodySize() const;
	std::string					get_default_error_404_path() const;
	struct sockaddr_in			&getAddress();
	int							getSocketFd() const;
	std::vector<std::string>	&getLocationAttributes(std::string path, std::string attribute);
	std::map<std::string, std::map<std::string, std::vector<std::string> > >	getLocation() const;
	std::vector<std::string>	get_CGI_ext(std::string path);
	std::vector<std::string>	get_CGI_path(std::string path);
	std::vector<std::string>	getRedirection() const;

	// Setter function
	void	setName(std::string value);
	void	setPort(int value);
	void	setclientMaxBodySize(unsigned long value);
	void	set_default_error_404_path(std::string value);
	void	setLocations(std::string path);
	void	setLocationAttributes(std::string path, std::string attribute, std::string value);
	void	setSocket_fd(int fd);
	void	setAddress();
	void	setRedirection(std::string value);

private:
	~Server();

	// vérifier si tous les attributs sont initialisés sinon print une erreur dans le terminal --> plutôt dans init servers car lui qui a les _servers et peut boucler dessus
	// modifier ces fonctions, ajouter un paramètre "directive" pour dynamiser les fonctions et ne pas les copier coller get_root et get_index

	// socket types of attributes
	int			_domain;				// AF_INET
	int			_service_type;			// SOCK_STREAM
	int			_protocol;				// 0
	int			_socket_fd;				// -2 because -2 is not used
	u_long		_interface;				// INADDR_ANY
	struct		sockaddr_in	_address;	// ft_bind() inizialize the values

	// conf file "one per server"
	std::string		_name;
	int				_port;
	unsigned long	_clientMaxBodySize;
	std::string		_error_page_404;
	std::vector<std::string>		_redirection;

	// conf file "multiple per server" : check in setLocationAttributes() directly, easier
	std::map<std::string, std::map<std::string, std::vector<std::string> > >	_locations;

};

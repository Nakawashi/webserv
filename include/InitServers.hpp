#pragma once

#include "webserv.hpp"

class InitServers
{
	public:
		InitServers(std::ifstream& fileStream);
		~InitServers();

		void	set_servers(std::ifstream& fileStream);
		void	launch_servers();
		void	run_servers();

		std::vector<Server*>	get_servers() const;

	private:
		int							_numConn;
		int							_timeout;
		int							_socket;
		std::vector<Server*>		_servers;

		void						_ft_trim(std::string& value);
	};

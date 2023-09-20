#pragma once

#include "webserv.hpp"

class Server;

class CgiHandler
{
	private:
		CgiHandler();

		std::string scriptPath;
		std::vector<char *> envp;

	public:
		CgiHandler(const std::string &scriptPath, Server &server);
		~CgiHandler();

		std::string executeCGI(const std::map<std::string, std::string>& _headers, Server &server, std::string formdata);
		// bool 		check_CGI_ext(std::string path, Server &server);


};

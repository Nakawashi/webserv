#pragma once

#include "webserv.hpp"

class RequestHandler
{
public:
	RequestHandler(const HttpRequest &request, Server &serv);
	~RequestHandler();

	std::string	getResponse() const;
	bool check_body_size(const HttpRequest &request, Server &server);
	std::string extract_binary_data(std::ifstream &infile, const std::string& boundary);



private:
	std::string	_response;
	int _content_len;

	RequestHandler();

	void	handleGetRequest(const HttpRequest &request, Server &serv);
	void	handlePostRequest(const HttpRequest &request, Server &serv);
	void	handleDeleteRequest(const HttpRequest &request, Server &serv);
	void	handleUnsupportedMethod(const HttpRequest &request, Server &serv);
	bool	is_method_allowed(std::string path, bool referer, Server &serv, std::string method);
};

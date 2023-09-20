
#pragma once

#include "webserv.hpp"

class Server;

class HttpRequest
{
public:
	HttpRequest(const std::string &message, Server &serv);
	~HttpRequest();

	void								displayRequest();
	void								parse(const std::string &message);

	std::string							getMethod() const;
	std::string							getPath() const;
	std::string							get_http_version() const;
	std::map<std::string, std::string>	get_headers() const;
	std::string							get_body() const;
	std::string							get_root() const;
	bool								get_is_referer() const;

	//setPath(std::sting value)

private:
	HttpRequest();
	// parsing
	void		_parseRequestLine(const std::string &requestLine);
	void		_parseHeaders(const std::string &headerBlock);
	std::string	_trim(const std::string& str);
	void		_root_handler(Server &serv);

	std::string							_method;
	std::string							_path;
	std::string							_httpVersion;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	std::string							_root;
	bool								_is_referer;
};

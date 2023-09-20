
#pragma once

class Server;

#include "webserv.hpp"

class Response
{
public:
	Response(std::string method, std::string httpVersion, std::map<std::string, std::string> headers, std::string path, int code, std::string root, std::string request_body, bool referer, Server &server);
	~Response();
	Response &operator=(const Response &rhs);

	void								setHttpVersion(const std::string &version);
	void								setBody(const int &code);
	void								setNewHeader(const std::string &key, const std::string &value);

	int									getCode() const;
	std::string							get_http_version() const;
	std::map<std::string, std::string>	get_headers() const;
	std::string							getDateTime() const;
	std::string							get_body() const;
	int									get_bodySize() const;
	std::string							buildFullResponse();

	bool 	check_CGI_ext(std::string path, Server &server);

private:
	Response();
	static std::map<int, std::string>	_initCodeDescription();
	static std::map<int, std::string>	_codeDescription;
	bool								_setErrorCode(Server &server);
	bool								_isMethodImplemented(std::string& method) const;
	std::string 						_generateAutoIndex(const std::string& directory);


	int									_code;
	std::string							_method;
	std::string							_httpVersion;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	int									_bodySize;
	std::string							_dateTime;
	std::string							_fullResponse;
	std::string							_path;
	std::string							_root;
	std::string							_request_body;
	bool								_referer;

	const static std::string			_allowedMethods[];
};

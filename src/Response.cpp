
#include "../include/webserv.hpp"

const std::string	Response::_allowedMethods[] =
{
	"GET",
	"POST",
	"DELETE"
};

Response::Response(std::string method, std::string httpVersion, std::map<std::string, std::string> headers, std::string path, int code, std::string root, std::string request_body, bool referer, Server &server)
: _code(code), _method(method), _httpVersion(httpVersion), _headers(headers), _bodySize(0), _fullResponse(""), _path(path), _root(root), _request_body(request_body), _referer(referer)
{
	_dateTime = get_current_time_in_GMT_format();
	std::vector<std::string> cgi_ext;
	if (code == 204)
	{
			return ;
	}
	if (check_CGI_ext(_path, server) == true && _setErrorCode(server) == false)
	{
		CgiHandler scriptCGI(_path, server);
		std::cout << "CGI print:" << std::endl;
		_body = scriptCGI.executeCGI(_headers, server, _request_body);
	}
	else
	{
		if (server.getRedirection().size() > 0)
		{
			if (_path == server.getRedirection()[0])
				_path = server.getRedirection()[1]; 
		}
		std::string	filePath = _path;
		if (_path == "/" || _path == _root || is_dir(_path))
		{
			if (is_dir(_path) && _path != _root)
			{
				_body = _generateAutoIndex(_path);
				return ;
			}
			else
			{
				filePath = server.get_index(_path)[0];
			}
		}
		if (_setErrorCode(server) || _code == 413)
		{
			std::string		filePath;
			if (_code == 404)
				filePath = server.get_default_error_404_path();
			else
				filePath = "public/error/" + ft_tostring(_code) + "/" + ft_tostring(_code) + ".html";

			std::ifstream	fileStream(filePath.c_str());
			std::vector<char> fileContent;
			char byte;

			while (fileStream.read(&byte, sizeof(byte)))
			{
				fileContent.push_back(byte);
			}
			if (_referer == false || _code == 413 || _code == 404)
				_body = std::string(fileContent.begin(), fileContent.end());
			return ;
		}
		std::ifstream	fileStream(filePath.c_str() + 1, std::ios::binary);
		std::vector<char> fileContent;
		char byte;
		while (fileStream.read(&byte, sizeof(byte)))
		{
			fileContent.push_back(byte);
		}
		_body = std::string(fileContent.begin(), fileContent.end());
	}
}

Response::~Response(){}

Response &Response::operator=(const Response &rhs)
{
	if (this != &rhs) {
		_code = rhs.getCode();
		_httpVersion = rhs.get_http_version();
		_headers = rhs.get_headers();
		_body = rhs.get_body();
	}
	return *this;
}

void Response::setHttpVersion(const std::string &version)
{
	_httpVersion = version;
}

void	Response::setBody(const int &code)
{
	(void) code;
	if (_body.empty() && _code != 201)
	{
		_body = "<h1>This code's status is not implemented</h1>";
	}
}

void Response::setNewHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

std::string Response::buildFullResponse()
{
	_fullResponse.append("HTTP/" + _httpVersion);
	_fullResponse.append(" ");
	_fullResponse.append(ft_tostring(_code));
	_fullResponse.append(" ");
	_fullResponse.append(_codeDescription[_code]);
	_fullResponse.append("\r\n");
	std::map<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it) {
		if (it->first == "Content-Length" && _code == 201)
			it->second = ft_tostring(_body.size());
		_fullResponse.append(it->first + ": " + it->second + "\n"); // suppression de \r
	}
	_fullResponse.append("\r\n");
	_fullResponse.append(_body);

	return _fullResponse;
}

bool	Response::_isMethodImplemented(std::string& method) const
{
	for (size_t i = 0; i < _allowedMethods->size(); ++i)
	{
		if (method == _allowedMethods[i])
			return true;
	}
	return false;
}

/*
	returns true if an error code is set
*/
bool	Response::_setErrorCode(Server &server)
{
	if (!_isMethodImplemented(_method))
	{
		_code = 501;
		return true;
	}
	if (get_http_version() != "HTTP/1.1")
	{
		_code = 505;
		return true;
	}
	if (_request_body.size() > server.getclientMaxBodySize())
	{
		_code = 413;
		return true;
	}
	if (!is_file(_path) && !is_dir(_path))
	{
		_code = 404;
		return true;
	}
	return false;
}

std::map<int, std::string> Response::_codeDescription = Response::_initCodeDescription();

std::map<int, std::string> Response::_initCodeDescription()
{
	std::map<int, std::string>	codeDescription;

	codeDescription[200] = "OK";
	codeDescription[201] = "Created";
	codeDescription[204] = "No Content";
	codeDescription[301] = "Moved Permanently";
	codeDescription[400] = "Bad request";					// request syntax invalid
	codeDescription[403] = "Forbidden";
	codeDescription[404] = "Not Found";
	codeDescription[405] = "Method not allowed";
	codeDescription[413] = "Payload Too Large";
	codeDescription[415] = "Unsupported Media Type";		// if media is not supported
	codeDescription[500] = "Internal Server Error";			// else:general server side http error
	codeDescription[501] = "Not Implemented";				// method not implemented
	codeDescription[505] = "HTTP Version Not Supported";	// if http version is not 1.1

	return codeDescription;
}


int Response::getCode() const
{
	return _code;
}


std::string Response::get_http_version() const
{
	return _httpVersion;
}

std::map<std::string, std::string> Response::get_headers() const
{
	return _headers;
}

std::string	Response::getDateTime() const
{
	return _dateTime;
}

std::string Response::get_body() const
{
	return _body;
}

int	Response::get_bodySize() const
{
	return _body.size();
}

bool 	Response::check_CGI_ext(std::string path, Server &server)
{
	std::vector<std::string> CGI_ext = server.getLocationAttributes("/cgi-bin", "cgi_extension");
	for(unsigned long i = 0; i != CGI_ext.size(); i++)
	{
		size_t pos = path.find(CGI_ext[i]);
		if (pos != std::string::npos)
		{
			return true;
		}
	}
	return false;
}

std::string Response::_generateAutoIndex(const std::string& directory)
{
	std::string autoIndex;
	autoIndex += "<html><head><title>Autoindex</title><meta charset=\"UTF-8\"></head><body>";
	autoIndex += "<h1>Contenu du répertoire</h1><ul>";

	DIR* dir = opendir(("." + directory).c_str());
	if (dir)
	{
		struct dirent* entry;
		while ((entry = readdir(dir))) {
			std::string entryName(entry->d_name);
			bool isDirectory = entry->d_type == DT_DIR;

			autoIndex += "<li>";
			if (isDirectory) {
				autoIndex += "<strong>[Dossier]</strong> ";
			}
			autoIndex += "<a href='" + entryName + "'>" + entryName + "</a>";
			autoIndex += "</li>";
		}
		closedir(dir);
	}
	autoIndex += "</ul></body></html>";
	return autoIndex;
}

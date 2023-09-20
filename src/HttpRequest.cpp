
#include "../include/webserv.hpp"

void	HttpRequest::_root_handler(Server &serv)
{
	if (is_file(_path))
		return ;


	std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_reverse_iterator it_loc; 
	std::map<std::string, std::map<std::string, std::vector<std::string> > > locations = serv.getLocation();
	for (it_loc = locations.rbegin(); it_loc != locations.rend(); ++it_loc)
	{
		std::string loc = _path;
		std::string to_find = it_loc->first;
		size_t	pos = loc.find(to_find, 0);
		if (pos != std::string::npos)
		{
			if (serv.get_root(it_loc->first)[0] == "missing directive")
			{
				break ;
			}
			std::vector<std::string> root = serv.getLocationAttributes(it_loc->first, "root");
			_root = root[0];
			int n = 1;
			if (it_loc->first == "/")
				n = 0;
			_path.replace(0, it_loc->first.size() + n, serv.getLocationAttributes(it_loc->first, "root")[0]); 

			break ;
		}
	}
}


HttpRequest::HttpRequest(const std::string &message, Server &server)
: _method(""), _path(""), _httpVersion(""), _headers(), _body(""), _is_referer(false)
{
	parse(message);
	if (_is_referer == false)
		_root_handler(server);
}

HttpRequest::~HttpRequest(){}

void HttpRequest::parse(const std::string &message)
{
	unsigned long pos = message.find("\r\n");
	if (pos != std::string::npos) {
		std::string requestLine = message.substr(0, pos);
		_parseRequestLine(requestLine);

		std::string::size_type start = pos + 2;
		pos = message.find("\r\n\r\n", start);
		if (pos != std::string::npos)
		{
			_parseHeaders(message.substr(start, pos - start));
			pos += 4;
			if (_method != "GET")
			{
				for(size_t i = pos; i < message.size(); i++)
				{
						_body.push_back(message[i]);
				}
			}
		}
		else {
			pos = message.find("\n\n", start);
			if (pos != std::string::npos)
			{
				_parseHeaders(message.substr(start, pos - start));
				if (_method != "GET")
				{
					for(size_t i = pos + 4; i < message.size(); i++)
					{
							_body.push_back(message[i]);
					}
				}
			}
			else {
				_parseHeaders(message.substr(start));
			}
		}
		std::map<std::string, std::string>::iterator it_headers;
		it_headers = _headers.find("Referer");
		if (it_headers != _headers.end())
		{
			_is_referer = true;
			size_t start = _path.rfind("/public");
			if (start != std::string::npos)
				_path = _path.substr(start, _path.size() - start);
		}
	}
}

void HttpRequest::_parseRequestLine(const std::string &requestLine)
{
	std::string::size_type start = 0;
	std::string::size_type pos = requestLine.find(' ');

	if (pos != std::string::npos) {
		_method = requestLine.substr(start, pos - start);
		start = pos + 1;

		pos = requestLine.find(' ', start);
		if (pos != std::string::npos) {
			_path = requestLine.substr(start, pos - start);
			std::transform(_path.begin(), _path.end(), _path.begin(), tolower);
			start = pos + 1;
			_httpVersion = requestLine.substr(start);
		}
	}
}

void HttpRequest::_parseHeaders(const std::string &headersBlock)
{
	std::istringstream iss(headersBlock);
	std::string line;
	while (std::getline(iss, line) && !line.empty()) {
		std::string::size_type pos = line.find(':');
		if (pos != std::string::npos) {
			std::string headerName = _trim(line.substr(0, pos));
			std::string headerValue = _trim(line.substr(pos + 1));

			_headers[headerName] = headerValue;
		}
	}
}

std::string HttpRequest::_trim(const std::string& str) {
	std::string::size_type start = str.find_first_not_of(" \t");
	std::string::size_type end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos) {
		return "";
	}

	return str.substr(start, end - start + 1);
}

void HttpRequest::displayRequest()
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}
	std::cout << "Body: " << _body << "\n[ici fin du body]" << std::endl;
}

std::string	HttpRequest::getMethod() const
{
	return _method;
}

std::string	HttpRequest::getPath() const
{
	return _path;
}

std::string	HttpRequest::get_http_version() const
{
	return _httpVersion;
}

std::map<std::string, std::string>	HttpRequest::get_headers() const
{
	return _headers;
}

std::string	HttpRequest::get_body() const
{
	return _body;
}

std::string	HttpRequest::get_root() const
{
	return _root;
}

bool	HttpRequest::get_is_referer() const
{
	return _is_referer;
}

#include "../include/webserv.hpp"

RequestHandler::RequestHandler(const HttpRequest &request, Server &serv)
{
	std::string method = request.getMethod();
	if (method == "GET" && is_method_allowed(request.getPath(), request.get_is_referer(), serv, "GET"))
		handleGetRequest(request, serv); // add
	else if (method == "POST" && is_method_allowed(request.getPath(), request.get_is_referer(), serv, "POST"))
		handlePostRequest(request, serv);
	else if (method == "DELETE" && is_method_allowed(request.getPath(), request.get_is_referer(), serv, "DELETE"))
		handleDeleteRequest(request, serv);
	else
		handleUnsupportedMethod(request, serv); // -> 501
}

RequestHandler::~RequestHandler(){}

void RequestHandler::handleGetRequest(const HttpRequest &request, Server &server)
{
	Response newResponse(request.getMethod(), request.get_http_version(), request.get_headers(), request.getPath(), 200, request.get_root(), request.get_body(), request.get_is_referer(), server);
	mime mime;
	std::map<std::string, std::string> mime_t;
	std::map<std::string, std::string>::iterator it;
	mime_t = mime.get_mime_types();
	std::string extension = "default";
	size_t pos = request.getPath().find(".");
	if (pos != std::string::npos)
		extension = request.getPath().substr(pos, -1);

	for (it = mime_t.begin(); it != mime_t.end(); it++)
	{
		if(request.getPath().find(extension) != std::string::npos)
			newResponse.setNewHeader("Content-Type", mime.get_content_type(extension));
	}
	newResponse.setNewHeader("Date", newResponse.getDateTime());
	newResponse.setNewHeader("Cache-Control", "max-age=0");
	newResponse.setHttpVersion("1.1");
	newResponse.setBody(newResponse.getCode());
	newResponse.setNewHeader("Content-Length", ft_tostring(newResponse.get_bodySize()));
	_response = newResponse.buildFullResponse();
}

bool check_boundary(const HttpRequest &request)
{
    std::map<std::string, std::string> map;
	map = request.get_headers();
    std::map<std::string, std::string>::const_iterator it;
    it = map.find("Content-Type");
    if (it != map.end())
	{
        size_t pos = it->second.find("boundary");

        if (pos != std::string::npos)
		{
            return true;
        }
    }
    return false;
}

bool RequestHandler::check_body_size(const HttpRequest &request, Server &server)
{
    std::map<std::string, std::string> map;
	map = request.get_headers();
    std::map<std::string, std::string>::const_iterator it;
    it = map.find("Content-Length");
    if (it != map.end())
	{
		_content_len = stoul(it->second);
        if (stoul(it->second) < server.getclientMaxBodySize())
			return true;
    }
    return false;;
}

std::string get_boundary(const std::string& line) {
    std::string boundary_start = "boundary=";
    size_t pos = line.find(boundary_start);
    if (pos != std::string::npos) {
        return line.substr(pos + boundary_start.size());
    }
    return "";
}

void	copy_file(std::string &input, std::string filename)
{
	std::ofstream outputfile("./uploads/copy_" + filename, std::ios::binary);
	if (outputfile.is_open())
	{
        outputfile.write(input.c_str(), input.size());
        outputfile.close();
	}
}

std::string get_filename()
{
	std::ifstream infile("tmp",  std::ios::binary);
	std::string line;

	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
		    std::string filename_start = "filename=\"";
		    size_t pos = line.find(filename_start);
			if (pos != std::string::npos)
			return line.substr(pos + filename_start.size(), (line.size() - 2) - (pos + filename_start.size()));
		}
    }
	infile.close();
    return "";

}


void RequestHandler::handlePostRequest(const HttpRequest &request, Server &server)
{
	int code = 201;
	std::string filename;
	if (check_boundary(request) == true)
	{
		if (check_body_size(request, server))
		{
			std::ifstream infile("tmp",  std::ios::binary);
			filename = get_filename();
			std::string bin_data;
			std::string line;
			std::string boundary;
			if (infile.is_open())
			{
				while (std::getline(infile, line))
				{
					std::string str;
					size_t pos = line.find("boundary=");
					if (pos != std::string::npos)
					{
						boundary = get_boundary(line);
						std::vector<char> fileContent;
						char byte;
						while (infile.read(&byte, sizeof(byte)))
						{
							fileContent.push_back(byte);
						}
						str = std::string(fileContent.begin(), fileContent.end());

						std::string delimiter = "--" + boundary;
						size_t start = str.find(delimiter);
						size_t pos = str.find("\r\n\r\n", start + delimiter.size());
						if(pos != std::string::npos)
						{
					        start = pos + 4;

					        size_t end = str.find("----", start);
					        if (end != std::string::npos)
							{
					            bin_data = str.substr(start, end - 2 - start);
							}
						}
						copy_file(bin_data, filename);
						infile.close();
					}
				}
			}
		}
		else
		{
			code = 413;
		}
	}
	else
	{
		if (request.get_body().size() < server.getclientMaxBodySize())
		{
			std::ofstream comments("./public/methods/comments.txt");
			comments.clear();
			comments << request.get_body();
			comments << "\nfin du commentaire\n";
			comments.close();
		}
	}
	if (filename.find(".sh") != std::string::npos)
	{
		Response newResponse(request.getMethod(), request.get_http_version(), request.get_headers(), "/uploads/copy_" + filename, code, request.get_root(), request.get_body(), request.get_is_referer(), server);
		newResponse.setNewHeader("Content-Type", "text/html;charset=utf-8");
		newResponse.setHttpVersion("1.1");
		newResponse.setBody(newResponse.getCode());
		newResponse.setNewHeader("Content-Length", ft_tostring(newResponse.get_bodySize()));
		_response = newResponse.buildFullResponse();
	}
	else
	{
		Response newResponse(request.getMethod(), request.get_http_version(), request.get_headers(), request.getPath(), code, request.get_root(), request.get_body(), request.get_is_referer(), server);
		newResponse.setNewHeader("Content-Type", "text/html;charset=utf-8");
		newResponse.setHttpVersion("1.1");
		newResponse.setBody(newResponse.getCode());
		newResponse.setNewHeader("Content-Length", ft_tostring(newResponse.get_bodySize()));
		_response = newResponse.buildFullResponse();
	}
	}


void RequestHandler::handleDeleteRequest(const HttpRequest &request, Server &server)
{
	std::cout << "-------- DELETE METHOD CALLED -----------" << std::endl;
	std::remove(("." + request.getPath()).c_str());
	Response newResponse(request.getMethod(), request.get_http_version(), request.get_headers(), request.getPath(), 204, request.get_root(), request.get_body(), request.get_is_referer(), server);
	newResponse.setNewHeader("Content-Type", "text/plain;charset=utf-8");
	newResponse.setNewHeader("Date", newResponse.getDateTime());
	newResponse.setHttpVersion("1.1");
	newResponse.setBody(newResponse.getCode());
	newResponse.setNewHeader("Content-Length", ft_tostring(newResponse.get_bodySize()));
	_response = newResponse.buildFullResponse();
}

void RequestHandler::handleUnsupportedMethod(const HttpRequest &request, Server &server)
{
	Response newResponse(request.getMethod(), request.get_http_version(), request.get_headers(), "/public/error/405/405.html", 405, request.get_root(), request.get_body(), request.get_is_referer(), server);

	newResponse.setNewHeader("Content-Type", "text/html;charset=utf-8");
	newResponse.setNewHeader("Date", newResponse.getDateTime());
	newResponse.setHttpVersion("1.1");
	newResponse.setNewHeader("Content-Length", ft_tostring(newResponse.get_bodySize()));
	newResponse.setBody(405);
	_response = newResponse.buildFullResponse();
}

std::string RequestHandler::getResponse() const
{
	return _response;
}

bool	RequestHandler::is_method_allowed(std::string path, bool referer, Server &serv, std::string method)
{
	if (referer == true && method == "GET")
		return true;
	std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_reverse_iterator it_loc; // TODO create typdef in hpp
	std::map<std::string, std::map<std::string, std::vector<std::string> > > locations = serv.getLocation();
	for (it_loc = locations.rbegin(); it_loc != locations.rend(); ++it_loc)
	{
		std::string loc = '/' + path;
		std::string to_find = it_loc->first;
		size_t	pos = loc.find(to_find, 0);
		if (pos != std::string::npos)
		{
			std::vector<std::string>::iterator it_method;
			for (it_method = serv.getLocationAttributes(it_loc->first, "method").begin(); it_method < serv.getLocationAttributes(it_loc->first, "method").end(); ++it_method)
			{
				if (*it_method == method)
				{
					return true;
				}
			}
			return false;
		}
	}
	std::vector<std::string>::iterator it_method;
	for (it_method = serv.getLocationAttributes("/", "method").begin(); it_method < serv.getLocationAttributes("/", "method").end(); ++it_method)
	{
		if (*it_method == method)
		{
			return true;
		}
	}
	return false;
}
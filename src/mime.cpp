#include "../include/webserv.hpp"

mime::mime()
{
    _mime_types[".html"] = "text/html;charset=utf-8";
    _mime_types[".htm"] = "text/html;charset=utf-8";
    _mime_types[".css"] = "text/css;charset=utf-8";
    _mime_types[".ico"] = "image/x-icon";
    _mime_types[".avi"] = "video/x-msvideo";
    _mime_types[".bmp"] = "image/bmp";
    _mime_types[".doc"] = "application/msword";
    _mime_types[".gif"] = "image/gif";
    _mime_types[".gz"] = "application/x-gzip";
    _mime_types[".ico"] = "image/x-icon";
    _mime_types[".jpg"] = "image/jpeg";
    _mime_types[".jpeg"] = "image/jpeg";
    _mime_types[".png"] = "image/png";
    _mime_types[".txt"] = "text/plain;charset=utf-8";
    _mime_types[".mp3"] = "audio/mp3";
    _mime_types[".pdf"] = "application/pdf";
    _mime_types[".js"] = "application/x-javascript; charset=utf-8";
    _mime_types["default"] = "text/html;charset=utf-8";
}

mime::~mime()
{
	// std::cout << 'Destructor called' << std::endl;
}

std::string mime::get_content_type(std::string extension)
{
	if(_mime_types.count(extension) != 0)
		return (_mime_types[extension]);
	else 
		return _mime_types["default"];
}

std::map<std::string, std::string> mime::get_mime_types() const
{
	return _mime_types;
}
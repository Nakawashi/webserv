#pragma once

#include "webserv.hpp"

class mime
{
	private:
		std::map<std::string, std::string> _mime_types;

	public:
		mime();
		~mime();

		std::string get_content_type(std::string extension);
		std::map<std::string, std::string> get_mime_types() const;
};


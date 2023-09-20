#include "../include/webserv.hpp"

int main(int argc, char const *argv[])
{
	if (argc > 2) {
		std::cerr << "Usage: " << argv[0] << " [config file]" << std::endl;
		return EXIT_FAILURE;
	}

	std::string configFile;
	if (argc == 2)
	{
		configFile = argv[1];
	}
	else
	{
		configFile = "./config/tartines.conf";
	}
	std::ifstream infile("tmp",  std::ios::binary);

	/////////////// METHOD 1 /////////////////////

	std::string str;
	std::string ligne;

	while (getline(infile, ligne))
	{
		str += ligne;
		str += '\n';
	}
	infile.close();
	std::ifstream	configFileStream(configFile.c_str());
	if (configFileStream.is_open())
	{
		InitServers init(configFileStream);
		configFileStream.close();
	}
	else
		throw std::runtime_error("\033[0;31mError : configuration file couldn't open. Doesn't exists or no space left.\033[0m");

	return EXIT_SUCCESS;
}


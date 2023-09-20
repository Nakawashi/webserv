
#include "../include/webserv.hpp"

CgiHandler::CgiHandler(const std::string &scriptPath, Server &server): scriptPath(scriptPath)
{
	(void) server;
}
CgiHandler::~CgiHandler(){}

std::string CgiHandler::executeCGI(const std::map<std::string, std::string>& _headers, Server &server, std::string formdata)
{
	(void)server;
	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);

		// On converti les headers dans la map en variables d'environnement pour le script CGI
		for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		{
			std::string env_var = it->first + "=" + it->second;
			envp.push_back(strdup(env_var.c_str()));
		}
		envp.push_back(NULL);

		const char *inter;
		std::vector<std::string> cgi_ext = server.getLocationAttributes("/cgi-bin", "cgi_extension");
		std::vector<std::string> cgi_path = server.getLocationAttributes("/cgi-bin", "cgi_path");
		for(unsigned long i = 0; i < cgi_ext.size(); i++)
		{
			if (scriptPath.find(cgi_ext[i]) != std::string::npos)
				inter = cgi_path[i].c_str();
		}
		char *path[] = {(char*)inter, strdup(scriptPath.c_str() + 1), strdup(formdata.c_str()), NULL};
		execve(path[0], path, &envp[0]);
		perror("Script problem :");
		std::vector<char *>::iterator it;
		for (it = envp.begin(); it != envp.end(); ++it)
		{
			free(*it);
		}

		exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipefd[1]);
		char buffer[1024];
		std::string result;
		while (read(pipefd[0], buffer, sizeof(buffer) - 1) > 0)
		{
			result += buffer;
			memset(buffer, 0, sizeof(buffer));
		}
		close(pipefd[0]);
		waitpid(pid, NULL, 0);

		return result;
	}
}

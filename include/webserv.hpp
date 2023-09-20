
#pragma once

# include <iostream>
# include <fstream>
# include <sstream>
# include <cstring>
# include <string>
# include <cstdio>
# include <ctime>
# include <fcntl.h>
# include <unistd.h>
# include <algorithm>
# include <iterator>
# include <vector>
# include <map>
# include <set>
# include <list>
# include <errno.h>
# include <poll.h>
# include <signal.h>
# include <ctype.h>

#include <iterator>

# include <dirent.h>

# include <sys/wait.h>
# include <sys/socket.h>
# include <netinet/in.h>

#include "CgiHandler.hpp"
#include "HttpRequest.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "RequestHandler.hpp"
#include "utils.hpp"
#include "mime.hpp"
#include "InitServers.hpp"

class HttpServer;
class Response;
class HttpRequest;
class RequestHandler;
class HttpCommand;

#define RED "\033[1m\033[31m"
#define YELLOW  "\033[1m\033[33m"
#define RESET   "\033[0m"

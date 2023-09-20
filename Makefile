CC = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRCDIR = src
INCDIR = include
BUILDDIR = build

NAME = webserv

SRCEXT = cpp
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.$(SRCEXT),$(BUILDDIR)/%.o,$(SOURCES))
INCPATHS = -I$(INCDIR)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCPATHS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCPATHS) -c -o $@ $<

run: $(NAME)
	./$(NAME)

clean:
	$(RM) -r $(BUILDDIR)

re: fclean all

fclean: clean
	$(RM) -rf $(NAME)

.PHONY: all clean re fclean run
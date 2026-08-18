SRC = src/main.cpp src/Client/Client.cpp src/Channel/Channel.cpp src/Utils/Parser.cpp
NAME = ft_ircserv
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
OBJ = $(SRC:%.cpp=%.o)
RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(OBJ) -o $(NAME)
	
%.o: %.cpp
	c++ $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) -f $(NAME)

re : fclean all

.PHONY: all clean fclean re


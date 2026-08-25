# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tseche <tseche@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 23:39:11 by tseche            #+#    #+#              #
#    Updated: 2026/08/25 15:22:06 by tseche           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Colors ---
RED    := \033[0;31m
GREEN  := \033[0;32m
YELLOW := \033[0;33m
BLUE   := \033[0;34m
PURPLE := \033[0;35m
CYAN   := \033[0;36m
NC     := \033[0m # No Color

# --- Compilation ---
NAME = ircserv
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
CC	= c++

# --- Directory ---

SRC_DIR = ./src
DIR = $(SRC_DIR)

OBJ_DIR = ./obj/$(DIR)

Channel = Channel.cpp
Client = Client.cpp
Server = Server.cpp
Command =	invite.cpp \
			join.cpp \
			kick.cpp \
			mode.cpp \
			nick.cpp \
			pass.cpp \
			privmsg.cpp \
			topic.cpp \
			user.cpp \


SUB_DIR = parsing utils hooks raycast player



VPATH := $(DIR) \
	$(addprefix $(DIR)/, $(SUB_DIR))

SRCS = main.cpp $(Command) $(Channel) $(Client) $(Server)

# --- LOGIC ---

OBJS = ${SRCS:%.cpp=$(OBJ_DIR)/%.o}

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(BLUE)🔗 Creating Executable $@...$(NC)\n"
	@$(CC) $(OBJS) -o $(NAME)
	@printf "$(GREEN)✅ Created $@$(NC)\n"
	
$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CXXFLAGS) -c $< -o $@
	@printf "\r\033[2K$(CYAN)📝 Compiling   %s$(R)" "$<"
	
clean:
	@printf "$(RED)🧹 Cleaning...$(NC)\n"
	@rm -rf ./obj/

fclean: clean
	@printf "$(RED)🗑️ Removing Executable $(NAME)...$(NC)\n"
	@rm -rf $(NAME)

re: fclean all

help:
	@printf "$(PURPLE)=== Makefile Help ===$(NC)\n"
	@printf "$(PURPLE)all:$(NC) Build the library\n"
	@printf "$(PURPLE)clean:$(NC) Remove object files\n"
	@printf "$(PURPLE)fclean:$(NC) Remove object files and library\n"
	@printf "$(PURPLE)re:$(NC) Rebuild the library from scratch\n"
	@printf "$(PURPLE)help:$(NC) Show this help message\n"
	@printf "$(PURPLE)credit:$(NC) Show the people who help with this project\n"
	
.PHONY: all libs clean bonus libclean fclean re help
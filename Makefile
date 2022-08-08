SHELL = bash


# ===== Target & FLAGS =====
NAME		:= webserv

CXX			:= c++
CXXFLAGS	:= -std=c++98 -pedantic -Wall -Wextra -Werror
RM			:= rm -rf


# ===== Sources =====
INCD			:=	-I ./include

SRCS_DIR	:=	src
SRCS_NAME	:=	main.cpp						\
							Log.cpp							\
							Scanner.cpp					\
							Parser.cpp					\
							Config.cpp					\
							ConfigCommon.cpp		\
							ConfigLocation.cpp	\
							ConfigServer.cpp		\
							Server.cpp					\
							Processor.cpp				\
							Request.cpp					\
							Utilities.cpp				\
							Webserv.cpp					\
							Response.cpp				\
							FileManager.cpp
SRCS			:= $(addprefix $(SRCS_DIR)/, $(SRCS_NAME))

OBJS			:=	$(SRCS:.cpp=.o)


# ===== Style =====
FG_TEXT		= \033[38;5;85m
FG_DEBUG	= \033[48;5;2m
CL_RESET	= \033[0m
CL_BOLD		= \033[1m
CL_BRESET	= \033[21m


# ===== Rules =====
.PHONY : all clean fclean re

all : $(NAME)

clean :
	@$(RM) ./src/*.o
	@echo -e "🗑 Remove $(CL_BOLD)$(NAME)'s OBJs$(CL_RESET) ... Done"

fclean : clean
	@$(RM) $(NAME)
	@echo -e "🗑 Remove $(CL_BOLD)$(NAME)$(CL_RESET) ... Done"

re : fclean
	@make all

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) -g
	@echo -e "  $(CL_RESET)$(FG_TEXT)Done$(CL_RESET)"

INDEX = 0
%.o : %.cpp
	$(eval INDEX = $(shell expr $(INDEX) + 1))
	@if [ $(INDEX) = 1 ] ; then \
		echo -en "$(FG_TEXT)Creating $(CL_BOLD)$(NAME) ◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎︎︎︎◼︎︎︎︎︎︎" ; \
	else \
		echo -en "$(FG_TEXT)◼︎︎︎︎︎︎◼︎︎︎︎︎◼︎︎︎︎︎︎◼︎︎︎︎︎" ; \
	fi
	@$(CXX) $(CXXFLAGS) $(INCD) -c $< -o $@ -g


# ===== Custom =====
.PHONY : run test

run : all
	@./$(NAME)

test : all
	@rm -f www/put_test/file_should_exist_after www/YoupiBanane/youpi.bla
	@touch www/YoupiBanane/youpi.bla
	@./$(NAME) ./config/test.conf
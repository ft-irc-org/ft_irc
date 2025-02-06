NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp \
        ./server/Server.cpp \
        ./server/ServerConfig.cpp \
        ./client/Client.cpp \
        ./client/Auth.cpp \
        ./channel/Channel.cpp \
        ./command/Dispatcher.cpp \
        ./command/Join.cpp \
        ./command/Kick.cpp \
        ./command/Mode.cpp \
        ./command/Nick.cpp \
        ./command/Ping.cpp \
        ./command/Pong.cpp \
        ./command/Privmsg.cpp \
        ./command/Quit.cpp \
        ./message/Message.cpp \
		./command/Pass.cpp \
		./command/User.cpp \
		./command/Whois.cpp \
        ./command/Part.cpp \
        ./command/Topic.cpp \
        ./command/Invite.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++17
INCLUDES = -I include

SRCDIR = source
INCDIR = include

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = montador

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
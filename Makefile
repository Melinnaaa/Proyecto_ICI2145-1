CC=gcc
NAME=pokemon
SRC=$(wildcard src/*.c)
INCLUDES=-Iinclude
OBJS=$(patsubst src/%.c, out/%.o, $(SRC))
INCLUDE=$(wildcard include/*)

all: prepare $(OBJS) $(NAME) 


debug:
	gcc main.c src/*.c -Iinclude -DDEBUG  -g

$(NAME): $(OBJS) $(INCLUDE) main.c
	$(CC) $(INCLUDES) main.c $(OBJS) -o $(NAME) -g -Wall -Werror -std=c99 -lm

prepare:
	mkdir -p src out include 


out/%.o: src/%.c
	$(CC) -o $@ -c $^ $(INCLUDES) -g -Wall -Werror -std=c99

clean:
	rm $(NAME) $(OBJS)

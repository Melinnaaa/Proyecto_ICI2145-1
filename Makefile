CC=gcc
NAME=pokemon

SRC=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, out/%.o, $(SRC))

INCLUDES=-Iinclude
INCLUDE=$(wildcard include/*)

TEST_SRC=$(wildcard src/tests/*.c)
TEST_BIN=$(patsubst src/tests/%.c, tests/%, $(TEST_SRC))

all: prepare $(OBJS) $(NAME) test

tests/%: src/tests/%.c $(OBJS)
	$(CC) $(INCLUDES) -o $@ $^ -g -Wall -Werror -std=c99

debug:
	gcc main.c src/*.c -Iinclude -DDEBUG  -g

test: $(TEST_BIN) 

$(NAME): $(OBJS) $(INCLUDE) main.c
	$(CC) $(INCLUDES) main.c $(OBJS) -o $(NAME) -g -Wall -Werror -std=c99 -lm

prepare:
	mkdir -p src/tests out include tests


out/%.o: src/%.c
	$(CC) -o $@ -c $^ $(INCLUDES) -g -Wall -Werror -std=c99

clean:
	rm $(NAME) $(OBJS)


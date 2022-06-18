CC=gcc
NAME=pokemon

SRC=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, out/%.o, $(SRC))

TESTS=$(wildcard tests/*.c)
TESTOBJS=$(patsubst tests/%.c, out/tests/%.o, $(TESTS))

INCLUDE=$(wildcard include/*)
INCLUDES=-Iinclude

all: prepare $(OBJS) $(NAME)  test



$(NAME): $(OBJS) $(INCLUDE) main.c
	$(CC) $(INCLUDES) main.c $(OBJS) -o $(NAME) -g -Wall -Werror -std=c99 -lm

# out: almacena objetos compilados
# out/tests: almacena tests compilados
# src: almacera codigo fuente
# include: almacena headers
prepare:
	mkdir -p src out/tests include 


# compila todos el código en src a out
out/%.o: src/%.c 
	$(CC) -o $@ -c $^ $(INCLUDES) -g -Wall -Werror -std=c99

# compila todos los tests
out/tests/%.o: tests/%.c
	$(CC) -o $@ -c $^ $(INCLUDES)  -g -Wall -Werror -std=c99

test: prepare $(INCLUDE) $(TESTOBJS) $(OBJS)
	$(CC) -o pokemon_test $(OBJS) $(TESTOBJS) -g -Wall -Werror -std=c99

# borra todo
clean:
	rm $(NAME) $(OBJS)


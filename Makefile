


VPATH = src/
CC=gcc
FLAGS= -Wall -Wextra -g -Wno-int-to-pointer-cast -Wno-unused-parameter
LIBS= -lGL -lGLEW -lglfw3 -ldl -lX11 -lpthread -lm

SRC  = $(wildcard *.c src/*c src/**/*.c src/**/**/*.c src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)


FOO := MACOS
FOO := $(patsubst %OS,%,$(FOO))


.PHONY: compile

%.o: %.c **/*.h
	@echo $<
	@$(CC) -o $@ -c $< $(FLAGS)

game: $(OBJ)
	@$(CC) -o program $^ $(FLAGS) $(LIBS)
	@./program

clean:
	rm $(OBJ)
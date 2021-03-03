VPATH = src/
CC=gcc
FLAGS= -Wall -Wextra -g -Wno-unused-parameter -Wno-int-to-pointer-cast
LIBS= -lGL -lGLEW -lglfw3 -ldl -lX11 -lpthread -lm -lcglm

SRC  = $(wildcard *.c src/*c src/**/*.c src/**/**/*.c src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)


.PHONY: compile

%.o: %.c **/*.h
	@echo $< 
	@$(CC) -o $@ -c $< $(FLAGS)

game: $(OBJ)
	@$(CC) -o program $^ $(FLAGS) $(LIBS)
	@./program

clean:
	rm $(OBJ)
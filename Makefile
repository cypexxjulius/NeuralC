VPATH = src/
CC=gcc
FLAGS = -Wall -Wextra -g -Wno-unused-parameter -Wno-int-to-pointer-cast -std=c11
FLAGS += -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include -Ilib/stb
LIBS = -lm 
ifeq ($(OS),)
OS = $(shell uname -s)
endif 

ifeq ($(OS), Linux)
LIBS += -lpthread -ldl
endif 


ifeq ($(OS), Windows_NT)
VENDOR_LIBS = lib/glfw/src/libglfw3.lib lib/glad/src/glad.o lib/cglm/libcglm.lib
endif 

ifeq ($(OS), Linux)
VENDOR_LIBS = lib/glfw/src/libglfw3.a lib/glad/src/glad.o lib/cglm/libcglm.a
endif 

SRC  = $(wildcard *.c src/*c src/**/*.c src/**/**/*.c src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)


default: NeuralEngine

# Making the vendor libraries

# glad
lib/glad/src/glad.o:
	cd lib/glad; $(CC) -o src/glad.o -Iinclude -c src/glad.c


ifeq ($(OS), Windows_NT)
# cglm
lib/cglm/libcglm.lib:
	@cd lib/cglm && cmake . -DCGLM_STATIC=ON && make

# glfw
lib/glfw/src/libglfw3.lib:
	@cd lib/glfw && cmake -G "MinGW Makefiles" -S . -B build && make
endif


ifeq ($(OS), Linux)
# cglm
lib/cglm/libcglm.a:
	@cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
# glfw
lib/glfw/src/libglfw3.a:
	@cd lib/glfw && cmake . && make

endif



%.o: %.c
	@echo $< 
	@$(CC) -o $@ -c $< $(FLAGS)

NeuralEngine: $(VENDOR_LIBS) $(OBJ) 
	@$(CC) -o program $(FLAGS) $(OBJ) $(VENDOR_LIBS) $(LIBS)  
	@./program


clean:
	rm $(OBJ)

cleanLib:
	rm $(VENDOR_LIBS)
VPATH = src/
CC=gcc
FLAGS = -Wall 
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
FLAGS += $(INCLUDE)

#--- Platform detection ---

ifeq ($(OS),)
OS = $(shell uname -s)
endif 


ifeq ($(OS), Linux)

LIBS = -lpthread -ldl -lm
FLAGS += -Wextra -Wno-unused-parameter -Wno-int-to-pointer-cast -g -std=c11 
VENDOR_LIBS = NeuralEngine/lib/glfw/src/libglfw3.a NeuralEngine/lib/glad/src/glad.o NeuralEngine/lib/cglm/libcglm.a
NAMEPROGRAM = -o program
INCLUDE = -isystem NeuralEngine/lib/cglm/include -isystem NeuralEngine/lib/glad/include -isystem NeuralEngine/lib/glfw/include -isystem NeuralEngine/lib/stb -I  NeuralEngine/


endif 


NEURAL_SRC  = $(wildcard NeuralEngine/*.c NeuralEngine/src/*c NeuralEngine/src/**/*.c NeuralEngine/src/**/**/*.c NeuralEngine/src/**/**/**/*.c)

OBJ  = $(NEURAL_SRC:.c=.o)

#--------------- Compiling NeuralEngine ----------------------------

default: Sandbox/program run

NeuralEngine/bin/NeuralEngine.a: $(OBJ) $(VENDOR_LIBS)
	rm NeuralEngine/bin/NeuralEngineCore.a NeuralEngine/bin/NeuralEngine.a
	@ar -rs NeuralEngine/bin/NeuralEngineCore.a $(OBJ) NeuralEngine/lib/glad/src/glad.o 
	@ar -rcT NeuralEngine/bin/NeuralEngine.a NeuralEngine/bin/NeuralEngineCore.a NeuralEngine/lib/glfw/src/libglfw3.a NeuralEngine/lib/cglm/libcglm.a

%.o: %.c
	@echo $< 
	@$(CC) $(FLAGS) -o $@ -c $< 

#--- Vendor Libs ---

#glad
NeuralEngine/lib/glad/src/glad.o:
	@cd NeuralEngine/lib/glad && $(CC) -o src/glad.o -lm -Iinclude $(FLAGS) -c src/glad.c

# cglm
NeuralEngine/lib/cglm/libcglm.a:
	@cd NeuralEngine/lib/cglm && cmake . -DCGLM_STATIC=ON && make
# glfw
NeuralEngine/lib/glfw/src/libglfw3.a:
	@cd NeuralEngine/lib/glfw && cmake . && make


Sandbox/program: NeuralEngine/bin/NeuralEngine.a Sandbox/main.c
	$(info Compiling Sandbox program)
	@$(CC) -o Sandbox/program Sandbox/main.c -isystem NeuralEngine/src -L NeuralEngine/bin/ -l:NeuralEngine.a $(LIBS) $(FLAGS)


clean:
	rm $(OBJ)

cleanLib:
	rm $(VENDOR_LIBS)


run:
	@cd Sandbox/ && ./program


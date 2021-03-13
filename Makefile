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

ifeq ($(OS), Windows_NT)

CC=cl
FLAGS += /W 0 /MDd
VENDOR_LIBS = NeuralEngine/lib/glfw/build/src/Debug/glfw3.lib NeuralEngine/lib/cglm/build/Debug/cglm.lib NeuralEngine/lib/glad/glad.obj
LIBS = kernel32.lib user32.lib gdi32.lib shell32.lib
INCLUDE = -INeuralEngine/lib/cglm/include -INeuralEngine/lib/glad/include -INeuralEngine/lib/glfw/include -INeuralEngine/lib/stb -I"$(CURDIR)/NeuralEngine"

endif 

NEURAL_SRC  = $(wildcard NeuralEngine/*.c NeuralEngine/src/*c NeuralEngine/src/**/*.c NeuralEngine/src/**/**/*.c NeuralEngine/src/**/**/**/*.c)


ifeq ($(OS), Windows_NT)
NEURAL_OBJ  = $(NEURAL_SRC:.c=.obj)
else 
OBJ  = $(NEURAL_SRC:.c=.o)
endif 


#--------------- Compiling NeuralEngine ----------------------------

ifeq ($(OS), Windows_NT)

default: Sandbox/program.exe run

NeuralEngine/bin/NeuralEngine.lib: $(VENDOR_LIBS) $(NEURAL_OBJ)  
	$(info Compiling NeuralEngine.lib)
	@lib /nologo /out:NeuralEngine/bin/NeuralEngine.lib $(VENDOR_LIBS) $(NEURAL_OBJ)   

NeuralEngine/%.obj: %.c
	@$(CC) /FoNeuralEngine/$@ /W0 /D "_CRT_SECURE_NO_WARNINGS" /c /Tc $< $(FLAGS)

#--- Vendor Libs ---

# glad
NeuralEngine/lib/glad/glad.obj:
	@cd NeuralEngine/lib/glad && $(CC) -Iinclude -c src/glad.c /MD

# cglm
NeuralEngine/lib/cglm/build/Debug/cglm.lib:
	@cd NeuralEngine/lib/cglm && cmake . -DCGLM_STATIC=ON -B build && cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64

# glfw
NeuralEngine/lib/glfw/build/src/Debug/glfw3.lib:
	@cd NeuralEngine/lib/glfw && cmake .  -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -B build && cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64



else 

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

endif 




ifeq ($(OS), Windows_NT) 

Sandbox/program.exe: NeuralEngine/bin/NeuralEngine.lib Sandbox/main.c
	$(info Compiling Sandbox program)
	@cl /FeSandbox/program.exe /MD $(INCLUDE) Sandbox/main.c /link NeuralEngine/bin/NeuralEngine.lib /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:MSVCRTD $(LIBS) 

run:
	$(info # Starting Sandbox program)
	@cd Sandbox/ && program.exe

else 

Sandbox/program: NeuralEngine/bin/NeuralEngine.a Sandbox/main.c
	$(info Compiling Sandbox program)
	@$(CC) -o Sandbox/program Sandbox/main.c -isystem NeuralEngine/src -L NeuralEngine/bin/ -l:NeuralEngine.a $(LIBS) $(FLAGS)


clean:
	rm $(OBJ)

cleanLib:
	rm $(VENDOR_LIBS)


run:
	@cd Sandbox/ && ./program

endif 


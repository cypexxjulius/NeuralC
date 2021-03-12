VPATH = src/
CC=gcc
FLAGS = -Wall 
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

INCLUDE = -INeuralEngine/lib/cglm/include -INeuralEngine/lib/glad/include -INeuralEngine/lib/glfw/include -INeuralEngine/lib/stb -I"$(CURDIR)/NeuralEngine"
FLAGS += $(INCLUDE)

#--- Platform detection ---

ifeq ($(OS),)
OS = $(shell uname -s)
endif 

ifeq ($(OS), Linux)

LIBS = -lpthread -ldl -lm
FLAGS += -Wextra -Wno-unused-parameter -Wno-int-to-pointer-cast -g -std=c11 
VENDOR_LIBS = lib/glfw/src/libglfw3.a lib/glad/src/glad.o lib/cglm/libcglm.a
NAMEPROGRAM = -o program

endif 

ifeq ($(OS), Windows_NT)

CC=cl
FLAGS += /W 0 /MDd
VENDOR_LIBS = NeuralEngine/lib/glfw/src/Debug/glfw3.lib NeuralEngine/lib/cglm/Debug/cglm.lib NeuralEngine/lib/glad/glad.obj
LIBS = kernel32.lib user32.lib gdi32.lib shell32.lib

endif 

NEURAL_SRC  = $(wildcard NeuralEngine/*.c NeuralEngine/src/*c NeuralEngine/src/**/*.c NeuralEngine/src/**/**/*.c NeuralEngine/src/**/**/**/*.c)


ifeq ($(OS), Windows_NT)
NEURAL_OBJ  = $(NEURAL_SRC:.c=.obj)
else 
OBJ  = $(NEURAL_SRC:.c=.o)
endif 

default: Sandbox/program.exe run

#--------------- Compiling NeuralEngine ----------------------------




ifeq ($(OS), Windows_NT)

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
NeuralEngine/lib/cglm/Debug/cglm.lib:
	@cd NeuralEngine/lib/cglm && cmake . -DCGLM_STATIC=ON -B build && msbuild ALL_BUILD.vcxproj -property:Platform=x64

# glfw
NeuralEngine/lib/glfw/src/Debug/glfw3.lib:
	@cd NeuralEngine/lib/glfw && cmake .  -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -B build && msbuild ALL_BUILD.vcxproj -property:Platform=x64



else 

NeuralEngine: $(VENDOR_LIBS) $(OBJ) 
	@$(CC) $(FLAGS) $(VENDOR_LIBS) $(OBJ)

NeuralEngine/%.o: %.c
	@echo $< 
	@$(CC) $(FLAGS) -o NeuralEngine/$@ -c $< 

#--- Vendor Libs ---

#glad
NeuralEngine/lib/glad/src/glad.o:
	@cd NeuralEngine/lib/glad; $(CC) -o NeuralEngine/src/glad.o -Iinclude -c NeuralEngine/src/glad.c

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

clean:
	rm $(OBJ)

cleanLib:
	rm $(VENDOR_LIBS)


run:
	cd Sandbox/ && program

endif 


VPATH = src/
CC=gcc
FLAGS = -Wall 
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

FLAGS += -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include -I"$(CURDIR)"

ifeq ($(OS),)
OS = $(shell uname -s)
endif 

ifeq ($(OS), Linux)
LIBS = -lpthread -ldl -lm
FLAGS += -Wextra -Wno-unused-parameter -Wno-int-to-pointer-cast -g -std=c11 
VENDOR_LIBS = lib/glfw/src/libglfw3.a lib/glad/src/glad.o lib/cglm/libcglm.a
NAMEPROGRAM = -o program

SRC  = $(wildcard *.c src/*c src/**/*.c src/**/**/*.c src/**/**/**/*.c)

endif 

ifeq ($(OS), Windows_NT)
CC=cl
FLAGS += /W 0 /MD
VENDOR_LIBS = lib/glfw/src/Debug/glfw3.lib lib/cglm/Debug/cglm.lib lib/glad/glad.obj
LIBS = user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

SRC  = $(wildcard src/*.c src/**/*.c src/**/**/*.c src/**/**/**/*.c)

endif 



ifeq ($(OS), Windows_NT)
OBJ  = $(SRC:.c=.obj)
else 
OBJ  = $(SRC:.c=.o)
endif 

default: program.exe

# Making the vendor libraries




ifeq ($(OS), Windows_NT)
# glad
lib/glad/glad.obj:
	@cd lib/glad && $(CC) -Iinclude -c src/glad.c /MD

# cglm
lib/cglm/Debug/cglm.lib:
	@cd lib/cglm && cmake . -DCGLM_STATIC=ON -B build && msbuild ALL_BUILD.vcxproj -property:Platform=x64

# glfw
lib/glfw/src/Debug/glfw3.lib:
	@cd lib/glfw && cmake .  -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -B build && msbuild ALL_BUILD.vcxproj -property:Platform=x64

%.obj: %.c
	$(CC) /Fo"$@" /W0 /D "_CRT_SECURE_NO_WARNINGS" /c /Tc $< $(FLAGS)

endif


ifeq ($(OS), Linux)
#glad
lib/glad/src/glad.o:
	@cd lib/glad; $(CC) -o src/glad.o -Iinclude -c src/glad.c

# cglm
lib/cglm/libcglm.a:
	@cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
# glfw
lib/glfw/src/libglfw3.a:
	@cd lib/glfw && cmake . && make

# C Files 

%.o: %.c
	@echo $< 
	$(CC) $(FLAGS) -o $@ -c $< 

endif





ifeq ($(OS), Windows_NT)


NeuralEngine.lib: $(VENDOR_LIBS) $(OBJ)  
	lib /nologo /out:NeuralEngine.lib $(LIBS) $(VENDOR_LIBS) $(OBJ)   

program.exe: NeuralEngine.lib main.c
	cl /Feprogram.exe main.c $(FLAGS) /link NeuralEngine.lib 

clean:
	del **\*.obj

cleanLib:
	erase $(VENDOR_LIBS)


else 
NeuralEngine: $(VENDOR_LIBS) $(OBJ) 
	$(CC) $(FLAGS) $(LIBS) $(VENDOR_LIBS) $(OBJ)

clean:
	rm $(OBJ)

cleanLib:
	rm $(VENDOR_LIBS)
endif 


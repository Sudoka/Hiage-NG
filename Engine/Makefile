OUTPUT=bin/hiage.so

#Linker
LDFLAGS=-g -shared
LDLIBS=-lGL -lSDL -lxerces-c

#Compiler
CXXFLAGS=-Iinclude -g -fpic -std=c++11
#CXX=g++
CXX=clang

OBJ=$(patsubst src/%.cpp,obj/%.o,$(shell ls src/*.cpp))

THIRDPARTY=lib/libxml2.a

#OBJ=obj/Display.o  \
	obj/Game.o  \
	obj/GameState.o \
	obj/InputManager.o  \
	obj/Log.o  \
	obj/ResourceManager.o  \
	obj/Timer.o  \
	obj/Vector.o

all:$(OUTPUT)

lib/libxml2.a:
	rm -rf src/third-party/libxml2-2.7.2 && cd src/third-party/ && tar -xzf libxml2-2.7.2.tar.gz && cd libxml2-2.7.2 && ./configure && $(MAKE)
	cp src/third-party/libxml2-2.7.2/.libs/libxml2.a lib

obj/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ) $(THIRDPARTY)
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LDLIBS) 

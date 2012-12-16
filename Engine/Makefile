OUTPUT=bin/libhiage.so

#Linker
LDFLAGS=-g -shared
LDLIBS=-Llib -lGL -lSDL -lxml2 -lboost_system -lboost_filesystem

#Compiler
CXXFLAGS=-Iinclude $(shell xml2-config --cflags) -g -fpic -std=c++11 -Wall -Werror
CXX=g++
#CXX=clang

OBJ=$(patsubst src/%.cpp,obj/%.o,$(shell ls src/*.cpp))

#THIRDPARTY=$(LIBXML)

all:$(OUTPUT)

clean:
	rm -r bin obj

purge: clean
	rm -r lib 

obj/%.o:src/%.cpp 
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ) #$(THIRDPARTY) 
	@mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LDLIBS)

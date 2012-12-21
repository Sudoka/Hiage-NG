OUTPUT=bin/libhiage.so

#Linker
LDFLAGS=-g -shared
LDLIBS=-Llib -lGL -lSDL -lxml2 -lboost_system -lboost_filesystem

#Compiler
CXXFLAGS=-Iinclude $(shell xml2-config --cflags) -g -fpic -std=c++11 -Wall -Werror
CXX=g++

DEPEND=$(patsubst src/%.cpp,work/%.d,$(shell ls src/*.cpp))
SRC=$(shell ls src/*.cpp)
OBJ=$(patsubst src/%.cpp,work/%.o,$(shell ls src/*.cpp))
HEADERS=$(shell ls include/*.h)

all:$(OUTPUT)

clean:
	rm -rf bin $(OBJ) $(DEPEND)

purge: clean
	rm -r lib 

work/%.o:src/%.cpp 
	@mkdir -p work
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ) 
	@mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LDLIBS)

depend: $(DEPEND)

work/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(patsubst src/%.cpp,work/%.o,$<) > $@

-include $(DEPEND)

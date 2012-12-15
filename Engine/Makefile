OUTPUT=bin/libhiage.so

#Linker
LDFLAGS=-g -shared
LDLIBS=-Llib -lGL -lSDL -lxml2 -lboost_system -lboost_filesystem

#Compiler
CXXFLAGS=-Iinclude $(shell xml2-config --cflags) -g -fpic -std=c++11
CXX=g++
#CXX=clang

OBJ=$(patsubst src/%.cpp,obj/%.o,$(shell ls src/*.cpp))

#LIBXML=lib/libxml2.so

#THIRDPARTY=$(LIBXML)

all:$(OUTPUT)

clean:
	rm -r bin obj

purge: clean
	rm -r lib src/third-party/libxml2-2.7.2

$(LIBXML):
	@mkdir lib
	rm -rf src/third-party/libxml2-2.7.2 && cd src/third-party/ && tar -xzf libxml2-2.7.2.tar.gz && cd libxml2-2.7.2 && ./configure && $(MAKE)
	cp src/third-party/libxml2-2.7.2/.libs/libxml2.so lib

obj/%.o:src/%.cpp 
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ) #$(THIRDPARTY) 
	@mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LDLIBS)

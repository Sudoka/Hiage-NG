OUTPUT=bin/libhiage.so

#Linker
LDFLAGS=-g -shared
LDLIBS=-Llib -lGL -lSDL -lxml2 -lboost_system -lboost_filesystem

#Compiler
CXXFLAGS=-Iinclude -g -fpic -std=c++11
CXX=g++
#CXX=clang

OBJ=$(patsubst src/%.cpp,obj/%.o,$(shell ls src/*.cpp))

LIBXML=lib/libxml2.a

THIRDPARTY=$(LIBXML)

all:$(OUTPUT)

clean:
	rm -r bin obj

$(LIBXML):
	rm -rf src/third-party/libxml2-2.7.2 && cd src/third-party/ && tar -xzf libxml2-2.7.2.tar.gz && cd libxml2-2.7.2 && ./configure && $(MAKE)
	cp src/third-party/libxml2-2.7.2/.libs/libxml2.a lib

obj/%.o:src/%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj:
	mkdir obj

bin:
	mkdir bin

$(OUTPUT): $(OBJ) $(THIRDPARTY) bin
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LIBXML) $(LDLIBS)

OUTPUT=bin/testing

#Linker
LDFLAGS+=-g 
LDLIBS+=-L../Engine/bin -lhiage -lboost_system 

#Compiler
CXXFLAGS+=-I../Engine/include -g -std=c++11
CXX=g++
#CXX=clang

OBJ=$(patsubst src/%.cpp,obj/%.o,$(shell ls src/*.cpp))

THIRDPARTY=../Engine/bin/libhiage.a

all:$(OUTPUT)

clean:
	rm -r bin obj

../Engine/bin/libhiage.a:
	$(MAKE) -C ../Engine

obj/%.o:src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ) $(THIRDPARTY) 
	@mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJ) $(LDLIBS) 

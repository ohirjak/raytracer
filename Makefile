# @file: Makefile
# @project: raytracer
# @author: Ondrej Hirjak, 2009-2010
# @description: 

CPP = g++
CXXFLAGS = -pipe -Wall -g
LDFLAGS = -pipe
LIBS =
SRC = main.cpp geometries.cpp scene.cpp
OUT = raytracer

.PHONY: all build optimized tags clean distclean

all: build

build: $(OUT)

optimized:
	make CXXFLAGS="-pipe -Wall -msse3 -O2"

run: build
	./$(OUT)

$(OUT): $(SRC:.cpp=.o)
	$(CPP) $(LDFLAGS) $(LIBS) -o $(OUT) $(SRC:.cpp=.o)

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c -o $@ $<

tags:
	ctags $(SRC) *.h

clean:
	-rm $(OUT) $(SRC:.cpp=.o)

distclean: clean
	-rm tags core output.bmp


# @file: Makefile
# @project: raytracer
# @author: Ondrej Hirjak, 2009-2010
# @description: Project makefile.

CPP = g++
CXXFLAGS = -pipe -Wall -g
LDFLAGS = -pipe
LIBS =
SRC = colors.cpp geometries.cpp main.cpp render.cpp scene.cpp
OUT = raytracer

.PHONY: all build optimized tags clean distclean

all:
	make -j3 build

build: $(OUT)

optimized:
	make CXXFLAGS="-pipe -Wall -msse3 -O2"

run: build
	./$(OUT)

show_output:
	eog output.bmp

show_debug:
	eog debug.bmp

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


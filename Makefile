# @file: Makefile
# @project: raytracer
# @author: Ondrej Hirjak, 2009-2010
# @description: Project makefile.

# setting

CPP = g++
CXXFLAGS = -pipe -Wall -g
LDFLAGS = -pipe
LIBS = -lpthread
SRC = colors.cpp geometries.cpp main.cpp render.cpp renderGPU.cpp renderGrid.cpp renderMT.cpp scene.cpp
OUT = raytracer
OUTPUTS = output.bmp debug.bmp

.PHONY: all build optimized tags clean distclean

# building

all:
	make -j3 build

build: $(OUT)

optimized:
	make CXXFLAGS="-pipe -Wall -msse3 -O2"

$(OUT): $(SRC:.cpp=.o)
	$(CPP) $(LDFLAGS) $(LIBS) -o $(OUT) $(SRC:.cpp=.o)

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c -o $@ $<

tags:
	ctags $(SRC) *.h

clean:
	-rm $(OUT) $(SRC:.cpp=.o)

distclean: clean
	-rm tags core $(OUTPUTS)

# running & results

run: build
	./$(OUT)

show_output:
	eog output.bmp

show_debug:
	eog debug.bmp


IDIR=include

CC=g++-10

CFLAGS=-std=c++17 -I$(IDIR) -I/usr/local/Cellar/glew/2.1.0/include -I/usr/local/Cellar/glfw/3.3/include -L/usr/local/lib -g -O3 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -lglfw -lglew -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon

ODIR=build
LDIR=

SDIR=src

LIBS=

MAINFILE=main

_DEPS = matrix.hpp vector.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(MAINFILE).o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

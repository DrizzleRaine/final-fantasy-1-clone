CC = g++
CFLAGS = -Wall -std=c++0x
LIBS = -lGL -lGLU -lSDL -lSDL_image -lSDL_ttf
CMD = $(CC) $(CFLAGS) $(LIBS) $(INCLUDE)

ODIR = ./obj/
SOURCES = $(shell ls *.cpp)
OBJECTS = $(SOURCES:%.cpp=$(ODIR)%.o)

One: $(OBJECTS)
	$(CMD) $^ -o $@

$(ODIR)%.o: %.cpp
	$(CMD) -c $< -o $@

clean:
	rm $(OBJECTS) One

new:
	make clean
	make One

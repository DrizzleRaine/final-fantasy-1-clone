CC = g++
CFLAGS = -Wall -std=c++0x
LIBS = -lGL -lGLU -lSDL -lSDL_image -lSDL_ttf
CMD = $(CC) $(CFLAGS) $(LIBS) $(INCLUDE)

ODIR = ./obj/
SOURCES = main.cpp game.cpp window.cpp input.cpp textures.cpp font.cpp party.cpp character.cpp statemanager.cpp state.cpp titlestate.cpp menustate.cpp gui.cpp menu.cpp dialog.cpp cursor.cpp newmenu.cpp jobmenu.cpp namemenu.cpp partymenu.cpp itemmenu.cpp items.cpp magicmenu.cpp spells.cpp statusmenu.cpp mapstate.cpp map.cpp world.cpp town.cpp npc.cpp
OBJECTS = $(SOURCES:%.cpp=$(ODIR)%.o)

One: $(OBJECTS)
	$(CMD) $^ -o $@

$(ODIR)%.o: %.cpp
	$(CMD) -c $< -o $@

clean:
	rm $(OBJECTS) One

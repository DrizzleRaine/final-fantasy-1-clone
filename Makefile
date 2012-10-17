CC = g++
CFLAGS = -Wall
LIBS = -lSDL -lGL -lGLU -lSDL_image -lSDL_ttf
CMD = $(CC) $(CFLAGS) $(LIBS) $(INCLUDE)

SOURCES = main.cpp window.cpp input.cpp game.cpp title.cpp title_scene.cpp menu.cpp submenu.cpp menu_new.cpp menu_job.cpp menu_name.cpp map.cpp npc.cpp dialog.cpp textures.cpp font.cpp
OBJECTS = $(SOURCES:%.cpp=./obj/%.o)

Six: $(OBJECTS)
		$(CMD) $(OBJECTS) -o One

./obj/main.o: main.cpp
		$(CMD) -c main.cpp -o ./obj/main.o

./obj/window.o: window.cpp
		$(CMD) -c window.cpp -o ./obj/window.o

./obj/input.o: input.cpp
		$(CMD) -c input.cpp -o ./obj/input.o

./obj/game.o: game.cpp
		$(CMD) -c game.cpp -o ./obj/game.o

./obj/title.o: title.cpp
		$(CMD) -c title.cpp -o ./obj/title.o

./obj/title_scene.o: title_scene.cpp
		$(CMD) -c title_scene.cpp -o ./obj/title_scene.o

./obj/menu.o: menu.cpp
		$(CMD) -c menu.cpp -o ./obj/menu.o
	
./obj/submenu.o: submenu.cpp
		$(CMD) -c submenu.cpp -o ./obj/submenu.o

./obj/menu_new.o: menu_new.cpp
		$(CMD) -c menu_new.cpp -o ./obj/menu_new.o

./obj/menu_job.o: menu_job.cpp
		$(CMD) -c menu_job.cpp -o ./obj/menu_job.o

./obj/menu_name.o: menu_name.cpp
		$(CMD) -c menu_name.cpp -o ./obj/menu_name.o

./obj/map.o: map.cpp
		$(CMD) -c map.cpp -o ./obj/map.o

./obj/npc.o: npc.cpp
		$(CMD) -c npc.cpp -o ./obj/npc.o

./obj/dialog.o: dialog.cpp
		$(CMD) -c dialog.cpp -o ./obj/dialog.o

./obj/textures.o: textures.cpp
		$(CMD) -c textures.cpp -o ./obj/textures.o

./obj/font.o: font.cpp
		$(CMD) -c font.cpp -o ./obj/font.o

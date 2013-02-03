#ifndef MAP_H
#define MAP_H

#include <fstream>

#include "mapstate.h"
class MapState;

class Map {
	public:
		Map();
		virtual ~Map();

		// initialize shared resources
		void init(Party *p, Input *in, MapState *ms);

		// linked list node operations
		Map *getNext() { return next; }
		Map *getPrev() { return prev; }
		void setNext(Map *map) { next = map; }
		void setPrev(Map *map) { prev = map; }

		// handle map operations
		void updateMap();
		void renderMap(int width, int height);

		// pause/unpause map
		void pauseMap();
		void unpauseMap();
	protected:
		// each map has its own input
		Input *input;

		// every map inherits access to the party
		Party *party;

		// texture ids and count
		enum TexInfo {MAP, COUNT};
		Textures textures;

		// maps can push/pop/change map stack
		MapState *mapState;

		// current width and height of window
		int windowWidth, windowHeight;	

		// parties current position on this map
		int currentX, currentY;

		// id of the map
		int mapID;

		// name of the map
		std::string mapName;

		// array of info about map tiles
		int **tiles;
		
		// map dimensions, size of each tile
		int mapWidth, mapHeight, tileSize;

		// each map needs to check for blocked tiles
		bool blockedTile(int x, int y);
	private:
		// any necessary unique map initialization
		virtual void init()=0;

		// menu operations
		virtual void update() = 0;
		virtual void render() = 0;

		virtual void pause() = 0;
		virtual void unpause() = 0;

		// next and prev maps
		Map *next, *prev;
};

#endif

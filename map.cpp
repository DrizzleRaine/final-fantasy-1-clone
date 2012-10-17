#include "map.h"

Map::Map(Input *I, Character *C, PlayersParty *P) {
	const char* TextureArray[] = {"img/map_char.tga"};
	CharTexture = new Textures(1, TextureArray);
	Dialog = new DialogManager;

	MInput = I;
	MInput->SetRepeatDelay(1); // remove repeat delay for walking
	Characters = C;
	Party = P;
	SpriteX = 1;
	SpriteY = 1; // TODO: changes based on job of first char, can i just use Characters[0]->Job? maybe rearrange sprite sheet to make it work?

	NPCs = 0;
	NPCCount = 0;
	MapTexture = 0;
	Tiles = 0;

	LoadMap();

	Walking = 0; Reverse = 0;
	WalkDelay = 250.0; // const
	Party->XMovement = 0.0; Party->YMovement = 0.0;
}

Map::~Map() {
	for (int i = 0; i < XTiles; i++) {
		delete[] Tiles[i];
		Tiles[i] = 0;
	}
	delete[] Tiles;
	Tiles = 0;

	delete Dialog;
	Dialog = 0;

	if (NPCCount) {
		delete[] NPCs;
		NPCs = 0;
	}
	delete MapTexture;
	MapTexture = 0;

	delete CharTexture;
	CharTexture = 0;
}

void Map::LoadMap() {
	if (MapTexture) {
		delete MapTexture;
		MapTexture = 0;
	}
	if (Party->Map == 1) {
		const char* TextureArray[] = {"map_data/world.tga"};
		MapTexture = new Textures(1, TextureArray);
		LoadTiles("map_data/WORLD.MAP");
		if (NPCCount) {
			delete[] NPCs;
			NPCs = 0;
			NPCCount = 0;
		}
	} else if (Party->Map == 2) {
		const char* TextureArray[] = {"map_data/cornelia.tga"};
		MapTexture = new Textures(1, TextureArray);
		LoadTiles("map_data/CORNELIA.MAP");
		NPCCount = 8;
		NPCs = new NPC[NPCCount];
		for (int i = 0; i < NPCCount; i++) {
			NPCs[i].Initialize(i + 1, Party, Dialog, Tiles, TileSize);
			NPCs[i].UpdateCoordinates(WindowWidth, WindowHeight);
		}
		Dialog->TimedDialog("Cornelia", 2000);
	} else {
		printf("ERROR: Could not load map %i\n", Party->Map);
		return;
	}
}

void Map::LoadTiles(const char *FileName) {
	if (Tiles) {
		for (int i = 0; i < XTiles; i++) {
			delete[] Tiles[i];
			Tiles[i] = 0;
		}
		delete[] Tiles;
		Tiles = 0;
	}
	
	std::ifstream In;
	In.open(FileName);
	In >> MapWidth; In >> MapHeight;
	In >> Party->XPos; In >> Party->YPos; In >> SpriteX;
	In >> TileSize; In >> XTiles; In >> YTiles;

	Tiles = new int *[XTiles];
	for (int i = 0; i < XTiles; i++) {
		Tiles[i] = new int[YTiles];	
	}
	for (int y = 0; y < YTiles; y++) {
		for (int x = 0; x < XTiles; x++) {
			In >> Tiles[x][y];
		}
	}
	In.close();
}

int Map::Process() {
	// TODO: instead of setting Direction and Walking call Party->Move(Direction)
	if (MInput->UpPressed() && !Walking) {
		Direction = North;
		if (!BlockedTile(Party->XPos / TileSize, Party->YPos / TileSize - 1)) {
			Walking = SDL_GetTicks();
		}
	}
	if (MInput->DownPressed() && !Walking) {
		Direction = South;
		if (!BlockedTile(Party->XPos / TileSize, Party->YPos / TileSize + 1)) {
			Walking = SDL_GetTicks();
		}
	}
	if (MInput->RightPressed() && !Walking) {
		Direction = East;
		if (!BlockedTile(Party->XPos / TileSize + 1, Party->YPos / TileSize)) {
			Walking = SDL_GetTicks();
		}
	}
	if (MInput->LeftPressed() && !Walking) { 
		Direction = West;
		if (!BlockedTile(Party->XPos / TileSize - 1, Party->YPos / TileSize)) {
			Walking = SDL_GetTicks();
		}
	}

	if (MInput->Keys[68] && NPCCount) {
		MInput->Keys[68] = 0;
		MInput->SetRepeatDelay(); // set repeat delay for npc action to default
		int PartyXTile = Party->XPos / TileSize;
		int PartyYTile = Party->YPos / TileSize;
		if (Direction == North) {
			NPCAction(PartyXTile, PartyYTile - 1);
		} else if (Direction == East) {
			NPCAction(PartyXTile + 1, PartyYTile);
		} else if (Direction == South) {
			NPCAction(PartyXTile, PartyYTile + 1);
		} else if (Direction == West) {
			NPCAction(PartyXTile - 1, PartyYTile);
		}
		MInput->SetRepeatDelay(1); // remove repeat delay for walking
	}
	return 1;
}

bool Map::BlockedTile(int XPos, int YPos) {
	if (!Tiles[XPos][YPos]) {
		return 1;
	}
	for (int i = 0; i < NPCCount; i++) {
		if (NPCs[i].Exists(XPos, YPos)) {
			return 1;
		}
	}
	return 0;
}

void Map::NPCAction(int XPos, int YPos) {
	for (int i = 0; i < NPCCount; i++) {
		if (NPCs[i].Exists(XPos, YPos)) {
			NPCs[i].Action(static_cast<int>(Direction));
			return;
		}
	}
}

bool Map::ProcessMovement() {
	Reverse = 0;
	int CurrentDelay = SDL_GetTicks() - Walking;
	if (Direction == North) {
		SpriteX = 5;
		if (CurrentDelay < WalkDelay && Walking) {
			if (CurrentDelay < WalkDelay / 2) {
				SpriteX++;
			}
			Party->YMovement = -(CurrentDelay / WalkDelay) * TileSize;
		} else if (Walking) { 
			Walking = 0;
			Party->YMovement = 0.0;
			Party->YPos -= TileSize;
		}
	} else if (Direction == East) { // reverse west sprite
		SpriteX = 3;
		if (CurrentDelay < WalkDelay && Walking) {
			if (CurrentDelay < WalkDelay / 2) {
				SpriteX++;
			}
			Party->XMovement = (CurrentDelay / WalkDelay) * TileSize;
		} else if (Walking) {
			Walking = 0;
			Party->XMovement = 0.0;
			Party->XPos += TileSize;
		}
		Reverse = 1;
	} else if (Direction == South) {
		SpriteX = 1;
		if (CurrentDelay < WalkDelay && Walking) {
			if (CurrentDelay < WalkDelay / 2) {
				SpriteX++;
			}
			Party->YMovement = (CurrentDelay / WalkDelay) * TileSize;
		} else if (Walking) {
			Walking = 0;
			Party->YMovement = 0.0;
			Party->YPos += TileSize;
		}
	} else if (Direction == West) {
		SpriteX = 3;
		if (CurrentDelay < WalkDelay && Walking) {
			if (CurrentDelay < WalkDelay / 2) {
				SpriteX++;
			}
			Party->XMovement = -(CurrentDelay / WalkDelay) * TileSize;
		} else if (Walking) {
			Walking = 0;
			Party->XMovement = 0.0;
			Party->XPos -= TileSize;
		}
	}
	if (!Walking && Tiles[Party->XPos / TileSize][Party->YPos / TileSize] > 0) {
		Party->Map = Tiles[Party->XPos / TileSize][Party->YPos / TileSize];
		return 0;
	}
	return 1;
}

void Map::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	DrawMap();
	for (int i = 0; i < NPCCount; i++) {
		NPCs[i].Update();
		NPCs[i].Render();
	}
	Dialog->Render();
	DrawCharacter();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	SDL_GL_SwapBuffers();
}

void Map::DrawMap() {
	// player sees 15x10 tile
	GLfloat TexCoords[4];
	TexCoords[2] = (Party->XPos + (TileSize * 7.5) + Party->XMovement) / MapWidth;
	TexCoords[3] = (Party->YPos + (TileSize * 5.0) + Party->YMovement) / MapHeight;
	TexCoords[0] = (Party->XPos - (TileSize * 7.5) + Party->XMovement) / MapWidth;
	TexCoords[1] = (Party->YPos - (TileSize * 5.0) + Party->YMovement) / MapHeight;
	glBindTexture(GL_TEXTURE_2D, MapTexture->GLTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(TexCoords[0], TexCoords[1]);
		glVertex2f(-WindowWidth, WindowHeight);
		glTexCoord2f(TexCoords[0], TexCoords[3]);
		glVertex2f(-WindowWidth, -WindowHeight);
		glTexCoord2f(TexCoords[2], TexCoords[3]);
		glVertex2f(WindowWidth, -WindowHeight);
		glTexCoord2f(TexCoords[2], TexCoords[1]);
		glVertex2f(WindowWidth, WindowHeight);
	glEnd();
}

void Map::DrawCharacter() {
	float SheetWidth = 2860.0;
	float SheetHeight = 1560.0;
	GLfloat TexCoords[4];
	TexCoords[2] = ((SpriteX * 130.0) / SheetWidth);
	TexCoords[3] = ((SpriteY * 130.0) / SheetHeight);
	TexCoords[0] = (((SpriteX - 1) * 130.0) / SheetWidth);
	TexCoords[1] = (((SpriteY - 1) * 130.0) / SheetHeight);
	if (Reverse) {
		GLfloat Temp;
		Temp = TexCoords[0];
		TexCoords[0] = TexCoords[2];
		TexCoords[2] = Temp;
	}
	if (Party->Map == 1) { // on world map, player is a little higher
		glTranslatef(0.0f, 20.0f, 0.0f); // move up a bit for some reason
	}
	glBindTexture(GL_TEXTURE_2D, CharTexture->GLTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(TexCoords[0], TexCoords[1]);
		glVertex2f(-65, 65); // 130 width just looks right
		glTexCoord2f(TexCoords[0], TexCoords[3]);
		glVertex2f(-65, -65);
		glTexCoord2f(TexCoords[2], TexCoords[3]);
		glVertex2f(65, -65);
		glTexCoord2f(TexCoords[2], TexCoords[1]);
		glVertex2f(65, 65);
	glEnd();
}

void Map::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
	for (int i = 0; i < NPCCount; i++) {
		NPCs[i].UpdateCoordinates(NewWidth, NewHeight);
	}
	Dialog->UpdateCoordinates(NewWidth, NewHeight);
}

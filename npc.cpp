#include "npc.h"

NPC::NPC() {
}

NPC::~NPC() {
	delete NPCTexture;
	NPCTexture = 0;

	delete[] Text;
	Text = 0;
}

void NPC::Initialize(int ID, PlayersParty *P, DialogManager *D, int **TileData, int TSize) {
	char ImageFile[256];
	int TempDirection;
	Text = new char[256];
	std::ifstream In;
	if (ID == 1) { // this needs to be handled better
		In.open("npc_data/CORNELIA_WELCOME.NPC");
	} else if (ID == 2) {
		In.open("npc_data/RESCUE_SARAH.NPC");
	} else if (ID == 3) {
		In.open("npc_data/CORNELIA_DANCER.NPC");
	} else if (ID == 4) {
		In.open("npc_data/GUARD_PROPHECY.NPC");
	} else if (ID == 5) {
		In.open("npc_data/GUARD_RESTORE.NPC");
	} else if (ID == 6) {
		In.open("npc_data/CORNELIA_OLDWOMAN.NPC");
	} else if (ID == 7) {
		In.open("npc_data/CORNELIA_PRAVOKAMAN.NPC");
	} else if (ID == 8) {
		In.open("npc_data/CORNELIA_OLDMAN.NPC");
	}
	In.getline(ImageFile, 256);
	In >> Moves; In >> AnimationDelay; In >> MoveDelay;
	In >> CurrentX; In >> CurrentY; In >> TempDirection;
	In >> SheetWidth; In >> SheetHeight;
	In.ignore(256, '\n'); // go to the next line
	In.getline(Text, 256);
	In.close();

	const char *TextureArray[] = {ImageFile};
	NPCTexture = new Textures(1, TextureArray);

	Party = P;
	Dialog = D;
	Tiles = TileData;
	TileSize = TSize;

	Moving = SDL_GetTicks();
	Animation = SDL_GetTicks();
	Reverse = 0; SpriteY = 1;
	if (TempDirection == 0) {
		Direction = North;
		SpriteX = 5;
	} else if (TempDirection == 1) {
		Direction = East;
		SpriteX = 3;
		Reverse = 1;
	} else if (TempDirection == 2) {
		Direction = South;
		SpriteX = 1;
	} else {
		Direction = West;
		SpriteX = 3;
	}
	NewDirectionChosen = 0;
	XMovement = 0.0; YMovement = 0.0;
}

void NPC::Render() {
	float XDistance = CurrentX + XMovement - Party->XMovement - Party->XPos;
	float YDistance = CurrentY + YMovement - Party->YMovement - Party->YPos;
	float WindowXPos = XDistance / TileSize * WindowTileWidth;
	float WindowYPos = YDistance / TileSize * WindowTileHeight;

	GLfloat TexCoords[4];
	TexCoords[2] = ((SpriteX * 100.0) / SheetWidth);
	TexCoords[3] = ((SpriteY * 100.0) / SheetHeight);
	TexCoords[0] = (((SpriteX - 1) * 100.0) / SheetWidth);
	TexCoords[1] = (((SpriteY - 1) * 100.0) / SheetHeight);
	if (Reverse) {
		GLfloat Temp;
		Temp = TexCoords[0];
		TexCoords[0] = TexCoords[2];
		TexCoords[2] = Temp;
	}
	glTranslatef(WindowXPos, -WindowYPos, 0.0f);
	glBindTexture(GL_TEXTURE_2D, NPCTexture->GLTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(TexCoords[0], TexCoords[1]);
		glVertex2f(-50, 50);
		glTexCoord2f(TexCoords[0], TexCoords[3]);
		glVertex2f(-50, -50);
		glTexCoord2f(TexCoords[2], TexCoords[3]);
		glVertex2f(50, -50);
		glTexCoord2f(TexCoords[2], TexCoords[1]);
		glVertex2f(50, 50);
	glEnd();
	glLoadIdentity();

	Dialog->Render();
}

void NPC::Update() {
	if (!Moving) {
		return;
	}
	Reverse = 0;
	unsigned int CurrentADelay = SDL_GetTicks() - Animation;
	if (Direction == North) {
		SpriteX = 5;
		if (CurrentADelay < AnimationDelay / 2) {
			SpriteX++;
		} else if (CurrentADelay >= AnimationDelay) {
			Animation = SDL_GetTicks();
		}
	} else if (Direction == East) {
		SpriteX = 3;
		if (CurrentADelay < AnimationDelay / 2) {
			SpriteX++;
		} else if (CurrentADelay >= AnimationDelay) {
			Animation = SDL_GetTicks();
		}
		Reverse = 1;
	} else if (Direction == South) {
		SpriteX = 1;
		if (CurrentADelay < AnimationDelay / 2) {
			SpriteX++;
		} else if (CurrentADelay >= AnimationDelay) {
			Animation = SDL_GetTicks();
		}
	} else if (Direction == West) {
		SpriteX = 3;
		if (CurrentADelay < AnimationDelay / 2) {
			SpriteX++;
		} else if (CurrentADelay >= AnimationDelay) {
			Animation = SDL_GetTicks();
		}
	}
	if (!Moves) {
		return;
	}
	unsigned int CurrentDelay = SDL_GetTicks() - Moving;
	if (!NewDirectionChosen && CurrentDelay > MoveDelay) {
		Direction = static_cast<Directions>(rand() % 4);
		int XTilePos = CurrentX / TileSize;
		int YTilePos = CurrentY / TileSize;
		if (Direction == North) {
			Blocked = PositionBlocked(XTilePos, YTilePos - 1);
		} else if (Direction == East) {
			Blocked = PositionBlocked(XTilePos + 1, YTilePos);
		} else if (Direction == South) {
			Blocked = PositionBlocked(XTilePos, YTilePos + 1);
		} else if (Direction == West) {
			Blocked = PositionBlocked(XTilePos - 1, YTilePos);
		}
		NewDirectionChosen = 1;
	} else if (NewDirectionChosen && CurrentDelay > MoveDelay * 2) {
		NewDirectionChosen = 0;
		if (Direction == North && !Blocked) {
			YMovement = 0.0;
			CurrentY -= TileSize;
		} else if (Direction == East && !Blocked) {
			XMovement = 0.0;
			CurrentX += TileSize;
		} else if (Direction == South && !Blocked) {
			YMovement = 0.0;
			CurrentY += TileSize;
		} else if (Direction == West && !Blocked) {
			XMovement = 0.0;
			CurrentX -= TileSize;
		}
		Blocked = 0;
		Moving = SDL_GetTicks();
	} else if (NewDirectionChosen && CurrentDelay > MoveDelay && !Blocked) {
		float MovementPercent = (CurrentDelay - MoveDelay) / static_cast<float>(MoveDelay);
		if (Direction == North) {
			YMovement = -MovementPercent * TileSize;
		} else if (Direction == East) {
			XMovement = MovementPercent * TileSize;
		} else if (Direction == South) {
			YMovement = MovementPercent * TileSize;
		} else {
			XMovement = -MovementPercent * TileSize;
		}
	}
	return;
}

bool NPC::PositionBlocked(int PosX, int PosY) {
	int PartyXTile = Party->XPos / TileSize;
	int PartyYTile = Party->YPos / TileSize;
	if (!Tiles[PosX][PosY] || (PartyXTile == PosX && PartyYTile == PosY)) {
		return 1;
	}
	return 0;
}

void NPC::Action(int CDir) {
	if (Moving && NewDirectionChosen && (SDL_GetTicks() - Moving) > MoveDelay && !Blocked) {
		return; // cant talk to npc until they are done moving
	}
	if (Dialog->Exists()) {
		Dialog->DeleteDialog();
		if (!Dialog->Exists()) {
			Moving = SDL_GetTicks(); // begin moving again
			Animation = SDL_GetTicks();
		}
		return;
	}
	Moving = 0; // stop npc from moving
	Reverse = 0;
	if (CDir == North) { // character is facing north
		SpriteX = 1; // npc should face south
	} else if (CDir == East) {
		SpriteX = 3; // npc should face west, etc..
	} else if (CDir == South) {
		SpriteX = 5;
	} else if (CDir == West) {
		SpriteX = 3;
		Reverse = 1;
	}
	Dialog->CreateDialog(Text);
}

bool NPC::Exists(int X, int Y) {
	int CX = CurrentX / TileSize;
	int CY = CurrentY / TileSize;
	if (CX == X && CY == Y) {
		return 1; // npc exists at X,Y
	}
	if (!Moving || !NewDirectionChosen || Blocked) {
		return 0; // not moving, just standing
	} // else block tile they are moving into also
	int TempX = CX, TempY = CY;
	if (Direction == North) {
		TempY--;
	} else if (Direction == East) {
		TempX++;
	} else if (Direction == South) {
		TempY++;
	} else {
		TempX--;
	}
	if (TempX == X && TempY == Y) {
		return 1; // npc is moving to X,Y
	}
	return 0;
}

void NPC::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
	WindowTileWidth = NewWidth / 7.5;
	WindowTileHeight = NewHeight / 5.0;
	Dialog->UpdateCoordinates(NewWidth, NewHeight);
}

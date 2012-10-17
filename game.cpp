#include "game.h"

Game::Game() {
	GameInput = new Input;
	GameState = TitleS;
	TitleState = new Title(GameInput);

	for (int i = 0; i < 4; i++) {
		Characters[i].Job = i;
	}

	Party = new PlayersParty;
	Party->Map = 1;
}

Game::~Game() {
	if (GameState == TitleS) {
		delete TitleState;
		TitleState = 0;
	} else if (GameState == MenuS) {
		delete MenuState;
		MenuState = 0;
	} else if (GameState == MapS) {
		delete MapState;
		MapState = 0;
	}
	delete Party;
	Party = 0;

	delete GameInput;
	GameInput = 0;
}

void Game::Update() {
	if (GameState == TitleS) {
		TitleState->Update();
	} else if (GameState == MenuS) {
		MenuState->Update();
	} else if (GameState == MapS) {
		if (!MapState->ProcessMovement()) {
			MapState->LoadMap();
		}
		MapState->RenderScene();
	}
}

void Game::Process(SDL_Event &Event) {
	GameInput->PollInput(Event);
	
	if (GameState == TitleS) {
		if (!TitleState->Process()) {
			delete TitleState;
			TitleState = 0;
			
			MenuState = new Menu(GameInput, Characters);
			MenuState->UpdateCoordinates(WindowWidth, WindowHeight);
			GameState = MenuS;
		}
	} else if (GameState == MenuS) {
		int Status = MenuState->Process();
		if (!Status) { // exit back to title screen
			delete MenuState;
			MenuState = 0;

			TitleState = new Title(GameInput);
			TitleState->UpdateCoordinates(WindowWidth, WindowHeight);
			GameState = TitleS;
		} else if (Status == 2) { // party selected, begin game
			delete MenuState;
			MenuState = 0;

			MapState = new Map(GameInput, Characters, Party);
			MapState->UpdateCoordinates(WindowWidth, WindowHeight);
			GameState = MapS;
		}
	} else if (GameState == MapS) {
		if (!MapState->Process()) {
			delete MapState;
			MapState = 0;
			// either go to battle or menu state
		}
	}

	// reset input
	if (GameInput->EnterPressed()) {
		GameInput->ResetEnter();
	}

	for (int i = 0; i < 96; i++) {
		GameInput->Keys[i] = 0;
	}

	if (GameInput->BackspacePressed()) {
		GameInput->ResetBackspace();
	}
}

void Game::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
	if (GameState == TitleS) {
		TitleState->UpdateCoordinates(NewWidth, NewHeight);
	} else if (GameState == MenuS) {
		MenuState->UpdateCoordinates(NewWidth, NewHeight);
	} else if (GameState == MapS) {
		MapState->UpdateCoordinates(NewWidth, NewHeight);
	}
	GameInput->UpdateCoordinates(NewWidth, NewHeight);
}

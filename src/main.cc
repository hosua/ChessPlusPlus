#include "chess.hh"
#include "input.hh"
#include "global.hh"
#include "graphics.hh"

void gameLoop(GFX* gfx, Board* board, Mouse* mouse);

void handleInputs(GFX* gfx, Mouse* mouse, Board* board, SDL_Event event, std::vector<Coord>& valid_moves);

int main(){
	if (SDL_Init(SDL_INIT_EVERYTHING)){ // Use this when running the game normally
	//if (SDL_Init(SDL_INIT_NOPARACHUTE)){ // Use this when debugging with gdb
		fprintf(stderr, "Fatal error: Failed to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	GFX* gfx = new GFX();
	Board* board = new Board();
	Mouse* mouse = new Mouse();

	gameLoop(gfx, board, mouse);
}

void handleInputs(GFX* gfx, Mouse* mouse, Board* board, SDL_Event event, std::vector<Coord>& valid_moves){
	switch(event.type){
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					gfx->cleanQuit();
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			mouse->updatePos();
			mouse->setCursorGhost(gfx, event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			mouse->selectPiece(*board, valid_moves); 
			// board->printValidMoves(valid_moves);
			mouse->movePiece(board, valid_moves);
			break;
		}
		default:
			break;
	}
}

void gameLoop(GFX* gfx, Board* board, Mouse* mouse){

	gfx->clearScreen();
	bool is_running = true;	

	std::vector<Coord> valid_moves;

	Coord prev; // keep track of previous coord to prevent frequent printing
	while(is_running){
		SDL_Event event;

		while (SDL_PollEvent(&event)){
			handleInputs(gfx, mouse, board, event, valid_moves);
			// mouse->printPos();
			Coord pos = mouse->getPos();
			if (pos != prev){
				mouse->describePiece(*board);
				prev = pos;
			}
		}
		mouse->setHighlight(gfx);

		gfx->setPieceSelection(mouse->checkIfInPieceSelection());

		gfx->clearScreen();
		gfx->renderBoard();
		gfx->renderPieces(*board);
		gfx->renderCursorGhost();
		gfx->renderHighlight();
		gfx->renderPresent();
	}
}

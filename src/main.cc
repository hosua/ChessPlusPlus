#include "chess.hh"
#include "input.hh"
#include "global.hh"
#include "graphics.hh"

void gameLoop(GFX* gfx, Board* board, Mouse* mouse){

	gfx->clearScreen();
	bool is_running = true;	

	Coord prev; // keep track of previous coord to prevent frequent printing
	while(is_running){
		SDL_Event event;
		
		while (SDL_PollEvent(&event)){
			handleInputs(gfx, mouse, board, event);
			// mouse->printPos();
			Coord pos = mouse->getPos();
			if (pos != prev){
				mouse->describePiece(*board);
				prev = pos;
			}
		}

		gfx->clearScreen();
		gfx->renderBoard();
		gfx->renderPieces(*board);
		gfx->renderPresent();
	}
}

int main(){
	if (SDL_Init(SDL_INIT_EVERYTHING)){
		fprintf(stderr, "Fatal error: Failed to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_SUCCESS);
	}

	GFX* gfx = new GFX();
	Board* board = new Board();
	Mouse* mouse = new Mouse();

	gameLoop(gfx, board, mouse);
}

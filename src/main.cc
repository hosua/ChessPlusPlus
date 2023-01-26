#include "chess.hh"
#include "input.hh"
#include "global.hh"
#include "graphics.hh"

void gameLoop(GFX* gfx, Board* board, Mouse* mouse);

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

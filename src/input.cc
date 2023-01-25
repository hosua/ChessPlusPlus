#include "input.hh"
#include "chess.hh"

void handleInputs(GFX* gfx, Mouse* mouse, Board* board, SDL_Event event){
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
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse->selectPiece(*board);
			mouse->movePiece(board);
			break;
		default:
			break;
	}
}

void Mouse::movePiece(Board* board){
	Coord c_src = selected_coord;
	Coord c_dest = getPos();
	if (in_piece_selection and c_src != c_dest){
		cout << "Moved piece from " << c_src.getChessCoordStr() << " -> " << c_dest.getChessCoordStr() << "\n";
		board->movePiece(c_src, c_dest);
		// Piece* temp = board->grid[c_src.y][c_src.x];
		// board->grid[c_src.y][c_src.x] = board->grid[c_dest.y][c_dest.x];
		// board->grid[c_dest.y][c_dest.x] = temp;
		in_piece_selection = false;
		selected_coord = Coord(-1,-1);
	}
}

void Mouse::describePiece(Board board){
	Coord c = getPos();
	Piece* pc = board.grid[c.y][c.x];
	if (pc)
		cout << pc << " " << c.getChessCoordStr() << "\n";
	else
		cout << c.getChessCoordStr() << "\n";
}

void Mouse::selectPiece(Board board){
	Coord c = getPos();	
	Piece* pc = board.grid[c.y][c.x];
	if (not in_piece_selection){ // select
		if (pc){
			cout << "selected piece: " << *pc << "\n";
			selected_coord = Coord(c.x, c.y);
			in_piece_selection = true;
		}
	} else { // deselect piece if same coordinate is clicked
		if ( getPos() == selected_coord){
			cout << "unselected piece: " << pc << "\n";
			selected_coord = Coord(-1,-1);
			in_piece_selection = false;
		}
	}
}

void Mouse::updatePos(){
	SDL_GetMouseState(&true_pos.x, &true_pos.y);
	pos.x = (true_pos.x / GRID_CELL_SIZE);
	pos.y = (true_pos.y / GRID_CELL_SIZE);
	if (pos.x >= GRID_WIDTH)
		pos.x--;
	if (pos.y >= GRID_HEIGHT)
		pos.y--;
}

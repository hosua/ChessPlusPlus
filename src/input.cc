#include "input.hh"
#include "chess.hh"

void Mouse::movePiece(Board* board){
	if (selected_coord == empty_coord)
		return;

	Coord c_src = selected_coord;
	Coord c_dest = getPos();
	Piece* pc = board->grid[c_src.y][c_src.x];
	
	if (not pc)
		return;

	P_Color src_color = NULL_COLOR;
	src_color = pc->getColor(); 

	if (in_piece_selection and c_src != c_dest){
		cout << "Moved piece from " << c_src.getChessCoordStr() << " -> " << c_dest.getChessCoordStr() << "\n";
		board->movePiece(c_src, c_dest, src_color);

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

void Mouse::setHighlight(GFX* gfx){
	Coord c = selected_coord;
	if (in_piece_selection and c != empty_coord){
		gfx->highlight.x = (c.x * GRID_CELL_SIZE);
		gfx->highlight.y = (c.y * GRID_CELL_SIZE);
	}
}

void Mouse::setCursorGhost(GFX* gfx, SDL_Event event){
	gfx->cursor_ghost.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;	
	gfx->cursor_ghost.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
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

#include "input.hh"
#include "chess.hh"

void Mouse::movePiece(Board* board, std::set<Coord> valid_moves){
	if (selected_coord == empty_coord)
		return;

	Coord src = selected_coord;
	Coord dest = getPos();
	Piece* pc = board->grid[src.y][src.x];
	
	if (not pc)
		return;

	P_Color src_color = NULL_COLOR;
	src_color = pc->getColor(); 

	if (in_piece_selection and src != dest){
		// cout << "Moved piece from " << src.getChessCoordStr() << " -> " << dest.getChessCoordStr() << "\n";
		board->movePiece(src, dest, src_color, valid_moves);
		in_piece_selection = false;
		selected_coord = Coord(-1,-1);
	}
}

void Mouse::describePiece(Board board){
	Coord c = getPos();
	Piece* pc = board.grid[c.y][c.x];

	if (pc)
		cout << pc << " ";

	cout << c.getChessCoordStr() << "\n";
}

void Mouse::selectPiece(Board board, std::set<Coord>& valid_moves){
	Coord c = getPos();	
	Piece* pc = board.grid[c.y][c.x];
	if (not in_piece_selection){ // select
		if (pc){
			cout << "selected piece: " << *pc << " at " << pc->getPos() << " \n";
			selected_coord = Coord(c.x, c.y);
			pc->setPos(selected_coord);
			Coord src = getPos();
			valid_moves = pc->getValidMoves(src, &board);
			board.printValidMoves(valid_moves);
			in_piece_selection = true;
		}
	} else { // deselect piece if same coordinate is clicked
		if (getPos() == selected_coord){
			cout << "unselected piece: " << pc << "\n";
			selected_coord = Coord(-1,-1);
			in_piece_selection = false;
			valid_moves = {};
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

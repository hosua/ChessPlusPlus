#include "chess.hh"

// left right down up movement
std::vector<Coord> lrdu = { {-1,0}, {+1,0}, {0,+1}, {0,-1} };

// diagonal movement
std::vector<Coord> diag = { {-1,-1}, {+1, -1}, {-1, +1}, {+1, +1} };

// knight movement
std::vector<Coord> knight_moves = {
	{+1,-2}, {+1,+2}, {+2,-1}, {+2,+1},
	{-1,-2}, {-1,+2}, {-2,-1}, {-2,+1}
};
										
Coord empty_coord = Coord(-1,-1);

std::vector<P_Type> main_row = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};

std::map<P_Type, std::string> p_type_to_str = {
	{NULL_TYPE, "null"},
	{BISHOP, "bishop"},
	{KING, "king"},
	{KNIGHT, "knight"},
	{PAWN, "pawn"},
	{ROOK, "rook"},
	{QUEEN, "queen"}
};

std::map<P_Color, std::string> p_color_to_str = {
	{BLACK, "black"}, {WHITE, "white"}
};

std::map<int,int> coord_conversion_map = {
	{0,7},{1,6},{2,5},{3,4},
	{4,3},{5,2},{6,1},{7,0}
};

// print piece description
std::ostream& operator<<(std::ostream& out, Piece pc){
	out << p_color_to_str[pc.getColor()] << " " << p_type_to_str[pc.getType()];
	return out;
}

// print piece description
std::ostream& operator<<(std::ostream& out, Piece* pc){
	out << p_color_to_str[pc->getColor()] << " " << p_type_to_str[pc->getType()];
	return out;
}

// print Coord
std::ostream& operator<<(std::ostream& out, const Coord& c){
	out << "(" << c.x << "," << c.y << ")";
	return out;
}

void Board::movePiece(Coord src, Coord dest, P_Color src_color, std::set<Coord> valid_moves){
	Piece* src_pc = grid[src.y][src.x];
	Piece* dest_pc = grid[dest.y][dest.x];

	Piece** a = &grid[src.y][src.x];
	Piece** b = &grid[dest.y][dest.x];
	
	if (std::find(valid_moves.begin(), valid_moves.end(), dest) == valid_moves.end()){
		cout << src.getChessCoordStr() << " -> " << dest.getChessCoordStr() << " is not a valid move.\n";
		return;
	} 
	/* ----------- en passant code ------------- */	
	
	// Check if move results in en passant
	if (passant_tile == dest and checkIfDifferentColor(src,dest)){
		cout << "En passant.\n";
		// destroy pawn
		delete grid[passant_pawn_tile.y][passant_pawn_tile.x];		
		grid[passant_pawn_tile.y][passant_pawn_tile.x] = nullptr;		
	}
	
	if (passant_tile != empty_coord and passant_tile != dest){
		cout << "Unset passant tile\n";
		passant_pawn_tile = passant_tile = empty_coord;
	}

	// pawn double move check for en passant
	if (src_pc and src_pc->getType() == PAWN 
			and ((abs(dest.x - src_pc->getPos().x) > 1)
			or (abs(dest.y - src_pc->getPos().y) > 1))
			){

		cout << "pawn double move detected\n";

		if (src_pc->getColor() == BLACK){
			passant_tile = Coord(dest.x, dest.y-1);
		} else {
			passant_tile = Coord(dest.x, dest.y+1);
		}

		cout << "Set passant tile to " << passant_tile.getChessCoordStr() << "\n";

		passant_pawn_tile = dest;
		cout << "Set passant pawn tile to " << passant_pawn_tile.getChessCoordStr() << "\n";
	}
	/* ----------- end en passant code ------------- */	

	// update king positions
	if (src_pc->getType() == KING){
		// checkKings() needs to occur before king can move
		(src_pc->getColor() == BLACK) ? black_king_pos = dest : white_king_pos = dest;
		// if true, undo move
	}

	cout << "Moved piece from " << src.getChessCoordStr() << " -> " << dest.getChessCoordStr() << "\n";

	if (not dest_pc){ // swap with empty tile
		std::swap(*a, *b);
	} else { // is enemy piece, swap and delete killed piece
		std::swap(*a, *b);
		delete *a;
		grid[src.y][src.x] = nullptr;
	}

	// make move, check if any kings are under attack
	checkKings();
}

// TODO: Returns the coordinate of an unsafe king, returns the empty_coord if both are safe
Coord Board::checkKings(){
	for (int y = 0; y < GRID_HEIGHT; y++){
		for (int x = 0; x < GRID_WIDTH; x++){
			Coord src = Coord(x,y);
			Piece* pc = grid[y][x];
				if (pc){
				std::set<Coord> valid_moves = pc->getValidMoves(src, this);
				if (valid_moves.find(white_king_pos) != valid_moves.end()){
					cout << "White king check!\n";
					return white_king_pos;
				} 

				if (valid_moves.find(black_king_pos) != valid_moves.end()){
					cout << "Black king check!\n";
					return black_king_pos;
				} 
			}
		}	
	}
	return empty_coord;
}


bool Board::checkIfCoordInbounds(Coord c){
	return (c.x >= 0 and c.y >= 0 and c.x < GRID_WIDTH and c.y < GRID_HEIGHT);	
}

bool Board::checkIfDifferentColor(Coord src, Coord dest){
	Piece* src_pc = grid[src.y][src.x];
	Piece* dest_pc = grid[dest.y][dest.x];
	if (not src_pc or not dest_pc)
		return true;
	return (src_pc->getColor() != dest_pc->getColor());
}

void Board::printValidMoves(std::set<Coord> moves){
	cout << "Valid moves: [";
	// For debugging
	if (moves.size()){
		auto itr = moves.begin();
		auto end_itr = --moves.end();
		while (itr != end_itr){
			cout << itr->getChessCoordStr() << ", ";
			itr++;
		}
		cout << itr->getChessCoordStr() << "]\n";
		// for (size_t i = 0; i < moves.size()-1; i++)
		// cout << (moves.end()-1)->getChessCoordStr();
	}
}
Board::Board() : passant_tile(empty_coord) { 
	for (int y = 0; y < GRID_HEIGHT; y++)
		for (int x = 0; x < GRID_WIDTH; x++)
			grid[y][x] = nullptr;
	reset(); 
}

void Board::reset(){
	cout << "Resetting the game!\n";
	for (int y = 0; y < GRID_HEIGHT; y++){
		for (int x = 0; x < GRID_WIDTH; x++){
			delete grid[y][x];
			grid[y][x] = nullptr;
		}
	}

	// Black Pieces;	
	for (int x = 0; x < GRID_WIDTH; x++){
		grid[0][x] = new Piece(main_row[x], BLACK, Coord(0,x));
		grid[1][x] = new Piece(PAWN, BLACK, Coord(1,0));
	}
	black_king_pos = Coord(4,0);
	// White Pieces;	
	for (int x = 0; x < GRID_WIDTH; x++){
		grid[6][x] = new Piece(PAWN, WHITE, Coord(6,x));
		grid[7][x] = new Piece(main_row[x], WHITE, Coord(7,x));
	}
	white_king_pos = Coord(4,7);
}

bool Board::isAttacking(Coord src, Coord dest){
	Piece* src_pc = grid[src.y][src.x];
	Piece* dest_pc = grid[src.y][src.x];
	if (not src_pc or not dest_pc)
		return false;
	// std::vector<Coord> valid_moves = getValidMoves(	
	return false;
}

// TODO: King checker needs to be implemented in this function somewhere
std::set<Coord> Piece::getValidMoves(Coord src, Board* board){
	std::set<Coord> valid_moves;
	
	// Helper function for rooks, bishops, and queens
	std::function<void(Coord,Coord)> moveHelper = [&](Coord dest, Coord move) -> void {
		if (not board->checkIfCoordInbounds(dest))
			return;
		if (board->grid[dest.y][dest.x]){
			if (board->checkIfDifferentColor(src, dest))
				valid_moves.insert(dest);
			return;
		}
		valid_moves.insert(dest);
		dest = Coord(dest.x + move.x, dest.y + move.y);
		moveHelper(dest, move);
	};

	Coord dest;

	switch(getType()){
		case BISHOP:
			for (Coord move : diag)
				moveHelper(src + move, move);
			break;
		case KING: 
			for (Coord move : lrdu){
				dest = Coord(src.x + move.x, src.y + move.y);
				if (board->checkIfCoordInbounds(dest) and board->checkIfDifferentColor(src, dest))
					valid_moves.insert(dest);
			}
			for (Coord move : diag){
				dest = Coord(src.x + move.x, src.y + move.y);
				if (board->checkIfCoordInbounds(dest) and board->checkIfDifferentColor(src, dest))
					valid_moves.insert(dest);

			}
			break;
		case KNIGHT:
			for (Coord move : knight_moves){
				dest = Coord(src.x + move.x, src.y + move.y);
				if (board->checkIfCoordInbounds(dest) and board->checkIfDifferentColor(src, dest))
					valid_moves.insert(dest);
			}
			break;
		case PAWN:
		{
			Coord p = board->getPassantPawnTile();
			if (getColor() == BLACK){
				dest = Coord(src.x, src.y+1);
				if (board->checkIfCoordInbounds(dest) and not board->grid[dest.y][dest.x]){
					valid_moves.insert(dest);
				 }

				// if first turn
				if (src.y == 1){
					// Do not allow pawns to jump over any pieces
					dest = Coord(src.x, src.y+2);
					if (not board->grid[dest.y][dest.x] and not board->grid[dest.y-1][dest.x])
						valid_moves.insert(dest);
				}

				// Diagonals (move only if enemy piece is present or passant tile is attackable
				dest = Coord(src.x-1, src.y+1); // bottom left
				if ((board->checkIfCoordInbounds(dest))
						and ((board->grid[dest.y][dest.x] and (board->checkIfDifferentColor(src, dest)))
						or ((board->getPassantTile() == dest) and board->checkIfDifferentColor(src, p)))){
					valid_moves.insert(dest);
				}

				dest = Coord(src.x+1, src.y+1); // bottom right
				// Do not allow pawns to jump over any pieces
				if (board->checkIfCoordInbounds(dest) 
						and board->grid[dest.y][dest.x]
						and board->checkIfDifferentColor(src, dest))
					valid_moves.insert(dest);
				
				if ((board->checkIfCoordInbounds(dest))
						and ((board->grid[dest.y][dest.x] and (board->checkIfDifferentColor(src, dest)))
						or ((board->getPassantTile() == dest) and board->checkIfDifferentColor(src, p)))){
					valid_moves.insert(dest);
				}

			} else if (getColor() == WHITE){
				// First move can move two tiles
				dest = Coord(src.x, src.y-1);
				if (board->checkIfCoordInbounds(dest) and not board->grid[dest.y][dest.x])
					valid_moves.insert(dest);

				dest = Coord(src.x,src.y-2);
				if (src.y == 6 and not board->grid[dest.y][dest.x] and not board->grid[dest.y+1][dest.x])	 
					valid_moves.insert(dest);

				// Diagonals (move only if enemy piece is present
				dest = Coord(src.x-1, src.y-1); // top left
				if ((board->checkIfCoordInbounds(dest))
						and ((board->grid[dest.y][dest.x] and (board->checkIfDifferentColor(src, dest)))
						or ((board->getPassantTile() == dest) and board->checkIfDifferentColor(src, p)))){
					valid_moves.insert(dest);
				}

				dest = Coord(src.x+1, src.y-1); // top right
				// Do not allow pawns to jump over any pieces
				if ((board->checkIfCoordInbounds(dest))
						and ((board->grid[dest.y][dest.x] and (board->checkIfDifferentColor(src, dest)))
						or ((board->getPassantTile() == dest) and board->checkIfDifferentColor(src, p)))){
					valid_moves.insert(dest);
				}
			}
			break;
		}
		case ROOK: 
			// TODO: Need to implement castles
			for (Coord move : lrdu)
				moveHelper(src + move, move);
			break;
		case QUEEN:
			for (Coord move : lrdu)
				moveHelper(src + move, move);
			for (Coord move : diag)
				moveHelper(src + move, move);
			break;
		default:
			break;
	}
	return valid_moves;
}

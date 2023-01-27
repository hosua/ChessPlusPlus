#include "chess.hh"

// TODO: Need to implement move validator to check moves as soon as piece is selected.
// Too tired rn.

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

Board::Board(){ 
	for (int y = 0; y < GRID_HEIGHT; y++)
		for (int x = 0; x < GRID_WIDTH; x++)
			grid[y][x] = nullptr;
	reset(); 
}

void Board::movePiece(Coord src, Coord dest, P_Color src_color, std::vector<Coord> valid_moves){
	Piece* dest_pc = grid[dest.y][dest.x];

	P_Color dest_color = NULL_COLOR;
	if (dest_pc) dest_color = dest_pc->getColor();

	Piece** a = &grid[src.y][src.x];
	Piece** b = &grid[dest.y][dest.x];
	
	// cout << "dest: " << dest.getChessCoordStr() << "\n";
	// printValidMoves(valid_moves);
	if (std::find(valid_moves.begin(), valid_moves.end(), dest) == valid_moves.end()){
		cout << src.getChessCoordStr() << " -> " << dest.getChessCoordStr() << " is not a valid move.\n";
		return;
	} 

	cout << "Moved piece from " << src.getChessCoordStr() << " -> " << dest.getChessCoordStr() << "\n";

	Coord c = Coord(src.x, src.y);

	if (not dest_pc){ // swap with empty tile
		std::swap(*a, *b);
	} else { // is enemy piece, swap and delete killed piece
		std::swap(*a, *b);
		delete *a;
		grid[src.y][src.x] = nullptr;
	}
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


// TODO 
bool Board::validateMove(Coord src, Coord dest, std::vector<Coord> valid_moves){
	// cout << "Warning: validateMove() is not yet implemented.\n";

	Piece* src_pc = grid[src.y][src.x];		
	Piece* dest_pc = grid[dest.y][dest.x];		
	
	cout << src << "\n";
	// If source piece is null or source piece is default empty coord
	if (not src_pc or src == empty_coord) 
		return false;
	// If src and dest color match
	if (not checkIfDifferentColor(src, dest))
		return false;
	
	// If the destination move is in the valid moves vector, then it is valid.
	return (std::find(valid_moves.begin(), valid_moves.end(), dest) != valid_moves.end());
}

void Board::printValidMoves(std::vector<Coord> moves){
	cout << "Valid moves: [";
	// For debugging
	if (moves.size()){
		for (int i = 0; i < moves.size()-1; i++)
			cout << moves[i].getChessCoordStr() << ", ";
		cout << (moves.end()-1)->getChessCoordStr();
	}
	cout << "]\n";
}

void Board::reset(){
	cout << "Board was reset.\n";
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
	// White Pieces;	
	for (int x = 0; x < GRID_WIDTH; x++){
		grid[6][x] = new Piece(PAWN, WHITE, Coord(6,x));
		grid[7][x] = new Piece(main_row[x], WHITE, Coord(7,x));
	}
}

std::vector<Coord> Piece::getValidMoves(Board* board){
	std::vector<Coord> valid_moves;
	Coord c = getPos();

	Coord dest;
	Piece* dest_pc = nullptr;

	switch(getType()){
		case BISHOP:
			break;
		case KING: 
			break;
		case KNIGHT:
			break;
		case PAWN:
			//  An example of how move validation will be implemented
			//  TODO: Add diagonals if there exists a diagonal enemy to the selected pawn
			//  Pawns can only attack an enemy if any only if they are diagonal from the pawn.
			if (getColor() == BLACK){
				// First move can move two tiles
				dest = Coord(c.x, c.y+1);
				if (board->checkIfCoordInbounds(dest) and not board->grid[dest.y][dest.x])
					valid_moves.push_back(dest);
				dest = Coord(c.x, c.y+2);
				// Do not allow pawns to jump over any pieces
				if (c.y == 1 and not board->grid[dest.y][dest.x] and not board->grid[dest.y-1][dest.x])	 
					valid_moves.push_back(Coord(c.x,c.y+2));

				// Diagonals (move only if enemy piece is present
				dest = Coord(c.x-1, c.y+1); // bottom left
				dest_pc = board->grid[dest.y][dest.x];
				if (board->checkIfCoordInbounds(dest) 
						and dest_pc and dest_pc->getColor() == WHITE)
					valid_moves.push_back(dest);

				dest = Coord(c.x+1, c.y+1); // bottom right
				dest_pc = board->grid[c.y+1][c.x+1]; 
				// Do not allow pawns to jump over any pieces
				if (board->checkIfCoordInbounds(dest) 
						and dest_pc and dest_pc->getColor() == WHITE)
					valid_moves.push_back(dest);

			} else if (getColor() == WHITE){
				// First move can move two tiles
				dest = Coord(c.x, c.y-1);
				if (board->checkIfCoordInbounds(dest) and not board->grid[dest.y][dest.x])
					valid_moves.push_back(dest);

				dest = Coord(c.x,c.y-2);
				if (c.y == 6 and not board->grid[dest.y][dest.x] and not board->grid[dest.y+1][dest.x])	 
					valid_moves.push_back(dest);

				// Diagonals (move only if enemy piece is present
				dest = Coord(c.x-1, c.y-1); // top left
				dest_pc = board->grid[dest.y][dest.x];
				if (board->checkIfCoordInbounds(dest) 
						and dest_pc and dest_pc->getColor() == BLACK)
					valid_moves.push_back(dest);

				dest = Coord(c.x+1, c.y-1); // top right
				dest_pc = board->grid[dest.y][dest.x]; 
				// Do not allow pawns to jump over any pieces
				if (board->checkIfCoordInbounds(dest) 
						and dest_pc and dest_pc->getColor() == BLACK)
					valid_moves.push_back(dest);
			}
		
			break;
		case ROOK: 
			break;
		case QUEEN:
			break;
		default:
			break;
	}
	return valid_moves;
}

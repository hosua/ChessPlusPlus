#include "chess.hh"

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

std::map<int,int> coord_conversion_map = {
	{0,7},{1,6},{2,5},{3,4},
	{4,3},{5,2},{6,1},{7,0}
};

// print piece description
std::ostream& operator<<(std::ostream& out, Piece pc){
	std::string color = (pc.getColor() == BLACK) ? "black" : "white";
	out << color << " " << p_type_to_str[pc.getType()];
	return out;
}

// print piece description
std::ostream& operator<<(std::ostream& out, Piece* pc){
	std::string color = (pc->getColor() == BLACK) ? "black" : "white";
	out << color << " " << p_type_to_str[pc->getType()];
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
	resetGame(); 
}

void Board::resetGame(){
	for (int y = 0; y < GRID_HEIGHT; y++)
		for (int x = 0; x < GRID_WIDTH; x++)
			delete grid[y][x];

	// Black Pieces;	
	for (int x = 0; x < GRID_WIDTH; x++){
		grid[0][x] = new Piece(main_row[x], BLACK);
		grid[1][x] = new Piece(PAWN, BLACK);
	}
	// White Pieces;	
	for (int x = 0; x < GRID_WIDTH; x++){
		grid[6][x] = new Piece(PAWN, WHITE);
		grid[7][x] = new Piece(main_row[x], WHITE);
	}
}

// ChessCoord coordToChessCoord(Coord c){
// 	char letter = c.x + 'A';
// 	int number = coord_conversion_map[c.y]+1;
// 	return ChessCoord({letter,number});
// }
// 
// Coord chessCoordToCoord(ChessCoord cc){
// 	int x, y;
// 	x = cc.x - 'A';
// 	y = coord_conversion_map[cc.y-1];
// 	return Coord({x,y});
// }

#ifndef CHESS_H
#define CHESS_H

#include "graphics.hh"
#include <vector>

class Piece;
class Board;

class Board {
public:
	Board();
	~Board(){}

	void resetGame();

	Piece* grid[GRID_HEIGHT][GRID_WIDTH];
};

class Piece {
public:
	Piece() : type(NULL_TYPE), color(NULL_COLOR){}
	Piece(P_Type type, P_Color color) : type(type), color(color){}
	~Piece(){}

	friend std::ostream& operator<<(std::ostream& out, Piece pc);
	friend std::ostream& operator<<(std::ostream& out, Piece* pc);

	P_Type getType(){ return type; }
	P_Color getColor(){ return color; }
private:
	P_Type type;
	P_Color color;	
};

// ChessCoord coordToChessCoord(Coord c);
// Coord chessCoordToCoord(ChessCoord cc);

#endif // CHESS_H

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

	void reset();


	bool checkIfCoordInbounds(Coord c);

	bool checkIfDifferentColor(Coord src, Coord dest);

	void movePiece(Coord src, Coord dest, P_Color src_color, std::vector<Coord> valid_moves);

	void printValidMoves(std::vector<Coord> moves);

	bool validateMove(Coord src, Coord dest, std::vector<Coord> valid_moves); // TODO

	Piece* grid[GRID_HEIGHT][GRID_WIDTH];
};

class Piece {
public:
	Piece() : type(NULL_TYPE), color(NULL_COLOR){}
	Piece(P_Type type, P_Color color) : type(type), color(color){}
	Piece(P_Type type, P_Color color, Coord c) : type(type), color(color), pos(c) {}

	~Piece(){}

	friend std::ostream& operator<<(std::ostream& out, Piece pc);
	friend std::ostream& operator<<(std::ostream& out, Piece* pc);

	P_Type getType(){ return type; }
	P_Color getColor(){ return color; }

	void setPos(Coord c){ pos = c; }
	Coord getPos(){ return pos; }

	std::vector<Coord> getValidMoves(Board* board);

private:
	Coord pos;
	P_Type type;
	P_Color color;	
};

extern Coord empty_coord;

// ChessCoord coordToChessCoord(Coord c);
// Coord chessCoordToCoord(ChessCoord cc);

#endif // CHESS_H

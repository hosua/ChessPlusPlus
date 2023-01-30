#ifndef CHESS_H
#define CHESS_H

#include "graphics.hh"
#include <vector>

extern Coord empty_coord; // An invalid coordinate that should be checked for before indexing the board

extern std::vector<Coord> lrdu; // left right down up movement
extern std::vector<Coord> diag; // diagonal movement
extern std::vector<Coord> knight_moves; // knight movement

class Piece;
class Board;

class Board {
public:
	Board();
	~Board(){}

	void reset();

	bool checkIfCoordInbounds(Coord c);
	bool checkIfDifferentColor(Coord src, Coord dest);

	std::vector<Coord> checkKings(P_Color king_color); // Return true if king is under attack in current game state
	bool checkIfMated(P_Color king_color, std::vector<Coord> attackers); // Return true if king is is checked and 
	
	bool isAttacking(Coord src, Coord dest);

	Coord getPassantTile(){ return passant_tile; }
	void setPassantTile(Coord c){ passant_tile = c; }
	void unsetPassantTile(Coord c){ passant_tile = empty_coord; }

	Coord getPassantPawnTile(){ return passant_pawn_tile; }
	void setPassantPawnTile(Coord c){ passant_pawn_tile = c; }
	void unsetPassantPawnTile(Coord c){ passant_pawn_tile = empty_coord; }

	void movePiece(Coord src, Coord dest, P_Color src_color, std::set<Coord> valid_moves);
	
	void printValidMoves(std::set<Coord> moves);

	Piece* grid[GRID_HEIGHT][GRID_WIDTH];

private:

	Coord white_king_pos, black_king_pos;

	Coord passant_tile; // tile that pawn has to attack diagonally if en passant
	Coord passant_pawn_tile; // coord of the pawn to till if en passant
};

class Piece {
public:
	Piece() : pos(empty_coord), type(NULL_TYPE), color(NULL_COLOR){}
	Piece(P_Type type, P_Color color) : pos(empty_coord), type(type), color(color){}
	Piece(P_Type type, P_Color color, Coord c) : pos(c), type(type), color(color) {}

	~Piece(){}

	friend std::ostream& operator<<(std::ostream& out, Piece pc);
	friend std::ostream& operator<<(std::ostream& out, Piece* pc);

	P_Type getType(){ return type; }
	P_Color getColor(){ return color; }

	void setPos(Coord c){ pos = c; }
	Coord getPos(){ return pos; }

	std::set<Coord> getValidMoves(Coord src, Board* board);

private:
	Coord pos;
	P_Type type;
	P_Color color;	
};

// ChessCoord coordToChessCoord(Coord c);
// Coord chessCoordToCoord(ChessCoord cc);

#endif // CHESS_H

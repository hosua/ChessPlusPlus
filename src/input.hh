#ifndef INPUT_H
#define INPUT_H

#include "global.hh"
#include "graphics.hh"

class Mouse {
public:
	Mouse() : pos({0,0}), true_pos({0,0}), 
		in_piece_selection(false), selected_coord({-1,-1}) {}

	~Mouse(){}

	void updatePos();

	void printPos(){
		Coord p = pos;
		printf("pos: (%i,%i)\n", p.x, p.y);
	}

	void printTruePos(){
		Coord p = true_pos;
		printf("true pos: (%i,%i)\n", p.x, p.y);
	}

	void selectPiece(Board board);

	void describePiece(Board board);
	
	void movePiece(Board* board); // move selected piece to cell at mouse pos

	Coord getPos(){ return pos; }
	Coord getTruePos(){ return true_pos; }
	
	bool checkIfInPieceSelection(){
		return in_piece_selection;
	}

private:
	Coord pos;
	Coord true_pos;

	bool in_piece_selection;
	Coord selected_coord; // the coordinate of the piece selected
};

void handleInputs(GFX* gfx, Mouse* mouse, Board* board, SDL_Event event);



#endif // INPUT_H

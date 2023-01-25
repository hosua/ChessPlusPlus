#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <iostream>
#include <map>

#define GRID_CELL_SIZE 80
#define PIECE_SIZE 70
#define GRID_WIDTH 8
#define GRID_HEIGHT 8
#define SCREEN_X (GRID_WIDTH * GRID_CELL_SIZE) + 1
#define SCREEN_Y (GRID_HEIGHT * GRID_CELL_SIZE) + 1

using std::cout;

class GFX;
class Board;
class Piece;

typedef enum {
	NULL_TYPE,
	BISHOP, KING, KNIGHT,
	PAWN, ROOK, QUEEN,
} P_Type;

typedef enum {
	NULL_COLOR,
	WHITE, BLACK
} P_Color;

extern std::map<int,int> coord_conversion_map;

struct Coord {
	int x, y;
	Coord(){}
	Coord(int x, int y) : x(x), y(y){}
	// print Coord
	friend std::ostream& operator<<(std::ostream& out, const Coord& c);

	bool operator==(Coord b){
		return (this->x == b.x and this->y == b.y);
	}
	bool operator!=(Coord b){
		return not (this->x == b.x and this->y == b.y);
	}

	std::string getChessCoordStr(){
		char letter = x + 'A';
		int number = coord_conversion_map[y]+1;
		return letter + std::to_string(number);
	}
};

// extern ChessCoord coordToChessChord(Coord coord);
// extern Coord chessCoordToCoord(ChessCoord chess_coord);

extern std::map<P_Type, std::string> p_type_to_str;

#endif // GLOBAL_H
#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <vector>
#include <iostream>
#include <map>
#include <functional> // for std::function

#define GRID_CELL_SIZE 80
#define PIECE_SIZE 60
#define GRID_WIDTH 8
#define GRID_HEIGHT 8
#define SCREEN_X (GRID_WIDTH * GRID_CELL_SIZE) + 1
#define SCREEN_Y (GRID_HEIGHT * GRID_CELL_SIZE) + 1

#define FONT_PATH "font/OpenSans-Regular.ttf"
#define FONT_SIZE 30

using std::cout;
using std::cerr;

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
extern std::map<P_Type, std::string> p_type_to_str;
extern std::map<P_Color, std::string> p_color_to_str;

struct Coord {
	int x, y;
	Coord() : x(-1), y(-1) {}
	Coord(int x, int y) : x(x), y(y){}
	// print Coord
	friend std::ostream& operator<<(std::ostream& out, const Coord& c);
	
	// Add two Coords
	Coord operator+(Coord b){ return Coord(x + b.x, y + b.y); }

	// Subtract two Coords
	Coord operator-(Coord b){ return Coord(x - b.x, y - b.y); }

	// Compare two Coords (equal)
	bool operator==(Coord b){ return (x == b.x and y == b.y); }

	// Compare two coords (not equal)	
	bool operator!=(Coord b){ return not (x == b.x and y == b.y); }

	std::string getChessCoordStr(){
		char letter = x + 'A';
		int number = coord_conversion_map[y]+1;
		return letter + std::to_string(number);
	}
};

extern Coord empty_coord;
#endif // GLOBAL_H

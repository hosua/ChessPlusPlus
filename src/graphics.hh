#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "global.hh"
#include <string>

class GFX {
public:
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect cursor; 
	SDL_Rect cursor_ghost;
	SDL_Rect highlight;
	
	TTF_Font* font;

	GFX() : window(nullptr), surface(nullptr), renderer(nullptr), texture(nullptr),
	font(nullptr), in_piece_selection(false) { 
		init(); 
	}

	~GFX(){ cleanQuit(); }

	void init();
	void clearScreen();
	void cleanQuit(bool success=true);

	void renderText(int x, int y, int w, int h, std::string text);
	void renderBoard();
	void renderCursorGhost();
	void renderHighlight();
	void renderPresent();
	void renderPieces(Board board);

	bool checkIfInPieceSelection(){ return in_piece_selection; }
	void setPieceSelection(bool flag){ in_piece_selection = flag; }

private:

	bool in_piece_selection;

	void initFont();
	void initRects();

	struct Colors {
		SDL_Color bg;
		SDL_Color cursor;
		SDL_Color cursor_ghost;
		SDL_Color white_sq, black_sq;
		SDL_Color font;
		SDL_Color highlight_piece;
	};
	Colors colors;

	void renderPiece(int x, int y, Piece piece);

	void setTheme(){
		colors.bg = {22,22,22,255};
		colors.cursor_ghost = {44,44,44,125};
		colors.cursor = {255,255,255,255};
		colors.black_sq = {111,78,55,255};
		colors.white_sq = {217,185,155,255};
		colors.font = {0,0,0,255};
		colors.highlight_piece = {24,245,50,125};
	}
	
};

#endif // GRAPHIC_H

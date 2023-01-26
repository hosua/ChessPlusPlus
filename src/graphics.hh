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
	
	TTF_Font* font;

	struct Colors {
		SDL_Color bg;
		SDL_Color cursor;
		SDL_Color cursor_ghost;
		SDL_Color white_sq, black_sq;
		SDL_Color font;
	};

	Colors colors;

	GFX() : window(nullptr), surface(nullptr), renderer(nullptr), texture(nullptr),
	font(nullptr) { 
		init(); 
	}

	~GFX(){ cleanQuit(); }

	void init();
	void initFont();
	void clearScreen();
	void cleanQuit(bool success=true);
	void renderText(int x, int y, int w, int h, std::string text);
	void renderBoard();
	void renderPresent();

	void renderPieces(Board board);

private:

	void renderPiece(int x, int y, Piece piece);

	void setTheme(){
		setSDLColor(22,22,22,255, &colors.bg);
		setSDLColor(44,44,44,125, &colors.cursor_ghost); 
		setSDLColor(255,255,255,255, &colors.cursor);
		setSDLColor(111,78,55,255, &colors.black_sq);
		setSDLColor(217,185,155,255, &colors.white_sq);
		setSDLColor(0,0,0,255, &colors.font);
	}

	void setSDLColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Color* color){
		color->r = r; color->g = g; color->b = b; color->a = a;
	}
	
};

#endif // GRAPHIC_H

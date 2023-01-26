#include "graphics.hh"
#include "chess.hh"

// Render a single piece
void GFX::renderPiece(int x, int y, Piece piece){
	P_Color color = piece.getColor();
	P_Type type = piece.getType();
	std::string s_color, s_type;
	s_color = (color == WHITE) ? "white" : "black";
	s_type = p_type_to_str[type];

	std::string filename = "sprites/" + s_color + "-" + s_type + ".png";

	SDL_Rect dest = {
		.x = x, .y = y,		
		.w = PIECE_SIZE, .h = PIECE_SIZE
	};
	surface = IMG_Load(filename.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
}

// Render all pieces
void GFX::renderPieces(Board board){
	for (int y = 0; y < GRID_HEIGHT; y++){
		for (int x = 0; x < GRID_WIDTH; x++){
			int board_x = (x * GRID_CELL_SIZE) + 5;
			int board_y = (y * GRID_CELL_SIZE) + 5;
			Piece* pc = board.grid[y][x];
			if (pc and pc->getType())
				renderPiece(board_x, board_y, *pc);
		}
	}
}

void GFX::renderText(int x, int y, int w, int h, std::string text){
	SDL_SetRenderDrawColor(renderer, 
			colors.font.r, colors.font.g, colors.font.b, 
			colors.font.a);

	SDL_Rect r;

	surface = TTF_RenderText_Solid(font, text.c_str(), colors.font);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	r = SDL_Rect {.x = x, .y = y, .w = w, .h = h};	

	SDL_RenderCopy(renderer, texture, NULL, &r);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void GFX::initFont(){
	TTF_Init();
	font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
	if (not font){
		cerr << "Error: Font file " << FONT_PATH << " is missing!\n";	
		exit(EXIT_FAILURE);
	}
}

void GFX::init(){
	window = SDL_CreateWindow("Maze", 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			SCREEN_X, SCREEN_Y, 
			0);

	if (not window){
		fprintf(stderr, "Fatal Error: Window failed to initialize\n");
		fprintf(stderr, "SDL2 Error: %s\n", SDL_GetError());
		cleanQuit(false);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = NULL;

	surface = SDL_GetWindowSurface(window);

	if (not surface){
		fprintf(stderr, "Failed to get surface from window.\n");
		fprintf(stderr, "SDL2 Error: %s", SDL_GetError());
		cleanQuit(false);
	}

	initFont();

	initRects();

	// Allows for transparent rendering
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	setTheme();
}

void GFX::cleanQuit(bool success){
	printf("Quitting, goodbye!\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (success) ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

void GFX::clearScreen(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // draw black screen
	SDL_RenderClear(renderer);
}

void GFX::renderPresent(){
	SDL_RenderPresent(renderer);
}

void GFX::renderBoard(){
	// Render A-H on the x-axis
	std::function <void()> renderBoardLetters = [&]() -> void {
		for (int i = 1; i <= GRID_HEIGHT; i++)
			renderText(((GRID_CELL_SIZE) * (i-1)) + (GRID_CELL_SIZE/2.5), 
						(GRID_CELL_SIZE * 8) - 25, 
						GRID_CELL_SIZE/5, 
						GRID_CELL_SIZE/3, 
						std::string(1, (i-1) + 'A'));
	};
	// Render 1-8 on the y-axis
	std::function <void()> renderBoardNumbers = [&]() -> void {
		for (int i = 1; i <= GRID_HEIGHT; i++)
			renderText(0, 
						((GRID_CELL_SIZE) * (i-1)) + GRID_CELL_SIZE/4, 
						GRID_CELL_SIZE/5, 
						GRID_CELL_SIZE/3, 
						std::to_string(i));
	};

	SDL_SetRenderDrawColor(renderer, 
			colors.bg.r, colors.bg.g, colors.bg.b,
			colors.bg.a);

	SDL_RenderClear(renderer);

	// Render the tiles of the chess board
	const int N = (GRID_HEIGHT*GRID_WIDTH)/2;
	SDL_Rect black_rects[N], white_rects[N];
	int i = 0, black_cnt = 0, white_cnt = 0;	

	for (int y = 0; y < GRID_HEIGHT; y++){
		for (int x = 0; x < GRID_WIDTH; x++){
			SDL_Rect r = {
				.x = (x * GRID_CELL_SIZE), .y = (y * GRID_CELL_SIZE),
				.w = GRID_CELL_SIZE, .h = GRID_CELL_SIZE
			};
			((++i) % 2 == 0) ? black_rects[black_cnt++] = r : white_rects[white_cnt++] = r;
		}
		++i;
	}

	SDL_SetRenderDrawColor(renderer,
			colors.black_sq.r, colors.black_sq.g, colors.black_sq.b,
			colors.black_sq.a);
	SDL_RenderFillRects(renderer, black_rects, black_cnt);

	SDL_SetRenderDrawColor(renderer,
			colors.white_sq.r, colors.white_sq.g, colors.white_sq.b,
			colors.white_sq.a);
	SDL_RenderFillRects(renderer, white_rects, white_cnt);
	
	// Render A-H on the x-axis
	renderBoardLetters();
	// Render 1-8 on the y-axis
	renderBoardNumbers();
}

void GFX::initRects(){
	cursor_ghost.x = cursor_ghost.y = -1;
	highlight.x = highlight.y = -1;
	cursor_ghost.w = cursor_ghost.h = GRID_CELL_SIZE;
	highlight.h = highlight.w = GRID_CELL_SIZE;
}

void GFX::renderCursorGhost(){
	// At start, do not render the cursor ghost until the player mouses over the game
	if (cursor_ghost.x == -1 and cursor_ghost.y == -1)
		return;

	SDL_Color c = colors.cursor_ghost;
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &cursor_ghost);
}

void GFX::renderHighlight(){
	if (checkIfInPieceSelection()){
		SDL_Color c = colors.highlight_piece;
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		SDL_RenderFillRect(renderer, &highlight);
	}
}

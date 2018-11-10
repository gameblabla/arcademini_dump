#ifndef _TEXTGUI_H_
#define _TEXTGUI_H_

extern SDL_Surface* font;

uint16_t gfx_font_width(SDL_Surface* inFont, char* inString);
uint16_t gfx_font_height(SDL_Surface* inFont);

void gfx_font_print(SDL_Surface* dest,  int16_t inX, int16_t inY, SDL_Surface* inFont, char* inString);
void gfx_font_print_char(SDL_Surface* dest, int16_t inX, int16_t inY, SDL_Surface* inFont, char inChar);
void gfx_font_print_center(SDL_Surface* dest, int16_t inY, SDL_Surface* inFont, char* inString);
void gfx_font_print_fromright(SDL_Surface* dest, int16_t inX, int16_t inY, SDL_Surface* inFont, char* inString);

SDL_Surface* gfx_tex_load_tga(const char* inPath);
SDL_Surface* gfx_tex_load_tga_from_array(uint8_t* buffer);


#endif

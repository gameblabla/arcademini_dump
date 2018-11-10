#ifdef SCALING
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "refresh.h"

#include "get_resolution.h"

void RefreshScreen(SDL_Surface* tmp)
{
	SDL_SoftStretch(tmp, NULL, real_screen, NULL);
	SDL_Flip(real_screen);
}

#endif

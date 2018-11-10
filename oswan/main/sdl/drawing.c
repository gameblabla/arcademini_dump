#include "drawing.h"
#ifdef SCALING
#include "gfx/SDL_rotozoom.h"
#endif

void Get_resolution(void)
{
#ifdef SCALING
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	screen_scale.w_display = info->current_w;
	screen_scale.h_display = info->current_h;
#else
	screen_scale.w_display = 320;
	screen_scale.h_display = 240;
#endif
}

void Set_resolution(uint16_t w, uint16_t h)
{
#ifdef SCALING
	screen_scale.w_scale = screen_scale.w_display / w;
	screen_scale.h_scale = screen_scale.h_display / h;
  
	screen_scale.w_scale_size = screen_scale.w_scale * w;
	screen_scale.h_scale_size = screen_scale.h_scale * h;
	
	screen_position.x = (screen_scale.w_display - screen_scale.w_scale_size)/2;
	screen_position.y = (screen_scale.h_display - screen_scale.h_scale_size)/2;	
#else
	screen_scale.w_display = w;
	screen_scale.h_display = h;
#endif

}

void SetVideo(uint8_t mode)
{
#ifdef SCALING	
	int32_t flags = FLAG_VIDEO | SDL_NOFRAME | SDL_FULLSCREEN;
#else
	int32_t flags = FLAG_VIDEO;
#endif
	uint16_t w = 320, h = 240;
	
	if (mode == 1) 
	{
		w = 224;
		h = 144;
	}
	
	if (!SDL_WasInit(SDL_INIT_VIDEO)) 
	{	
		SDL_Init(SDL_INIT_VIDEO);
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	#if defined(SCALING)
		if (real_screen) SDL_FreeSurface(real_screen);
		if (actualScreen) SDL_FreeSurface(actualScreen);
	#else
		if (actualScreen) SDL_FreeSurface(actualScreen);
	#endif
	
	#if !defined(NOSCREENSHOTS)
		if (screenshots) SDL_FreeSurface(screenshots);
	#endif
	
	Set_resolution(w, h);

	#if defined(SCALING)
		real_screen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
		actualScreen = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, BITDEPTH_OSWAN, 0,0,0,0);
	#else
		actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
	#endif
	
	#if !defined(NOSCREENSHOTS)
		screenshots = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, BITDEPTH_OSWAN, 0,0,0,0);
	#endif
}

void Set_DrawRegion(void)
{
	/* Clear screen too to avoid graphical glitches */
	SDL_FillRect(actualScreen, NULL, 0);

	if (GameConf.m_ScreenRatio == 2)
	{
		screen_to_draw_region.w	= 320;
		screen_to_draw_region.h	= 204;
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 18; 
	}
	else if (GameConf.m_ScreenRatio == 1)
	{
		screen_to_draw_region.w	= 320;
		screen_to_draw_region.h	= 240;
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 0; 
	}
	else if (GameConf.m_ScreenRatio == 0)
	{
		screen_to_draw_region.w	= 224;
		screen_to_draw_region.h	= 144;
#ifdef NATIVE_RESOLUTION
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 0; 
#else
		screen_to_draw_region.offset_x = ((actualScreen->w - SYSVID_WIDTH)/2);
		screen_to_draw_region.offset_y = ((actualScreen->h - SYSVID_HEIGHT)/2); 
#endif
	}
}

void screen_draw(void)
{
	uint16_t *buffer_scr = (uint16_t *) actualScreen->pixels;
	uint32_t W,H,ix,iy,x,y;
	
	SDL_LockSurface(actualScreen);
	
	x=screen_to_draw_region.offset_x;
	y=screen_to_draw_region.offset_y; 
	W=screen_to_draw_region.w;
	H=screen_to_draw_region.h;
	ix=(SYSVID_WIDTH<<16)/W;
	iy=(SYSVID_HEIGHT<<16)/H;
	
	buffer_scr += (y)*320;
	buffer_scr += (x);
	do   
	{
		uint16_t *buffer_mem=(uint16_t *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
		W=screen_to_draw_region.w; x=0;
		do 
		{
			*buffer_scr++=buffer_mem[x>>16];
#if BITDEPTH_OSWAN == 32
			*buffer_scr++=buffer_mem[x>>16];
#endif
			x+=ix;
		} while (--W);
		y+=iy;
#ifndef NATIVE_RESOLUTION
		if (screen_to_draw_region.w == 224) buffer_scr += actualScreen->pitch - 320 - SYSVID_WIDTH;
#endif
	} while (--H);
	
	static char buffer[4];
	if (GameConf.m_DisplayFPS) 
	{
#ifndef NATIVE_RESOLUTION
		if (GameConf.m_ScreenRatio == 2 || GameConf.m_ScreenRatio == 0)
#else
		if (GameConf.m_ScreenRatio == 2)
#endif
		{
			SDL_Rect pos;
			pos.x = 0;
			pos.y = 0;
			pos.w = 17;
			pos.h = 16;
			SDL_FillRect(actualScreen, &pos, 0);
		}
		sprintf(buffer,"%d",FPS);
		print_string_video(2,2,buffer);
	}
	
	SDL_UnlockSurface(actualScreen);
	flip_screen(actualScreen);
}

#if defined(SCALING)
void flip_screen(SDL_Surface* screen)
{
	SDL_SoftStretch(actualScreen, NULL, real_screen, &screen_position);
	SDL_Flip(real_screen);
}
#endif

void take_screenshot(void)
{
#if !defined(NOSCREENSHOTS)
	/* Save current screen in screenshots's layer */
	SDL_BlitSurface(actualScreen, NULL, screenshots, NULL);
#endif
}

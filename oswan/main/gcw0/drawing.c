#include "drawing.h"

void Get_resolution(void)
{
}

void Set_resolution(uint16_t w, uint16_t h)
{
	screen_scale.w_display = w;
	screen_scale.h_display = h;
}

void SetVideo(unsigned char mode)
{
	int32_t flags = FLAG_VIDEO;
	uint16_t w = 224, h = 144;
	
	if (m_Flag == GF_MAINUI) 
	{
		w = 320;
		h = 240;
	}
	else
	{
		if (mode == 2)
		{
			w = 224;
			h = 144;
		}
		else
		{
			w = 320;
			h = 240;	
		}
	}

	if (actualScreen) SDL_FreeSurface(actualScreen);
	
	#if !defined(NOSCREENSHOTS)
		if (screenshots) SDL_FreeSurface(screenshots);
	#endif
	
	Set_resolution(w, h);
	
	if (SDL_WasInit(SDL_INIT_VIDEO)) 
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

	actualScreen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, BITDEPTH_OSWAN, flags);
	
	#if !defined(NOSCREENSHOTS)
		screenshots = SDL_CreateRGBSurface(FLAG_VIDEO, w, h, BITDEPTH_OSWAN, 0,0,0,0);
	#endif
}

void Set_DrawRegion(void)
{
	/* Clear screen too to avoid graphical glitches */
	/* Clear screen too to avoid graphical glitches */
	SDL_FillRect(actualScreen, NULL, 0);

	if (GameConf.m_ScreenRatio == 2)
	{
		screen_to_draw_region.w	= 224;
		screen_to_draw_region.h	= 144;
		screen_to_draw_region.offset_x = 0;
		screen_to_draw_region.offset_y = 0; 
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
		screen_to_draw_region.offset_x = ((actualScreen->w - SYSVID_WIDTH)/2);
		screen_to_draw_region.offset_y = ((actualScreen->h - SYSVID_HEIGHT)/2); 
	}
	SetVideo(GameConf.m_ScreenRatio);
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
	
	if (GameConf.m_ScreenRatio != 2) 
	{
		buffer_scr += (y)*320;
		buffer_scr += (x);
	}
	
	do   
	{
		uint16_t *buffer_mem=(uint16_t *) (FrameBuffer+((y>>16)*SCREEN_WIDTH));
		W=screen_to_draw_region.w; x=0;
		do 
		{
			*buffer_scr++=buffer_mem[x>>16];
			x+=ix;
		} while (--W);
		y+=iy;
		if (GameConf.m_ScreenRatio == 0) buffer_scr += actualScreen->pitch - 320 - SYSVID_WIDTH;
	} while (--H);
	
	static char buffer[4];
	if (GameConf.m_DisplayFPS) 
	{
		if (GameConf.m_ScreenRatio == 2 || GameConf.m_ScreenRatio == 0)
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

void take_screenshot(void)
{
#ifndef NOSCREENSHOTS
	/* Save current screen in screenshots's layer */
	SDL_BlitSurface(actualScreen, NULL, screenshots, NULL);
#endif
}

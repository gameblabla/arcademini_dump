
#include "new_scaler.h"

/* Not*/


/* alekmaul's scaler taken from mame4all */
void bitmap_scale(int startx, int starty, int viswidth, int visheight, int newwidth, int newheight,int pitch, uint16_t *src, uint16_t *dst) {
  unsigned int W,H,ix,iy,x,y;
  x=startx<<16;
  y=starty<<16;
  W=newwidth;
  H=newheight;
  ix=(viswidth<<16)/W;
  iy=(visheight<<16)/H;

  do
  {
	uint16_t *buffer_mem=&src[(y>>16)*160];
	W=newwidth; x=startx<<16;
	do {
	  *dst++=buffer_mem[x>>16];
	  x+=ix;
	} while (--W);
	dst+=pitch;
	y+=iy;
  } while (--H);
}

/* end alekmaul's scaler taken from mame4all */

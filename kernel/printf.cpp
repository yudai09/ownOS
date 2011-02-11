#include"kcom.h"
#include"FileSystem.h"
#include"KGlobal.h"
#include"printf.h"
Font *font;
u8_t fgcolor;
u8_t bgcolor;
void putfont8(char *screen,char c){
	int i;
	char *p, d /* data */;
    char *afont = font->font_of(c);
	for (i = 0; i < 16; i++) {
      p = screen +  i * vram_info->vx;
		d = afont[i];
		if ((d & 0x80) != 0) { p[0] = fgcolor; }
		if ((d & 0x40) != 0) { p[1] = fgcolor; }
		if ((d & 0x20) != 0) { p[2] = fgcolor; }
		if ((d & 0x10) != 0) { p[3] = fgcolor; }
		if ((d & 0x08) != 0) { p[4] = fgcolor; }
		if ((d & 0x04) != 0) { p[5] = fgcolor; }
		if ((d & 0x02) != 0) { p[6] = fgcolor; }
		if ((d & 0x01) != 0) { p[7] = fgcolor; }
	}
	return;
}

/*解像度を挙げた後のprintf*/
void cls(){
  int xsize = vram_info->vx;
  int ysize = vram_info->vy;
  unsigned char *screen = vram_info->vram;
  for(int i=0;i<ysize;i++){
    for(int j=0;j<xsize;j++){
      screen[j+i*xsize]=bgcolor;
    }
  }
}

u32_t xpos;
u32_t ypos;
u32_t COLUMNS;
u32_t LINES;

void vx_putchar(char c){
  u32_t screen_x = vram_info->vx;
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      //clear line
      for(int i=xpos;i<COLUMNS;i++){
        //*(video + (i + ypos *COLUMNS) *2) = 0;
        putfont8((char *)((u32_t)vram_info->vram+i*Font::vx+ypos*screen_x*Font::vy),' ');
      }
      if (ypos >= LINES){
        ypos = 0;
      }
      return;
    }
  // *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
  // *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
  putfont8((char *)((u32_t)vram_info->vram+xpos*Font::vx+ypos*screen_x*Font::vy),c);
  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

/* Format a string and print it on the screen, just like the libc
   function printf. */

void vx_itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /* If %d is specified and D is minus, put `-' in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF. */
  *p = 0;

  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}
void vx_printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0)
    {
      if (c != '%')
	vx_putchar (c);
      else
	{
	  char *p;

	  c = *format++;
	  switch (c)
	    {
	    case 'd':
	    case 'u':
	    case 'x':
	      vx_itoa (buf, c, *((int *) arg++));
	    p = buf;
	    goto string;
	    break;

	    case 's':
	      p = *arg++;
	      if (! p)
            p = (char *)"(null)";

	    string:
	      while (*p)
		vx_putchar (*p++);
	      break;

	    default:
	      vx_putchar (*((int *) arg++));
	      break;
	    }
	}
    }
}

void init_vx_printf(){
 
  //  kprintf("%x%x \n",font[4094],font[4095]);
  font = new Font();
  fgcolor = 0;
  bgcolor = 7;
  u32_t cx = vram_info->vx/Font::vx;
  u32_t cy = vram_info->vy/Font::vy;
  cls();

  xpos = 0;
  ypos = 0;
  COLUMNS = cx;
  LINES   = cy;

  for(int j=0;j<10;j++)
    for(int i='a';i<'z';i++){
      vx_putchar((char)i);
    }
  vx_putchar((char)'\n');
 for(int j=0;j<10;j++)
    for(int i='a';i<'z';i++){
      vx_putchar((char)i);
    }
 kprintf = vx_printf;
  //  putfont8((char *)vram_info->vram+0x100,(char)'a');

  // for(int i=0;i<200;i++)
  //   kprintf("%x",*(fonts+'a'*16+i));
}

// class Font
Font::Font(){
  File *file = fs->fopen(kstring((char *)"/hankaku.bin"));
  fonts = new char[4096];
  file->read(0,4096,fonts);
}
char *Font::font_of(char c){
 char *afont = fonts+16*c;
 return afont;
}

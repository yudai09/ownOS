#ifndef VGA_H
#define VGA_H

/* void init_qemuvga0(int x, int y, int c, int flag); */
/* void init_palette(void); */
namespace Vram{
  class VramInfo{
  public:
    const static int nr_color = 16;
    unsigned char table_rgb[nr_color * 3];
    unsigned int vx;
    unsigned int vy;
    unsigned char *vram;
    VramInfo();
  };
  void set_qemuvga0reg(int reg, int dat);
  void init_qemuvga0(int x, int y, int c, int flag);
  void set_palette(int start, int end);
  void init_palette(void);
  void boxfill8(unsigned char c, int x0, int y0, int x1, int y1);
  void init_vga();
};





#endif


#ifndef PRINTF_H
#define PRINTF_H
class Font{
  char *fonts;
 public:
  static const u32_t vx=8;
  static const u32_t vy=16;
  Font();
  char *font_of(char c);
};
/* class Terminal{ */
/*   u32_t x,y; */
/*   u32_t x_size,y_size; */

/* }; */
void init_vx_printf();
void vx_printf(const char *format,...);
#endif

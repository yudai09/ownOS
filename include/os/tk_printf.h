#ifndef TK_PRINTFH
#define TK_PRINTFH
/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

void tk_cls (void);
void tk_itoa (char *buf, int base, int d);
void tk_putchar (int c);
void tk_printf (const char *format, ...);
#endif

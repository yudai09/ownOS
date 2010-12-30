/*
 * temp_output.h
 *
 *  Created on: 2010/03/07
 *      Author: yudai
 *   カーネルが初期化されるまで出力を行うための一時的な出力関数
 *
 */

#ifndef TEMP_OUTPUT_H_
#define TEMP_OUTPUT_H_
namespace temp_output{//make static (local)
/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000
/* Variables. */
/* Save the X position. */
static int xpos;
/* Save the Y position. */
static int ypos;
/* Point to the video memory. */
static volatile unsigned char *video;
/* Forward declarations. */
void screen_cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void tk_printf (const char *format, ...);

}
#endif /* TEMP_OUTPUT_H_ */

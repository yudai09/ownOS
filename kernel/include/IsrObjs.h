/*
 * IsrObjs.h
 *
 *  Created on: 2010/03/07
 *      Author: yudai
 */

#ifndef ISROBJS_H_
#define ISROBJS_H_

extern void (*IRQhandler[0x0F][0x0F])(void);
extern void (*IRQfunc[0x0F][0x0F])(void);

#endif /* ISROBJS_H_ */

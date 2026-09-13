#ifndef _RCR_PROCEED_H
#define _RCR_PROCEED_H

#include <atari.h>
#include <stdint.h>

extern uint8_t proceed_flag; // indica si se ha recibido una interrupcion

extern void proceed_handler();  // handler interrupciones linea proceed
extern void proceed_enable();   // habilita as interrupciones linea proceed
extern uint8_t proceed_check(); // para saber si ha llegado una interrupcion
extern void proceed_restore();  // restaura estado de interrupcion proceed

#endif // _RCR_PROCEED_H

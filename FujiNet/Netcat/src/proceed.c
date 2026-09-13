// tomadas y adaptadas desde
//   https://github.com/FujiNetWIFI/netcat/tree/main/src/atari

#include "proceed.h"

uint8_t proceed_flag = 0; // if attention=1, fujinet is asking us for attention.
static void *old_vprced;  // old PROCEED vector, restored on exit.
static uint8_t old_enabled = 0; // were interrupts enabled for old vector.

// habilita interrupciones linea proceed del bus SIO
void proceed_enable() {
    old_vprced = OS.vprced;      // save the old interrupt vector
    old_enabled = PIA.pactl & 1; // keep track of old interrupt state
    PIA.pactl &= (~1);           // Turn off interrupts before changing vector
    OS.vprced = proceed_handler; // Set PROCEED interrupt vector to our
                                 // interrupt handler.
    PIA.pactl |= 1; // Indicate to PIA we are ready for PROCEED interrupt.
}

// retoorna 1 se activo la linea proceed
uint8_t proceed_check() {
    if (!proceed_flag)
        return 0;

    proceed_flag = 0;
    PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
    return 1;
}

// restaura estado de las interrupciiones linea proceed de SIO
void proceed_restore() {
    PIA.pactl &= ~1; // disable interrupts
    OS.vprced = old_vprced;
    PIA.pactl |= old_enabled;
    proceed_flag = 0;
}

#include <Arduino.h>

//#define SIGNAL_IN   PA5
//#define SIGNAL_OUT  PA6

void setup() {
  // PA5 input / PA6 output
  GPIOA->regs->CRL = (GPIOA->regs->CRL & 0xF00FFFFF) | 0x03800000;
  GPIOA->regs->ODR |= 0b01000000;
}

void loop() {
  // sicronizamos al inicio de la onda
  while((GPIOA->regs->IDR & 0b00100000) == 0);

  unsigned long int m1, m2, dt;
  m1 = micros();
  while(1) {
    // medimos el tiempo que dura la onda completa
    while((GPIOA->regs->IDR & 0b00100000) != 0);
    while((GPIOA->regs->IDR & 0b00100000) == 0);
    m2 = micros();
    dt = m2 - m1;


    // convertimos la frequencia a MARK o SPACE segun definicion dela lectura desde caset
    
    // MARK 5327Hz, 187.7229 micros
    if(dt<200)
      GPIOA->regs->ODR |= 0b01000000;
      
    // SPACE 3995Hz, 250.3129 micros
    else
      GPIOA->regs->ODR &= 0b10111111;

    m1 = m2;
  }
}

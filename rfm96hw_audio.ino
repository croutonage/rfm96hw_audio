#include <SPI.h>

#include "lplRFM69registersh.h"
#include "lplRFM69h.h"

#include "waakhond.h"

RFM69 radio(SPI_CS, RF69_IRQ_PIN, true);

#define SAMPLE_FREQ  8000L

void setup() {
  Serial.begin(115200);
  Serial.println("Go!");

  radio.initialize(RF69_868MHZ, 213, 1);
  radio.setHighPower(true);
  radio.setPowerLevel(0);

  radio.setMode(RF69_MODE_TX);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 250000/SAMPLE_FREQ;                // compare match register 16MHz/64/x
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10) | (1 << CS11);    // 64 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts  
}

ISR(TIMER1_COMPA_vect)
{
  static int addr = 44;
  uint8_t b = pgm_read_byte_near(waakhond_wav + addr);
  radio.writeReg(0x09, b);
  
  addr++;
  if (addr >= waakhond_wav_len) {
    addr = 44;
  }
}

void loop() {
  // runs in interrupt, nothing to do here
}

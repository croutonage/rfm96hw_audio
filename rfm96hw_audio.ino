#include <SPI.h>

#include "lplRFM69registersh.h"
#include "lplRFM69h.h"

RFM69 radio(SPI_CS, RF69_IRQ_PIN, true);

#define SAMPLE_FREQ  10000L

static int tabel[16];

void setup() {
  Serial.begin(115200);
  Serial.println("Go!");

  radio.initialize(RF69_868MHZ, 213, 1);
  radio.setHighPower(true);
  radio.setPowerLevel(0);

  radio.setMode(RF69_MODE_TX);
  
  int i;
  for (i = 0; i < 16; i++) {
    tabel[i] = 120 * (sin(i / 16.0 * 2 * PI) + 1.0);
  }
}

int t = 0;
double freq1 = 1000.0, freq2 = 500.0;

void loop() {
  int vi = tabel[t];

  if (vi < 0)
    vi = 0;
  else if (vi > 255)
    vi = 255;
  
  radio.writeReg(0x09, vi);
  
  t = (t + 1) % 16;
  
  delayMicroseconds(1);
}

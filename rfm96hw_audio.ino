#include <SPI.h>

#include "lplRFM69registersh.h"
#include "lplRFM69h.h"

RFM69 radio(SPI_CS, RF69_IRQ_PIN, true);

#define SAMPLE_FREQ  15000

void setup() {
  Serial.begin(115200);
  Serial.println("Go!");

  radio.initialize(RF69_868MHZ, 213, 1);
  radio.setHighPower(true);
  radio.setPowerLevel(31);

  radio.setMode(RF69_MODE_TX);
}

int t = 0;
double freq1 = 8000.0, freq2 = 500.0;

void loop() {
  double v2 = sin((((double)t * freq2) / (double)SAMPLE_FREQ) * PI / 180.0);
  double vd = sin((((double)t * (freq1 + 8000.0 * v2)) / (double)SAMPLE_FREQ) * PI / 180.0) * 127.5 + 127.5;
  int vi = (int)vd;

  if (vi < 0)
    vi = 0;
  else if (vi > 255)
    vi = 255;
  
  radio.writeReg(0x09, vi);
  
  t++;
  
  if (t >= SAMPLE_FREQ)
    t = 0;
  
  delayMicroseconds(1000000 / SAMPLE_FREQ);
}

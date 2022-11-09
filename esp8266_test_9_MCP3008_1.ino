//1-30-2021
//another esp8266 test with mcp3008, testing possible speeds and errors

#include <SPI.h>

#define CS_PIN 15 // Use 10 when compiling for Arduino

int adcRead(int channel) {
  if ((channel > 7) || (channel < 0)) {
    return -1;
  }

  digitalWrite(CS_PIN, LOW);

  SPI.transfer(1);
  uint8_t r1 = SPI.transfer((channel + 8) << 4);
  uint8_t r2 = SPI.transfer(0);

  digitalWrite(CS_PIN, HIGH);

  return ((r1 & 3) << 8) + r2;
}

void setup() {
  pinMode(CS_PIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setFrequency(3600000);
  Serial.begin(250000);
  digitalWrite(CS_PIN, HIGH);
}

int samples[1000];

void loop() {
  //Serial.println(adcRead(0));
  for (int n = 0; n < 1000; n++) {
    samples[n] = adcRead(0);
    //delayMicroseconds(100);
  }
  String outString;
  for (int n = 0; n < 1000; n++) {
    outString.concat(samples[n]);
    outString.concat("\n"); //ascii newline
    //Serial.println(samples[n]);
  }
  Serial.println(outString);
  //delay(4); // If a true delay of 4ms, sampling rate 250,000 Hz (MCP3008 max 200kHz)
}

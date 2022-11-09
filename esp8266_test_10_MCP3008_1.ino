//2-6-2021
//

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
  SPI.setFrequency(8000000);
  Serial.begin(500000);
  digitalWrite(CS_PIN, HIGH);
}

int samples[1000];
unsigned long t1, t2, t3, t4, t5, tdiff; //time 1, time 2, time3, time 4, time 5, time difference

void loop() {
  //Serial.println(adcRead(0));
  t1 = millis();
  for (int n = 0; n < 1000; n++) {
    samples[n] = adcRead(0);
    //delayMicroseconds(100);
  }
  t2 = millis();
  tdiff = t2 - t1;
  String outString;
  for (int n = 0; n < 1000; n++) {
    outString.concat(samples[n]);
    outString.concat(" "); //ascii newline
    //Serial.println(samples[n]);
  }
  t3 = millis();
  Serial.println(outString);
  t4 = millis();
  Serial.print("time = "); // ~16ms == ~62.5 kHz sampling (faster SPI speed of 8 MHz -> ~13ms == ~77 kHz)(@clock speed 160MHz -> ~8 to 9 ms -> ~125 ksps)
  Serial.print(tdiff);
  Serial.print("  ");
  Serial.println(t3 - t2); // ~24ms
  Serial.print("  ");
  Serial.println(t4 - t3); // ~75ms (with baud rate of 500,000 baud, down to ~38ms)
  Serial.print("  ");
  Serial.println(millis() - t4); // ~1ms
  //delay(4); // If a true delay of 4ms, sampling rate 250,000 Hz (MCP3008 max 200kHz)
}

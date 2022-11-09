//2-6-2021
//

//#include <ESP8266WiFi.h>
#include <SPI.h>

#define CS_PIN 15 // Use 10 when compiling for Arduino

int sample = 0;
long int samplenum = 0;

int ICACHE_RAM_ATTR adcRead(int channel) {
  //if ((channel > 7) || (channel < 0)) {
  //  return -1;
  //}

  digitalWrite(CS_PIN, LOW);

  SPI.transfer(1);
  uint8_t r1 = SPI.transfer((channel + 8) << 4);
  uint8_t r2 = SPI.transfer(0);

  digitalWrite(CS_PIN, HIGH);

  return ((r1 & 3) << 8) + r2;
}


#define LED 2  //On board LED
bool busy = false;
//=======================================================================
void ICACHE_RAM_ATTR onTimerISR() {
  samplenum++;
  if (!busy) {
    busy = true;
    //sample = adcRead(0);
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(1);
    uint8_t r1 = SPI.transfer((0 + 8) << 4);
    uint8_t r2 = SPI.transfer(0);

    digitalWrite(CS_PIN, HIGH);

    sample = ((r1 & 3) << 8) + r2;

    busy = false;
  }

}




void setup()
{
  pinMode(CS_PIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setFrequency(10000000);
  //Serial.begin(500000);
  //digitalWrite(CS_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("");

  pinMode(LED, OUTPUT);

  //Initialize Ticker
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  //timer1_write(5000000);
  timer1_write(80); //ksps 63ksps
  //timer1_write(250); //20ksps

}

int samples[1000];

void loop()
{
  delayMicroseconds(1000000);
  Serial.print(sample);
  Serial.print("  ");
  Serial.println(samplenum);
  samplenum = 0;
  //Serial.println(".");
}
//=======================================================================

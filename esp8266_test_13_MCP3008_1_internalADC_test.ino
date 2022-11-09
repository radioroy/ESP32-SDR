//2-5-2021
//internal ADC speed test

long int sample = 0;
int value = 0;




#define LED 2  //On board LED
bool busy = false;
//=======================================================================
void ICACHE_RAM_ATTR onTimerISR() {
  //digitalWrite(LED, !(digitalRead(LED))); //Toggle LED Pin
  /*if (!busy) {
    busy = true;
    sample = adcRead(0);
    busy = false;
  } */
  value = analogRead(A0);
  sample++;
}




void setup()
{
  //pinMode(CS_PIN, OUTPUT);
  //SPI.begin();
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE0);
  //SPI.setFrequency(2000000);
  //Serial.begin(500000);
  //digitalWrite(CS_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("");

  //pinMode(LED, OUTPUT);

  //Initialize Ticker
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  //timer1_write(5000000);
  timer1_write(700); //10ksps
  //timer1_write(8000); //10ksps

}

int samples[1000];

void loop()
{
  delayMicroseconds(1000000);
  Serial.print(value);
  Serial.print(" ");
  Serial.println(sample);
  sample=0;
  //Serial.println(".");
}
//=======================================================================

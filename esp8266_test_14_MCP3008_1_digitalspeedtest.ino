//2-5-2021
//more speed testing

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

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
  value = digitalRead(D1);
  value += digitalRead(D2);
  value += digitalRead(D3);
  value += digitalRead(D4);
  value += digitalRead(D5);
  value += digitalRead(D6);
  value += digitalRead(D7);
  value += digitalRead(D8);

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
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  //pinMode(LED, OUTPUT);

  //Initialize Ticker
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  //timer1_write(5000000);
  timer1_write(40); //10ksps
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

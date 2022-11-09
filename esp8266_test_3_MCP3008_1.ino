//1-24-2021
//test of esp8266 with mcp3008 ADC

#include <MCP3008.h>

//MCP3008 adc(5, 7, 6, 8);
#define CS_PIN 5
#define CLOCK_PIN 8
#define MOSI_PIN 6
#define MISO_PIN 7
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("------------------------------------------");
//Serial.println("Value:" + String(adc.readADC(0)));
 int val = adc.readADC(0); // read Channel 0 from MCP3008 ADC (pin 1)
 Serial.println(val);
delay(1000);
}

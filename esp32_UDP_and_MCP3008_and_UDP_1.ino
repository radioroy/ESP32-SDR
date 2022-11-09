//2-20-2021

#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "NETWORKNAME";
const char * networkPswd = "PWD";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char * udpAddress = "192.168.86.25";
const int udpPort = 25565; //minecraft server port, so I know it is most likely not in use

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

}

#define CS_PIN 5 // Use 10 when compiling for Arduino UNO

short int sample = 0;
long int samplenum = 0;



#define LED 2  //On board LED
bool busy = false;

int n = 0;
//=======================================================================


void setup()
{

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);

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

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true); //1,000,000 = every 1 sec
  timerAlarmEnable(timer);

}

int samples[1000];

void loop()
{

  if (interruptCounter > 0) {

    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

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
    Serial.println(sample);
    udp.beginPacket(udpAddress, udpPort);
    udp.write((const uint8_t*)&sample, sizeof(sample));
    udp.endPacket();

  }
}

void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(WiFi.localIP(), udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
//=======================================================================

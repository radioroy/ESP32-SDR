//2-19-2021

/*
    UDP Testing

*/
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

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

int n = 0;

void loop() {
  //only send data when connected
  if (connected) {
    //Send a packet
    udp.beginPacket(udpAddress, udpPort);
    //udp.printf("Seconds since boot: %lu", millis()/1000, n);
    short int nnn[400];  // make an array of 400 ints
    char n1[2];
    for (int j = 0; j < 400; j++){
      n = j%10;
      //itoa(n, n1, 10); //input, string, base (base 10 babee)
      nnn[j]=n;
    }
    
    
    //udp.printf(nnn);
    udp.write((const uint8_t*)nnn,sizeof(nnn));
    udp.endPacket();
    if (n < 8) {
      n++;
    } else {
      n = 0;
    }
  }
  //Wait for 1 second
  delay(10);
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

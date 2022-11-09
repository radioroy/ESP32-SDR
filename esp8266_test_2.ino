//1-20-2021
//web server test

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* LED details */
#define LEDon LOW
#define LEDoff HIGH
#define LEDPin 16

/* WiFi details */
#define ssid "SSID"
#define password "PWD"

ESP8266WebServer server(80);



void setup() {
  // Enable internal LED
  pinMode(LEDPin, OUTPUT);

  // Enable serial monitor
  Serial.begin(115200);

  // Start WiFi
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.begin(ssid, password);

  // Wait for the WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // We are connected! Show IP address
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define web server event handlers
  server.on( "/",       []() { handleButtonDisplay();  });
  server.on( "/LEDOn",  []() { SetLED(LEDPin, LEDon);  });
  server.on( "/LEDOff", []() { SetLED(LEDPin, LEDoff); });

  // Start webserver  
  server.begin();
  
  Serial.println ( "HTTP server started" );

  // Do some blinking so we know we're up and running
  SetLED(LEDPin, LEDon);
  delay(100);
  SetLED(LEDPin, LEDoff);
  delay(100);
  SetLED(LEDPin, LEDon);
  delay(100);
  SetLED(LEDPin, LEDoff);
  delay(100);
}

void loop() {
  // Keep repeating: looking for web server events
  server.handleClient();
} 


void SetLED(byte LedPinNo, byte LedState) {
  // Set LED on or OFF, output serial monitor info, and send the updated HTML
  digitalWrite(LedPinNo, LedState);
  Serial.print("LED turned ");
  Serial.println(LedState==LEDon ? "ON" : "OFF");
  handleButtonDisplay();
}

void handleButtonDisplay() {
  // Read the current LED state
  bool LEDIsOn = (digitalRead(LEDPin)==LEDon);

  // Build a String with all the HTML in it.
  String HTML = "";

  HTML += "<style>input[type=\"button\"] { width: 64px; padding: 2px; margin-bottom: 4px; }</style>";
  
  HTML += "<input type=\"button\" onClick=\"location.href='/LEDOn';\"";
  HTML += (LEDIsOn ? "Disabled" : "" );
  HTML += " value=\"ON\">";

  HTML += "<br>";
  
  HTML += "<input type=\"button\" onClick=\"location.href='/LEDOff';\"";
  HTML += (!LEDIsOn ? "Disabled" : "" );
  HTML += " value=\"OFF\">";

  // Send the string to the browser as HTML text
  server.send(200, "text/html", HTML);
}

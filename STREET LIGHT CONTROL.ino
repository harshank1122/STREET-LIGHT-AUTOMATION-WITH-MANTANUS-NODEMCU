#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1  D4


#define WLAN_SSID       "harshank"             // Your SSID
#define WLAN_PASS       "10203040"        // Your password

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "harshpatil"            // Replace it with your username
#define AIO_KEY         "cc7909601e5946fbbe76f97e19797794"   // Replace with your Project Auth Key

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe  Light1 = Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME "/feeds/fan");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(Relay1, OUTPUT);


  //Connecting to the WiFi access point
  Serial.println(); Serial.println();
  Serial.println("Connection to...");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&Light1);

 }

void loop() {
  

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)Light1.lastread);
      
      if (strcmp((char *)Light1.lastread, "ON") == 0) {
        digitalWrite(Relay1, HIGH); 
      }
      if (strcmp((char *)Light1.lastread, "OFF") == 0) {
        digitalWrite(Relay1, LOW); 
      }
    }
    
    
   
}
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}

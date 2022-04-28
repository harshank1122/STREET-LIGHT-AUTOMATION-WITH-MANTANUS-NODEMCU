#include <ESP8266WiFi.h>

int temt6000Pin = A0;

const char* ssid     = "harshank";  

const char* password = "10203040"; 



int LED =D4;  
float light;
int light_value;

const char* host = "maker.ifttt.com";     //IFTTT channel address





void setup() {



  pinMode(LED,OUTPUT);

  Serial.begin(115200);

  digitalWrite(LED,LOW); // By default Off state
  pinMode(temt6000Pin, INPUT);


  }



int value = 1;

void loop() {

   int light_value = analogRead(temt6000Pin);
light = light_value * 0.0976;// percentage calculation
Serial.println(light);
Serial.println(" %");

  
    if (light <= 30) {                    

       if (value == 1){

         WiFi.disconnect();

        Serial.println("Button Pressed");

       WiFi.begin(ssid, password); // connecting to wifi

       while (WiFi.status() != WL_CONNECTED) {

        Serial.println("Conencting");

       delay(500);

       }

       digitalWrite(LED,LOW);  // LED turns On

       WiFiClient client;

       const int httpPort = 80;

       if (!client.connect(host, httpPort)) {

        Serial.println("Weak Network");

       return;

       }

       // We now create a URI for the request

       String url = "/trigger/street/with/key/g3Yn2ZWbbFxGrxeWdsNErG71ExnFxfx3B62qnftbkkE";   //our link to trigger the event with special key and event name 

  

       // This will send the request to the server

       client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");  // GET request 

       

       value = 0;

       delay(5000);

       digitalWrite(LED,HIGH);  // LED turns Off

       }

    }

    else{

    value = 1;

    delay(500);

    }

}

#include <Blynk.h>
#include <ESP8266WiFi.h>  
#include <WiFiClient.h>  
#include <ThingSpeak.h>  
#include <BlynkSimpleEsp8266.h>
#define PIN V1
#define LOWL V2
#define MEDM V3
#define HIGHE V4
SimpleTimer timer;
char auth[] = "rAY3w5zmSHeRVQKwWZuDvh-tz-q5KWiI";
const char* ssid = "K";  //Hotspot name
const char* password = "hello1234";  //Hotspot password
WiFiClient client;  
unsigned long myChannelNumber = 1250414;  
const char * myWriteAPIKey = "766FD4UF9D6QWN5X";  


// defines pins numbers
const int trigPin = D5;  //D5
const int echoPin = D6;  //D6
const int buzzPin = D1;   //D1


// defines variables
long duration;
int distance;

int percent;
int d1;
void setup() {
Blynk.begin(auth, ssid, password);//Blynk is ON

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzPin, OUTPUT); // buzz pin is output to control buzzering
pinMode(BUILTIN_LED, OUTPUT); //initializing LED
Serial.begin(115200); // Starts the serial communication
// Connect to WiFi network  
  Serial.println();  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED)
   {  
   delay(500);  
   Serial.print(".");  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  // Print the IP address  
  Serial.println(WiFi.localIP());  
  ThingSpeak.begin(client);
  timer.setInterval(1000, sendPercent);
 
}
void sendPercent(){
  Blynk.virtualWrite(PIN, percent);
  }


void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the 0sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
d1 = 15-distance;
int volume;
volume = 3.14*2.5*2.5*d1;

percent = 100-(distance*100)/25;//Convert into percentage
Blynk.virtualWrite(V1,volume);
  //Sent data to blynk server
// Prints the distance on the Serial Monitor
//divide for output in 100digit
//delay(1000);
  

if (volume >=200 && volume<=270) {
      // Buzz
      
      Blynk.virtualWrite(V4,255);
      delay(2000);
      Blynk.virtualWrite(V4,0);
      Serial.print("Water tank is full");
      Serial.print("\t Volume in ml = ");
      Serial.print(volume);
      Serial.println();
      Blynk.email("kushal.h1999@gmail.com", "Water level", "Tank is Full !!!"); //your mail id
      digitalWrite(buzzPin, HIGH);
      digitalWrite(BUILTIN_LED, HIGH);// LED OFF
      
     
    } 
    else if(volume <200 && volume >=100) {
      Blynk.virtualWrite(V3,255);
      delay(2000);
      Blynk.virtualWrite(V3,0);
       Serial.print("Water level Medium");
       Serial.print("\t Volume in ml = ");
       Serial.print(volume);
       Serial.println();
       digitalWrite(BUILTIN_LED, LOW);// LED on
       digitalWrite(buzzPin, LOW);
       
      }
    else  {
      Blynk.virtualWrite(V2,255);
      delay(2000);
      Blynk.virtualWrite(V2,0);
      Serial.print("Water Level Low");
      Serial.print("\t Volume in ml = ");
      Serial.print(volume);
        Serial.println();
      digitalWrite(BUILTIN_LED, LOW);            // LED on
      digitalWrite(buzzPin, LOW);                // Buzzer OFF
     
      }
    Blynk.run();
    timer.run(); // Initiates SimpleTimer
  ThingSpeak.writeField(myChannelNumber, 1, volume, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, volume, myWriteAPIKey);
   delay(100); // ThingSpeak will only accept updates every 15 seconds.
  }

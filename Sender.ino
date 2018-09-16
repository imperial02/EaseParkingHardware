/*
  topchic
*/
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306.h" 
#include "images.h"

// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true
const int trigPin = 25;
const int echoPin = 34;
long duration;
int distance;

unsigned int counter = 0;

SSD1306 display(0x3c, 4, 15);
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  pinMode(16,OUTPUT);
  pinMode(25,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI00);
  
  if (!LoRa.begin(433E6))
  {
    display.drawString(0, 0, "Starting LoRa failed!");
    display.display();
    while (1);
  }
  display.drawString(0, 0, "LoRa Initial success!");
  display.display();
  delay(1000);
  
  Serial.begin(9600); // Starts the serial communication
}

void loop()
{
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);

String data = "";
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  
  if(distance>40)
    data = "0";
  else
    data = "1";
  // send packet
  
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
display.drawString(0, 0, data);
display.drawString(90, 0, String(counter));
display.display();
  
   
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second
}

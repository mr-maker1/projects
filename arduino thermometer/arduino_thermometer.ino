#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);

char temp[8];

void setup() {

  TFTscreen.begin();
  Serial.begin(9600);

  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255,255,255);
  TFTscreen.setTextSize(2);
 TFTscreen.text("Mr.maker", 30, 60);
  delay(2000);
  TFTscreen.background(0, 0, 0);
  TFTscreen.text("TEMPERATURE:", 10, 20);
}

void loop() {
float sensorVal = analogRead(A7);
float temperature = (sensorVal *500 ) /1023 ;
Serial.print(temperature);
dtostrf(temperature, 6, 2, temp);

  TFTscreen.fill(0, 0, 255);
  TFTscreen.rect(30, 50, 100, 40);
  
   TFTscreen.setTextSize(3);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(temp, 20, 60);
  delay(500);
}

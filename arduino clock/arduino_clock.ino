#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <TimeLib.h>
#include <DS1307RTC.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 

#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
delay(2000); }


void loop() {

tmElements_t tm;

  if (RTC.read(tm)) {
    
  
    display.clearDisplay();


display.setTextSize(1); 
display.setTextColor(WHITE); 
display.setCursor(0, 0); 
    display.print("Time = ");
    print2digits(tm.Hour);
    display.write(':');
    print2digits(tm.Minute);
    display.write(':');
    print2digits(tm.Second);

    display.setCursor(0,20); 
    display.print("Date= ");
    display.print(tm.Day);
    display.write('/');
    display.print(tm.Month);
    display.write('/');
    display.print(tmYearToCalendar(tm.Year));
    display.println();
    display.display();
}}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    display.write('0');
  }
  display.print(number);
}

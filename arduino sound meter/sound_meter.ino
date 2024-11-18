
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() 
{
   Serial.begin(9600);                                    //Serial comms for debugging
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);             //OLED display start
   display.display();                                     //show buffer
   display.clearDisplay();                                //clear buffer
   display.setTextSize(1);                                //Set text size to 1 (1-6)
   display.setTextColor(WHITE);                           //Set text color to WHITE (no choice lol)
   display.setCursor(30,0);                                //cursor to upper left corner
   display.println("Sound Meter");                        //write title
      display.setCursor(40,20);                                //cursor to upper left corner
   display.println("MR.maker");                        //write title

   display.display();                                     //show title
   delay(2000);                                           //wait 2 seconds
}

  
void loop() 
{
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);                             //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   float db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels
   display.setCursor(40,0);                                //cursor to upper left
   display.setTextSize(1);                                //set text size to 1
   display.print(db);                                     //write calibrated deciBels
   display.print(" dB");                                  //write units


    for(int x =6;x<122;x=x+6){                            //draw scale
      display.drawLine(x, 10, x, 14, WHITE);
    }
   display.drawRoundRect(0, 16, 128, 10, 3, WHITE);       //draw outline of bar graph
   int r = map(db,0,120,1,120);                           //set bar graph for width of screen
   display.fillRoundRect(0, 16, r, 10, 3, WHITE);         //draw bar graph with a width of r
   display.display();                                     //show all that we just wrote & drew
   display.clearDisplay();                                //clear the display
}

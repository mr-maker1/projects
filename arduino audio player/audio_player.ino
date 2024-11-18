#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 10 //Chip select is pin number 10
TMRpcm music; //Lib object is named "music"

int song_number=0;
boolean debounce1=true;
boolean debounce2=true;
boolean play_pause;

void setup(){
music.speakerPin = 9; //Auido out on pin 9
Serial.begin(9600); //Serial Com for debugging 
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

pinMode(2, INPUT_PULLUP); //Button 1 with internal pull up to chage track
pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to play/pause
pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to fast forward

music.setVolume(5);    //   0 to 7. Set volume level
music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal
//music.volume(0);        //   1(up) or 0(down) to control volume
//music.play("filename",30); plays a file starting at 30 seconds into the track    
}

void loop()
{ 

  
  if (digitalRead(2)==LOW  && debounce1 == true) //Button 1 Pressed
  {
  song_number++;
  if (song_number==5)
  {song_number=1;}
  debounce1=false;
  Serial.println("KEY PRESSED");
  Serial.print("song_number=");
  Serial.println(song_number);

  if (song_number ==1)
  {music.play("1.wav");} //Play song 1 from 10th second 

  if (song_number ==2)
  {music.play("2.wav");} //Play song 2 from 33rd second 

  if (song_number ==3)
  {music.play("3.wav");} //Play song 3 from start

  if (song_number ==4)
  {music.play("4.wav");} //Play song 4 from 25th second 
  
  }
  if (digitalRead(3)==LOW  && debounce2 == true) //Button 2 Pressed
  {
  music.pause();  Serial.println("PLAY / PAUSE");
  debounce2=false;}
  
  if (digitalRead(3)==HIGH)//Avoid debounce
  debounce2=true;
  
  if (digitalRead(2)==HIGH) //Avoid debounce
  debounce1=true;
 
}

int triggerPin = 7; //triggering on pin 7
int echoPin = 8;    //echo on pin 8
int buzzer = 9;
void setup() {

 
  pinMode(buzzer, OUTPUT);
  pinMode(triggerPin, OUTPUT); //defining pins
  pinMode(echoPin, INPUT);

}

void loop() {

  int duration, distance;    //Adding duration and distance

  digitalWrite(triggerPin, HIGH); //triggering the wave(like blinking an LED)
  delay(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH); //a special function for listening and waiting for the wave
  distance = (duration / 2) / 29.1; //transforming the number to cm(if you want inches, you have to change the 29.1 with a suitable number
  if (distance < 100  )
  {
digitalWrite(buzzer,HIGH);    
delay(500);
digitalWrite(buzzer,LOW);


  }
  
  else {
    digitalWrite(buzzer, LOW);
}}        

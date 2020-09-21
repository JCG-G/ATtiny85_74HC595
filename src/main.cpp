#include <Arduino.h>
#include <tinySPI.h>

const int
    LATCH_PIN(3), /// storage register clock (slave select)
    DATA_PIN(1),  /// data in
    CLOCK_PIN(2); /// shift register clock

const byte OE_PIN(0);

bool direction; // True = Green to Blue (G->B), False = Blue to Green (B->G)
bool alternate; // test git stuff

//int count;
//double repeatFunction = 6;
//int delayTime;
//double repeated;

//Setting Fucntions
//void fade();
void fadeTimer(int delayTime);

//Pattern Functions
void scanningLed(double repeatFunction);
void fadingLed(double repeatFunction);
void binaryCount(int repeatFunction);
void randomLed(double repeatFunction);
void alternateLed(double repeatFunction);

void setup(){
  ///**///
  /// pullups on for unused pins to minimize power consumption
  pinMode(4, INPUT_PULLUP);

  Serial.begin(9600);
  SPI.begin(); /// start hardware SPI (the library sets up the clock and data pins)

  pinMode(CLOCK_PIN, OUTPUT); /// set up the pins for software SPI
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT); /// latch pin needs to be set up for hardware or software
  pinMode(OE_PIN, OUTPUT);

  digitalWrite(LATCH_PIN, HIGH);

  analogWrite(OE_PIN, 255);
  ///**///
}

//implement fadeTimer
void loop(){
  int count = random(0,6);
  int repeatFunction = random(0,6);

  if(count == 0){
    scanningLed(repeatFunction);
  }
  else if (count == 1){
    fadingLed(repeatFunction);
  }
  else if (count == 2){
    binaryCount((int)repeatFunction);
  }
  else if (count == 3){
    randomLed(repeatFunction);
  }
  else {
    alternateLed(repeatFunction);
  }
}

///////////////////////////////////////////////////////
/// void fade()
/// {
///   analogWrite(OE_PIN, 255); /// First Part - Fade In from zero brightness
///   for (byte bright = 255; bright > 0; bright -= 5)
///   {                              /// loop from 255 to zero in increments of 5
///     analogWrite(OE_PIN, bright); /// and display the information / brightness of the s
///     delay(50);                   /// allows you to perceive the change in brightness. Reduce to make changes faster
///   }
///   analogWrite(OE_PIN, 0); /// Second Part - Fade Out from full brightness
///   for (byte bright = 0; bright < 255; bright += 5)
///   {                              /// loop from zero to 255 in increments of 5
///     analogWrite(OE_PIN, bright); /// and display the information / brightness of the s
///     delay(50);                   /// can reduce this for a faster fade
///   }
/// }
///////////////////////////////////////////////////////

// Test fade
void fadeTimer(int delayTime)
{
  analogWrite(OE_PIN, 255); /// First Part - Fade In from zero brightness
  for (byte bright = 255; bright > 0; bright -= 5)
  {                              /// loop from 255 to zero in increments of 5
    analogWrite(OE_PIN, bright); /// and display the information / brightness of the s
    delay(delayTime);            /// allows you to perceive the change in brightness. Reduce to make changes faster
  }

  analogWrite(OE_PIN, 0); /// Second Part - Fade Out from full brightness
  for (byte bright = 0; bright < 255; bright += 5)
  {                              /// loop from zero to 255 in increments of 5
    analogWrite(OE_PIN, bright); /// and display the information / brightness of the s
    delay(delayTime);            /// can reduce this for a faster fade
  }
}
////////////////////////////////////////////////////////

void alternateLed(double repeat){
  double timesRepeated = 0.0;
  analogWrite(OE_PIN, 0);

  while (timesRepeated < repeat){
    if (alternate == true){
     byte testByte = 85;
     digitalWrite(LATCH_PIN, LOW);
     fadeTimer(100);
     SPI.transfer(testByte);
     digitalWrite(LATCH_PIN, HIGH);
     alternate = false;
    }
    else  {
     byte testByte1 = 170;
     digitalWrite(LATCH_PIN, LOW);
     fadeTimer(100);
     SPI.transfer(testByte1);
     digitalWrite(LATCH_PIN, HIGH);
     alternate = true;
    }
    timesRepeated += 0.5; // double amount of times repeated
  }
}

void scanningLed(double repeat){
  double timesRepeated = 0.0;
  analogWrite(OE_PIN, 0);

  while (timesRepeated < repeat){
   if (direction == true){
      for (int b = 0; b < 8; b++){
        uint8_t myByte = 3 << b;
        digitalWrite(LATCH_PIN, LOW);
        delay(750); // Raise for faster scan, lower for slower scan
        SPI.transfer(myByte);
        digitalWrite(LATCH_PIN, HIGH);
      }
      direction = false;
    }
    else{
      for (int b = 0; b < 8; b++){
        uint8_t myByte = 192 >> b;
        digitalWrite(LATCH_PIN, LOW);
        delay(750);
        SPI.transfer(myByte);
        digitalWrite(LATCH_PIN, HIGH);
      }
      direction = true;
    }
    timesRepeated += 0.5; // scan from B->G counts as one, G->B counts as one
                          // set to 0.5 to double amount of times scan occurs
  }
}

void fadingLed(double repeat){
  double timesRepeated = 0.0;
  
  while (timesRepeated < repeat){
    if (direction == true){
      for (int b = 0; b < 8; b++){
        uint8_t myByte = 3 << b;
        digitalWrite(LATCH_PIN, LOW);
        fadeTimer(50); //raise for slower fade, lower for faster fade
        SPI.transfer(myByte);
        digitalWrite(LATCH_PIN, HIGH);
      }
      direction = false;
    }
    else{
      for (int b = 0; b < 8; b++){
        uint8_t myByte = 192 >> b;
        digitalWrite(LATCH_PIN, LOW);
        fadeTimer(50);
        SPI.transfer(myByte);
        digitalWrite(LATCH_PIN, HIGH);
        }
        direction = true;
      }
    timesRepeated += 1;
  }
}

void binaryCount(int repeat){
 int timesRepeated = 0.0;

 while (timesRepeated < repeat/2){ 
     for (int b = 0; b <= 255; b++) {
    //for (int b = 0; b <= 3; b++) {
      uint8_t myByte = b;
      digitalWrite(LATCH_PIN, LOW);
      fadeTimer(10);
      //delay(1000);
      SPI.transfer(myByte);
      digitalWrite(LATCH_PIN, HIGH);
      }
      // direction = true;
    timesRepeated += 1;
 }
 // Implement binary count in reverse direction
}

void randomLed(double repeat){
  double timesRepeated = 0.0;
  analogWrite(OE_PIN, 0);
 
 while (timesRepeated < repeat){
    uint8_t myByte = random(1, 140);
    digitalWrite(LATCH_PIN, LOW);
    fadeTimer(50);
    //delay(1000);
    SPI.transfer(myByte);
    digitalWrite(LATCH_PIN, HIGH);
    timesRepeated += 0.5 ;
  }
}
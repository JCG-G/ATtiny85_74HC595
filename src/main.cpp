#include <Arduino.h>
#include <tinySPI.h>

const int
    LATCH_PIN(3), /// storage register clock (slave select)
    DATA_PIN(1),  /// data in
    CLOCK_PIN(2); /// shift register clock

const byte OE_PIN(0);

bool direction; // True = Green to Blue (G->B), False = Blue to Green (B->G)
bool alternate; 

//int count;
//double repeatFunction = 6;
//int delayTime;
//double repeated;

//Setting Fucntions
//void fade();
void fadeTimer(int delayTime);

//Pattern Functions
void scanLeds(double repeatFunction);
void fadLeds(double repeatFunction);
void binaryCount(int repeatFunction);
void randomLeds(double repeatFunction);
void alternateLeds(double repeatFunction);

//Comments with three slashes "///" are borrowed code from the tinyspi lib
//cannot remember where I found the void fade() function, but it is also borrowed

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
    scanLeds(repeatFunction);
  }
  else if (count == 1){
    fadLeds(repeatFunction);
  }
  else if (count == 2){
    binaryCount((int)repeatFunction);
  }
  else if (count == 3){
    randomLeds(repeatFunction);
  }
  else {
    alternateLeds(repeatFunction);
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
  analogWrite(OE_PIN, 255); 
  for (byte bright = 255; bright > 0; bright -= 5){                              
    analogWrite(OE_PIN, bright); 
    delay(delayTime);            
  }

  analogWrite(OE_PIN, 0); 
  for (byte bright = 0; bright < 255; bright += 5){                              
    analogWrite(OE_PIN, bright); 
    delay(delayTime);            
  }
}

//Alternate LEDs as so: 10101010 to 01010101
void alternateLeds(double repeat){
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
    timesRepeated += 0.5; 
  }
}

//Knightrider effect on Leds
void scanLeds(double repeat){
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
    timesRepeated += 0.5; //B->G is one repeat, G->B is another repeat
                          //set to 0.5 to double amount of times scan occurs
  }
}

//Faded knightrider effect
void fadLeds(double repeat){
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
    timesRepeated += 1; //G->B is one count but its slow enough to keep when using fade()
  }
}

//Light up all Leds using binary from 0 to 255
void binaryCount(int repeat){
 int timesRepeated = 0.0;
 while (timesRepeated < repeat/2){ 
     for (int b = 0; b <= 255; b++) {
      uint8_t myByte = b;
      digitalWrite(LATCH_PIN, LOW);
      fadeTimer(10);
      //delay(1000);
      SPI.transfer(myByte);
      digitalWrite(LATCH_PIN, HIGH);
      }
    timesRepeated += 1;
 }
 //TODO: Implement binary count in reverse direction
}

//Randomly light up any one or more of the eight Leds
void randomLeds(double repeat){
  double timesRepeated = 0.0;
  analogWrite(OE_PIN, 0);
 while (timesRepeated < repeat){
    uint8_t myByte = random(1, 140);
    digitalWrite(LATCH_PIN, LOW);
    fadeTimer(50);
    //delay(1000);
    SPI.transfer(myByte);
    digitalWrite(LATCH_PIN, HIGH);
    timesRepeated += 0.5;
  }
}
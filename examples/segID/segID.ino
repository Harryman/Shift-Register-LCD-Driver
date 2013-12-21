/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ Donate: 15Hnv9T4fr9uzKMNgW5Vbop52Aq9YwSYvz
/ The Everything Corp

Segment Name Map
    -   ->    a
   | |  ->  f   b 
    -   ->    g
   | |  ->  e   c
    - x ->    d  dp

0 pin is located at the highest pin of the final(furthest from micro)shift register and fills from there

*/ 

#include "SPI.h"
///// THESE ARE THE VARIBLES YOU ARE PROBABLY GOING TO NEED TO CHANGE
uint8_t segs = 39;//number of segments including com/bp pin
uint16_t bp = 0; //com pin for backplane
uint8_t csPin = 9; //Chip Select Pin
uint8_t oePin = 8; // Output enable pin, haven't tried tying this on but would probably work
uint8_t displayArray[5];// change size according to the number of shift registers required to drive 


// other vars you shouldn't need to touch
uint16_t crnt = 1;
uint32_t lastTime = 0;
uint32_t crntTime = 0;
uint8_t shiftRegNum = 0;

void setup(){
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(LSBFIRST);
  pinMode(csPin, OUTPUT);
  pinMode(oePin, OUTPUT);
  segs--;
  shiftRegNum = (segs >> 3) + 1;
}

void loop(){
  if(crnt == bp){
    crnt++;
  }
  if(crnt > segs){
    crnt = 1;
  }
  for(uint16_t i = 1; i<= crnt; i++){
    SetAll('|');
    SetArray(i,'!');
    for(uint8_t i = 0; i<35; i++){
      Display();
      delay(10);
    }
    ClearDisp();
  }
  for(uint8_t i = 0; i<6; i++){
    SetAll('|');
    SetArray(crnt,'!');
    for(uint8_t i = 0; i<50;i++){
      Display();
      delay(10);
    }
    ClearDisp();
    SetAll('|');
    for(uint8_t i = 0; i<50; i++){
      Display();
      delay(10);
    }
    ClearDisp();
  }
  crnt++;
}



void SetArray(uint16_t segOn, char mathCommand){
  if((segOn <= segs || segOn > 0) && segOn != bp){
    uint8_t var = 0;
    uint8_t arrayNum = 0;
    arrayNum = segOn >> 3;    //divide by 8
    segOn ^= (arrayNum << 3); //multiply by 8 then subtract
    var = 1 << segOn;         //sets the approriate bit in var high
    if(mathCommand == '|'){   //turns on the led regardless of current state
        displayArray[arrayNum] |= var; 
    }
    if(mathCommand == '!'){   //turns off the led regardless of current state
      var = ~var;             // inverts byte
      displayArray[arrayNum] &= var;
    }
    if(mathCommand == '~'){   //flips led on->off, off->on
      displayArray[arrayNum] ^= var;
    }
  }
}
void SetAllBelow(uint8_t segsBelow, char allState){
  if(segsBelow > 0 && segsBelow <= segs){
    for(uint8_t x = segsBelow; x>0; x--){
      SetArray(x,allState);
    }
  }
}
void SetAllAbove(uint8_t segsAbove, char allState){
  if(segsAbove > 0 && segsAbove <= segs){
    for(uint8_t x = segsAbove; x<=segs; x++){
      SetArray(x,allState);
    }
  }
}

void SetAll(char allState){
   for(uint8_t x = segs; x>0; x--){
     SetArray(x,allState);
   }
}

void forceDisplay(){//useful if you don't have much looping code to put Display into
  Display();
  delay(10); //this is temporary take it out so it doesn't block
  Display();
}

void Display(){
  SPI.setBitOrder(LSBFIRST);
  digitalWrite(csPin, LOW);
  for(uint8_t i=0; i<shiftRegNum; i++){
    SPI.transfer(displayArray[i]);
    displayArray[i] = ~displayArray[i];
  }
  digitalWrite(csPin, HIGH);
}

void ClearDisp(){
  for(uint8_t i=0; i<shiftRegNum; i++){
    displayArray[i] = B00000000; // sets the array back to zero after shifting it out
  }
}
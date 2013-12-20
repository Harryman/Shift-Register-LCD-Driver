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

   
Digits are zero refrenced starting from least significant digit
Example:
 number is: 3756   3 7 5 6
 digits are        4 3 2 1

All which translates to 
3 = [a[4],b[4],c[4],d[4],g[4]]
7 = [a[3],b[3],c[3]]
5 = [a[2],c[2],d[2],f[2],g[2]]
6 = [a[1],c[1],d[1],e[1],f[1],g[1]]

*/ 

#include "SPI.h"

uint8_t segs = 39;//number of segments including com/bp pin
uint16_t bp = 0; //com pin for backplane
uint8_t csPin = 9;
uint8_t oePin = 8;
uint16_t crnt = 1;
uint32_t lastTime = 0;
uint32_t crntTime = 0;
uint8_t displayArray[5];
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
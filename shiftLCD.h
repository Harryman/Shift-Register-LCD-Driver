// file generated using https://github.com/Harryman/Arduino-header-keyword-creator

/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ The Everything Corp
// Please Donate if you find this useful: 1LZp3FUqoQVE6Y1Cc7XFpKsrWoz5sTjHe1


This library was dynamically generated using the python script in the library folder
To generateupload segID.ino after changing the variables approriately then run the script and
enter the digit-segment data as prompted

*/ 
#ifndef shiftLCD_h
#define shiftLCD_h
#include <stdint.h>
#include "Arduino.h"


class shiftLCD{
  public:
    shiftLCD(uint8_t csPin,uint8_t oePin);
    void setArray(uint8_t segOn);//sets the elements in the displayArray()
    void setChar(uint8_t digit, uint8_t value);//sets a digit using the pin map. dp & col must be set using setArray
    void display();//Shifts out the the display data while flipping the bits, so each time its ran is opposite as previous polarity
    void clearDisp();//sets display array to all zeros
    void print(char *str, uint8_t offset);
    void print(char c, int pos);
    uint8_t printInt(int32_t i); //values that are too large will be truncated 
    uint8_t printInt(int32_t i, uint8_t offset); //values that are too large will be truncated 
    uint8_t formatSet(uint32_t i, uint8_t offset, uint32_t maxValue);//truncates, sets display array, returns number of digits truncated
    uint32_t negHandler(int32_t num);// if num is negative sets the negitive segment and returns maxValue
    void printFloat(float number);//prints the interger section of the float
    void printFloat(float number, uint8_t decPlaces);//Displays float with specified number of decimal places, will move decimal when truncating
    void setCol(bool state, uint8_t digit);
    void setSeg(char seg, uint8_t digit);


      uint8_t halfD;
    int32_t negMax;
    uint8_t digits;
    uint8_t segs;
    int32_t maxValue;
    uint8_t shiftRegNum;
    uint8_t displayArray[5];
    uint8_t neg;
    uint8_t divdots;
    uint8_t com;
    uint8_t arrow;
private:
    uint8_t _csPin;
    uint8_t _oePin;
};
#endif
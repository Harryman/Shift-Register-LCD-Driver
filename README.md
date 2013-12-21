# Shift Register based LCD driver 
This library and python script allows you to drive an lcd with a single backplane with shift registers allowing for easier routing and cheap components


---------
## Quickstart Explaination video
[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/Slf3Xrs_wXg/0.jpg)](http://www.youtube.com/watch?v=Slf3Xrs_wXg)

## Available Functions
####void setArray(uint8_t segmentOn)
* Turns on the segment(unless it is the common/BP pin)

####void setChar(uint8_t digit, uint8\_t value)
* will set a digit to display the character or number given in the value

#### void display()
* will shift out the display buffer to shift registers
* inverts display buffer to so repeated calls will create the necessary AC waveform 

#### void clearDisp()
* zeros out the display buffer

#### void print( char *str, uint8_t offset)
* sets the display buffer with the given string
* starts string at the given offset

#### void printInt(int32_t integer)
* sets display buffer with the integer 
* truncates if it is too long
* returns number of truncated digits

#### void PrintInt(int32_t integer, uint8_t offset)
* sets display buffer with integer starting at the offset digit
* truncates if it is too long
* returns number of truncated digits

#### void printFloat(float number, uint8_t decPlaces)
* sets display buffer with float number with the set number of decPlaces

#### void setCol(uint8_t digit)
* set the display buffer with the colon segment for the given digit

#### void setSeg(char segment, uint8_t digit)
* turns on the segment of the digit
* accepts a-g 

__________
Contribute to my beerfund: 151sgJ4z2tNGzVFJHGyBAd8H4F4RT62ebQ

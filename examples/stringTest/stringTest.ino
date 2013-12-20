/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ Donate: 15Hnv9T4fr9uzKMNgW5Vbop52Aq9YwSYvz
/ The Everything Corp
*/

#include "SPI.h"
#include <shiftLCD.h>

shiftLCD test(9,8);
char alpha[] = {"abcdefghijlnopqrstuvxyz"};
void setup(){
}
void loop(){
	for(uint8_t i = 0; i<20; i++){
		test.print(alpha,i);
    test.display();
    delay(500);
    test.display();
    delay(500);
    test.clearDisp();
	}
}
/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ Donate: 15Hnv9T4fr9uzKMNgW5Vbop52Aq9YwSYvz
/ The Everything Corp
*/
#include "SPI.h"
#include <shiftLCD.h>


shiftLCD test(9,8);
void setup(){
}
void loop(){
	for(int i = -(test.negMax); i< test.maxValue;i++){
		test.printInt(i);
		test.display();
		delay(10);
		test.display();
		delay(10);
		test.clearDisp();
	}
}
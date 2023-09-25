#include <Arduino.h>
#ifndef pins
#define pins
#define ROW_COUNT 5
#define COL_COUNT 14
char rows[] = {
  PB0, // 1 ROW0 OK
  PA7, // 2 ROW1 OK
  PA6, // 3 ROW2
  PA5, // 4 ROW3
  PA4}; //5 ROW4
char cols [] = {
 PA3 // 6 col0   
,PA2 // 7 col1   
,PA1 // 8 col2   
,PA0 // 9 col3   
,PC15 // 10 col4 
,PC14 //11 col5  
,PC13 // 12 col6 
,PA8 // 13 col7  
//,PA11 // 13 col7
,PB15 // 14 col8  
,PA15 // 15 col9  
,PB3 // 16 col10  
,PB4 // 17 col11  
,PB5 // 18 col12  
,PB8 // 19 col13  
}; // PA2 - 7

#endif

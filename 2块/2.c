/*
LCD 12864 Arduino
RS = 17; Analog Pin3
RW = 16; Analog Pin2
EN = 18; Analog Pin4
D0  = 8; 
D1  = 9;
D2  = 10; 
D3  = 11; 
D4  = 4;
D5  = 5; 
D6  = 6; 
D7  = 7;
PIN15 PSB = 5V;
*/
 
#include "LCD12864R.h"
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
 
unsigned char show0[]={ 0xB5, 0xB1,  0xC7, 0xB0,  0xC4, 0xA3,  0xCA, 0xBD,  0xA3, 0xBA};//当前模式：
unsigned char show1[]={ 0xB6, 0xF9,  0xCD, 0xAF};     //儿童
unsigned char show2[]={ 0xB3, 0xC9,  0xC8, 0xCB};     //成人
int mod = 0;
void setup()
{
Serial.begin(9600);
LCDA.Initialise(); // 屏幕初始化
delay(100);
}
 
void loop()
{
LCDA.CLEAR();//清屏
delay(500);
LCDA.DisplayString(0,0,show0,AR_SIZE(show0));//第一行第一格开始，显示文字 当前模式
mod=Serial.read();

if(mod==30)
{
delay(500);
LCDA.DisplayString(1,5,show2,AR_SIZE(show1));;//第二行第1格开始，显示文字 成人
}
else if (mod==25)
{
delay(500);
LCDA.DisplayString(1,5,show1,AR_SIZE(show1));//第二行第1格开始，显示文字 儿童
}
}
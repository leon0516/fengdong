
#include "LCD12864R.h"
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
#include <NewPing.h>
#define up     0X7570
#define down   0X776E
#define right  0X6874
#define left   0X6674
#define TRIGGER_PIN  12 
#define ECHO_PIN     11 
#define MAX_DISTANCE 200

#define key1 2 
#define key2 3
#define key3 4
#define key4 5
uint16_t recivevalue;
uint8_t  r_buffer[4];
uint8_t num=0;
uint8_t mark0=0;
unsigned char hengxian[]={
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD,
  0xA3, 0xAD
     };                    //－－－－－－－－
unsigned char messageString[]={
  0xCE, 0xDE,
  0xCE, 0xFD,
  0xD6, 0xB0,
  0xD2, 0xB5,
  0xBC, 0xBC,
  0xCA, 0xF5,
  0xD1, 0xA7,
  0xD4, 0xBA
     };                    //无锡职业技术学院
unsigned char show0[]={
  0xB5, 0xB1,
  0xC7, 0xB0,
  0xC4, 0xA3,
  0xCA, 0xBD,
  0x3A, 0x00
     };                    //当前模式:

unsigned char show1[]={0xB6,0xF9,0xCD,0xAF};//儿童
unsigned char show2[]={
  0xC0, 0xCF,
  0xC8, 0xCB
     };       //老人
unsigned char show3[]={
  0xB3, 0xC9,
  0xC8, 0xCB
   };     //成人
unsigned char show4[]={
  0xB9, 0xD8,
  0xB1, 0xD5
     };                    //关闭
int pwmpin=13;
int val=0;
int pwmval=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
double cm;
double tempctrl,KP=1,KI=1,KD=0.3;
int cmset=0;
void setup()
{
pinMode(key1,INPUT); 
pinMode(key2,INPUT); 
pinMode(key3,INPUT); 
pinMode(key4,INPUT); 
pinMode(pwmpin,OUTPUT);
Serial.begin(9600);
LCDA.Initialise(); 
delay(100);
LCDA.CLEAR();//清屏
delay(100);
LCDA.DisplayString(0,0,messageString,AR_SIZE(messageString));//无锡职业技术学院
delay(100);
LCDA.DisplayString(1,0,hengxian,AR_SIZE(hengxian));//第一行第三格开始，显示文字
delay(100);
LCDA.DisplayString(2,0,show0,AR_SIZE(show0));//第一行第三格开始，显示文字
delay(100);
LCDA.DisplayString(3,0,hengxian,AR_SIZE(hengxian));//第一行第三格开始，显示文字
delay(100);
}
void loop()
{
	
	Bluetooth();
	  if(digitalRead(key1)==HIGH)
  {
    delay(10); 
    if(digitalRead(key1)==HIGH) 
    {
      _up();
    }
  }
  
  
     if(digitalRead(key2)==HIGH) 
  {
    delay(10); 
    if(digitalRead(key2)==HIGH) 
    {
      _down();
    }
  }
  
  if(digitalRead(key3)==HIGH) 
  {
    delay(10);
    if(digitalRead(key3)==HIGH) 
    {
      _left();
    }
  }
  
  
	if(digitalRead(key4)==HIGH)
	  {
		delay(10); 
		if(digitalRead(key4)==HIGH) 
		{
		  _right();
		}
	  } 
delay(50);                     
unsigned int uS = sonar.ping(); 
Serial.print(cmset);
Serial.print("Ping: ");
cm=uS / US_ROUNDTRIP_CM;
Serial.print("high:");
Serial.print(cm); 
Serial.println("cm");
if (cmset==30)
	{
	KP=1.9,KI=0.4,KD=0.3;
	}
if (cmset==20)
	{
	KP=2,KI=0.4,KD=0.4;
	}
if (cmset==40)
	{
	KP=1,KI=0.4,KD=0.3;
	}
		if(cmset==0)
		{
		pid0();
		}
		else
		{
		PID();
		}
analogWrite(pwmpin,tempctrl);
Serial.print ("pwmval:");
Serial.println (tempctrl);
lcd();
}
void PID()
{ 
	static double err0,err1,err2;// three value
	double add;  //The incremental
	err0=cmset-cm;
	add=KP*(err0-err1)+KI*err0+KD*(err0-2*err1+err2);
	tempctrl=tempctrl+add;
	//if(tempctrl>140)tempctrl=130;
	if(tempctrl<110)tempctrl=110;
	err2=err1;
	err1=err0; 
}
void pid0()
{
tempctrl=0;
}
void Bluetooth()
{

    if(Serial.available()>0)
    {
      uint8_t readbuf=Serial.read();
      r_buffer[num]=readbuf;
      num++;
      delay(2);
      Serial.println(num);
      if(readbuf==0X3B)
      {
        if(num==3)
        {

          recivevalue=(r_buffer[0]<<8)|(r_buffer[1]);

        }

        num=0;
        mark0=1;
      } 
    }
	
	if(mark0==1)
    {
      mark0=0;

      switch(recivevalue)
      {
      case up    : 
        _up();
        break;
      case down  : 
        _down();
        break;
      case left  : 
        _left();
        break;
      case right : 
        _right();
        break;
      }
    }
}



void _up()
{
cmset=0;
}

void _down()
{
cmset=20;
}

void _left()
{
cmset=30;
}

void _right()
{
cmset=40;
}


void lcd()
{
	if (cmset==20)
	{
	LCDA.DisplayString(2,5,show1,AR_SIZE(show1));
	delay(20);
	}
	if (cmset==30)
	{
	  LCDA.DisplayString(2,5,show2,AR_SIZE(show2));
	  delay(20);
	}
	if(cmset==40)
	{
	  LCDA.DisplayString(2,5,show3,AR_SIZE(show3));
	  delay(20);
	} 
	if(cmset==0)
	{
	  LCDA.DisplayString(2,5,show4,AR_SIZE(show4));
	  delay(20);
	} 
}
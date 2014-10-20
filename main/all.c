

#include "LCD12864R.h"
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )


#include <NewPing.h>
#define up     0X7570  //ctrl  HEX code
#define down   0X776E
#define right  0X6874
#define left   0X6674
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
//int potpin=0;//定义模拟接口0
uint16_t recivevalue;      //some transist recivevalue
uint8_t  r_buffer[4];
uint8_t num=0;
uint8_t mark0=0;
unsigned char show0[]={
  0xB5, 0xB1,
  0xC7, 0xB0,
  0xC4, 0xA3,
  0xCA, 0xBD,
  0x3A, 0x00
     };                    //当前模式:

unsigned char show1[]={  0xB6, 0xF9,  0xCD, 0xAF     };
unsigned char show2[]={
  0xC0, 0xCF,
  0xC8, 0xCB
     };       
unsigned char show3[]={
  0xB3, 0xC9,
  0xC8, 0xCB
     };     

int pwmpin=13;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
int pwmval=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
double cm;
double tempctrl,KP=1,KI=1,KD=0.3;
int cmset=0;
void setup()

{
pinMode(pwmpin,OUTPUT);//定义数字接口11 为PWM输出
//pinMode (pin, INPUT);
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
LCDA.Initialise(); // 屏幕初始化
LCDA.CLEAR();//清屏
delay(100);
}
void loop()
{
  Bluetooth();

delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print(cmset);
  Serial.print("Ping: ");
  cm=uS / US_ROUNDTRIP_CM;
  Serial.print("high:");
  Serial.print(cm); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
    if (cmset==30)
	{
	KP=0.975,KI=0.4735,KD=0.267445;
	}
if (cmset==20)
	{
	KP=0.975,KI=0.4,KD=0.267445;
	}
if (cmset==40)
	{
	KP=0.975,KI=0.4,KD=0.267445;
	}
  PID();
  analogWrite(pwmpin,tempctrl);// 输出PWM（PWM 输出最大值255）
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
      case up    : //Serial.println(up,HEX);
        _up();
        break;
      case down  : //Serial.println(down,HEX);
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
cmset=20;
}

void _down()
{
cmset=30;
 // analogWrite(l_motor,0);
  //analogWrite(r_motor,0);
}

void _left()
{
cmset=40;
//  analogWrite(l_motor,0);
 // analogWrite(r_motor,200);
}

void _right()
{
  cmset=0;
 // analogWrite(l_motor,200);
 // analogWrite(r_motor,0);
}


void lcd()
{

delay(100);
LCDA.DisplayString(0,0,show0,AR_SIZE(show0));//第一行第三格开始，显示文字极客工坊
delay(100);
if (cmset==20)
{
LCDA.DisplayString(0,5,show1,AR_SIZE(show1));
}
if (cmset==30)
{
  LCDA.DisplayString(0,5,show2,AR_SIZE(show2));
}
if(cmset==40)
{
  LCDA.DisplayString(0,5,show3,AR_SIZE(show3));
} 

}
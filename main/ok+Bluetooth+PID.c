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

int pwmpin=10;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
int pwmval=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
double cm;
double tempctrl,KP=1,KI=1,KD=0.3;
int cmset=30;
void setup()

{
pinMode(pwmpin,OUTPUT);//定义数字接口11 为PWM输出
//pinMode (pin, INPUT);
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
}
void loop()
{
  Bluetooth();
  Serial.print("CMSET:");
  Serial.print(cmset);
  Serial.println("cm");
  Serial.println();
delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  cm=uS / US_ROUNDTRIP_CM;
  Serial.print("high:");
  Serial.print(cm); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
//val=analogRead(potpin);// 读取传感器的模拟值并赋值给val
//Serial.println(val);//显示val 变量
//pwmval=val/4;
  PID();
  
  

analogWrite(pwmpin,tempctrl);// 输出PWM（PWM 输出最大值255）
Serial.print ("pwmval:");
Serial.println (tempctrl);
Serial.println ();
delay(500);//延时0.01 秒
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
cmset=cmset+1;
}

void _down()
{
cmset=cmset-1;
 // analogWrite(l_motor,0);
  //analogWrite(r_motor,0);
}

void _left()
{
//  analogWrite(l_motor,0);
 // analogWrite(r_motor,200);
}

void _right()
{
 // analogWrite(l_motor,200);
 // analogWrite(r_motor,0);
}
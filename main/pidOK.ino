#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
//int potpin=0;//定义模拟接口0
int pwmpin=10;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
int pwmval=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
float cm;
uint8_t tempctrl,KP=1,KI=1,KD=0.3;
uint8_t cmset=30;
void setup()

{
pinMode(pwmpin,OUTPUT);//定义数字接口11 为PWM输出
//pinMode (pin, INPUT);
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
}
void loop()
{
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
  static signed int err0,err1,err2;// three value
  signed int add;  //The incremental
  err0=cmset-cm;
  add=KP*(err0-err1)+KI*err0+KD*(err0-2*err1+err2);
  tempctrl=tempctrl+add;
  if(tempctrl>140)tempctrl=130;
  if(tempctrl<110)tempctrl=110;
  err2=err1;
  err1=err0;  
}


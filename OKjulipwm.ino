#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define pin A1
int potpin=0;//定义模拟接口0
int pwmpin=10;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
int pwmval=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int juli;
void setup()

{
pinMode(pwmpin,OUTPUT);//定义数字接口11 为PWM输出
pinMode (pin, INPUT);
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
}
void loop()
{
delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  juli=uS / US_ROUNDTRIP_CM;
  Serial.print("high:");
  Serial.print(juli); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
val=analogRead(potpin);// 读取传感器的模拟值并赋值给val
//Serial.println(val);//显示val 变量
pwmval=val/4;
analogWrite(pwmpin,pwmval);// 输出PWM（PWM 输出最大值255）
Serial.print ("pwmval:");
Serial.println (pwmval);
Serial.println ();
delay(500);//延时0.01 秒
}
uint16_t get_gp2d12 (uint16_t value) {
        if (value < 30)
                value = 30;
        return ((67870.0 / (value - 3.0)) - 40.0);
}


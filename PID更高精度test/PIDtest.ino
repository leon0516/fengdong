#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
int pwmpin=10;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
int pwmval=0;
float cm;
int PWMResolution = 900; //设置PWM占空比分辨率
double tempctrl,KP=2,KI=1,KD=0.1;
double cmset=30;
void setup()
{
pinMode(pwmpin,OUTPUT);//定义数字接口11 为PWM输出
//pinMode (pin, INPUT);
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
pinMode(TRIGGER_PIN, OUTPUT); 
pinMode(ECHO_PIN, INPUT); 
}
void loop()
{
digitalWrite(TRIGGER_PIN, LOW); 
delayMicroseconds(2); 
digitalWrite(TRIGGER_PIN, HIGH); 
delayMicroseconds(10); 
digitalWrite(TRIGGER_PIN, LOW); 
 
cm = pulseIn(ECHO_PIN, HIGH) / 58.0; //算成厘米 
cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
Serial.print(cm); 
Serial.print("cm"); 
Serial.println(); 
delay(500); 
//val=analogRead(potpin);// 读取传感器的模拟值并赋值给val
//Serial.println(val);//显示val 变量
//pwmval=val/4;
  PID();
  
  
pwm(tempctrl);// 输出PWM（PWM 输出最大值255）
Serial.print ("pwmval:");
Serial.println (tempctrl);
Serial.println ();
delay(500);//延时0.01 秒
}
void PID()
{ 
  static double  err0,err1,err2;// three value
  double  add;  //The incremental
  err0=cmset-cm;
  add=KP*(err0-err1)+KI*err0+KD*(err0-2*err1+err2);
  tempctrl=tempctrl+add;
  if(tempctrl>910)tempctrl=910;
  if(tempctrl<800)tempctrl=800;
  err2=err1;
  err1=err0;  
}
void pwm(int kaiguanbili)
{
//kaiguanbilis自增一次。直到kaiguanbilis=PWMResolution时候，将kaiguanbilis置零重新计数。
    
    if((kaiguanbili++) == PWMResolution) kaiguanbili = 0;
    
 
    for(int i = 0; i <= PWMResolution; i++) //i是计数一个PWM周期
    {
            if(i < kaiguanbili)
            {
                digitalWrite(pwmpin, HIGH);
                delayMicroseconds(2);
            }
            else
            {
                digitalWrite(pwmpin, LOW);
                delayMicroseconds(2);
            }
    }
}

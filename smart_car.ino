#include <IRremote.h>
#include <Metro.h> // Include Metro library
#define rightmotor 6//right motor pin
#define leftmotor 7//left motor pin
#define TrigPin 22  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define EchoPin 24  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define LED 13
#define RECV_PIN 26//irre recvie pin

uint8_t tempctrl,KP,KI,KD;
float cm;
uint8_t cmset;
long unsigned int ctrcode;
IRrecv irrecv(RECV_PIN);
decode_results results;// Create variables to hold the LED states
Metro IRre=Metro(100);
Metro Ultr=Metro(20);//ultrasonic state  

void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(leftmotor,OUTPUT);
  pinMode(rightmotor,OUTPUT);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();//init irr
}

void loop()
{

  if (IRre.check() == 1)
  { 
       

  }
  if (Ultr.check() == 1)
  { 
    static float cm0,cm1;
    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
    delayMicroseconds(2); 
    digitalWrite(TrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TrigPin, LOW); 
    cm0 = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm 
    cm0 = (int(cm * 100.0)) / 100.0; //保留两位小数 
    if(((cm1-cm0)>5)&&((cm1-cm0)<-5))
    {
      cm=cm1;
    }
    if(((cm1-cm0)<5)&&((cm1-cm0)>-5))
    {
      cm1=cm0;
      cm=cm0;
    }
    Serial.print(cm);
    Serial.println();
  } 
}

void PID()
{ 
  static signed int err0,err1,err2;// three value
  signed int add;  //The incremental
  err0=cmset-cm;
  add=KP*(err0-err1)+KI*err0+KD*(err0-2*err1+err2);
  tempctrl=tempctrl+add;
  if(tempctrl>130)tempctrl=130;
  if(tempctrl<120)tempctrl=120;
  err2=err1;
  err1=err0;  
}







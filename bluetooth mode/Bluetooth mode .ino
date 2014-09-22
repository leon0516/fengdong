#define up     0X7570  //ctrl  HEX code
#define down   0X776E
#define right  0X6874
#define left   0X6674

uint16_t value;      //some transist value
uint8_t  r_buffer[4];
uint8_t num=0;
uint8_t mark0=0;

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop()
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

          value=(r_buffer[0]<<8)|(r_buffer[1]);

        }

        num=0;
        mark0=1;
      } 
    }    



    if(mark0==1)
    {
      mark0=0;

      switch(value)
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
digitalWrite(13,1);
}

void _down()
{
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





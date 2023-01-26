#include <IRremote.h>
#define AI1 22
#define AI2 23
#define pwmA 16
#define BI1 19
#define BI2 21
#define pwmB 17
#define kairre90 36
#define kaire45 39
#define vidurys 34
#define desine45 35
#define desine90 32
#define klinija 15
#define dlinija 4 
#define startMod 27
#define lempute 25
#define dataFromStartModule 26

int code = 0; 
int pagrKonst = 2.1;

void drive(int speedA, int speedB);
void sensors(); 



void drive(int speedA, int speedB)
{
  if(digitalRead(StartModule))
  {
    if(speedA == 0 && speedB == 0)
    {
      analogWrite(pwmA, 255);
      analogWrite(pwmB, 255);
      digitalWrite(AI1, HIGH);
      digitalWrite(AI2, HIGH);
      digitalWrite(BI1, HIGH);
      digitalWrite(BI2, HIGH);
    }
    else
    {
      if(speedA>0)
      {
        if(speedA>255) speedA=255;
        analogWrite(pwmA, speedA);
        digitalWrite(AI1, HIGH);
        digitalWrite(AI2, LOW);
      }
      else
      { 
        analogWrite(pwmA, -speedA);
        digitalWrite(AI1, LOW);
        digitalWrite(AI2, HIGH);
      }
      if(speedB>0)
      {
        if(speedB>255) speedB=255;
        analogWrite(pwmB, speedB);
        digitalWrite(BI1, HIGH);
        digitalWrite(BI2, LOW);
      }
      else
      { 
        analogWrite(pwmB, -speedB);
        digitalWrite(BI1, LOW);
        digitalWrite(BI2, HIGH);
      }
    }
  }
  else
  {
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);

  }
}

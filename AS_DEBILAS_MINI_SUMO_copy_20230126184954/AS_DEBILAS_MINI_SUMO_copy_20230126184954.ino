#include <IRremote.h>
#define AI1 22
#define AI2 23
#define pwmA 16
#define BI1 19
#define BI2 21
#define pwmB 17
#define kaire90 36
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
int atstumasKaire90;
int atstumasKaire45;
int atstumasVidurys;
int atstumasDesine45;
int atstumasDesine90;
int a, b, c, d, e;
double pagrKonst = 2.1;
double milk;
double speed;
int greitis = 60;
int reverseGreitis = -120;
int turnBackTime = 100;
int posukioGreitis = 100;



void drive(int speedA, int speedB);
void sensors(); 

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(dataFromStartModule);
  pinMode(klinija, INPUT);
  pinMode(dlinija, INPUT);
  pinMode(startMod, INPUT);
  pinMode(dataFromStartModule, INPUT);

  pinMode(kaire90, INPUT);
  pinMode(kaire45, INPUT);
  pinMode(vidurys, INPUT);
  pinMode(desine45, INPUT);
  pinMode(desine90, INPUT);
  
  pinMode(pwmA, OUTPUT);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);

  pinMode(pwmB, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);
  pinMode(lempute, OUTPUT);

  while(code==0){
    IrReceiver.resume();
    if(IrReceiver.decode()){
      if(IrReceiver.decodedIRData.protocol == NEC){
        code = (IrReceiver.decodedIRData.command);
        break;
      }
    }
  }
  digitalWrite(lempute, HIGH);
  Serial.println(code);
  while(digitalRead(startMod)==LOW){
    if(digitalRead(startMod)==HIGH){
      break;
    }
  }
    if(code == 69){  //A
    sensors();
    drive(60,60);
    delay(100);
    drive(0,0);
    delay(100);
    }
    if(code == 24){   //2
    sensors();
    drive(50,50);      
    delay(50);
    drive(-50,50);
    delay(100);
    drive(0,0);
    delay(50);
    drive(90,-90);
    delay(150);
    drive(70,70);
    delay(200);    
  }
}


void sensors(){
  a = pulseIn(kaire90, HIGH);
  atstumasKaire90= ((a-1000)/10*20)-20;
  if(atstumasKaire90<0){ atstumasKaire90 = 0; }

  b = pulseIn(kaire45, HIGH);
  atstumasKaire90 = (b-1000)/10*20;
  if(atstumasKaire45<0){ atstumasKaire45 = 0; }

  c = pulseIn(vidurys, HIGH);
  atstumasVidurys = (c-1000)/10*20;
  if(atstumasVidurys<0){ atstumasVidurys = 0; }

  d = pulseIn(desine45, HIGH);
  atstumasDesine45 = (d-1000)/10*20;
  if(atstumasDesine45<0){ atstumasDesine45 = 0; }
  
  e = pulseIn(desine90, HIGH);
  atstumasDesine90 = ((e-1000)/10*20)-20;
  if(atstumasDesine90<0){ atstumasDesine90 = 0; }

  digitalRead(klinija);
  digitalRead(dlinija);
  Serial.print(atstumasKaire90);
  Serial.print(" ");
  Serial.print(atstumasKaire45);
  Serial.print(" ");
  Serial.print(atstumasVidurys);
  Serial.print(" ");
  Serial.print(atstumasDesine45);
  Serial.print(" ");
  Serial.print(atstumasDesine90);
  Serial.print(" ");
  Serial.print(klinija);
  Serial.print(" ");
  Serial.println(dlinija);
}

void drive(int speedA, int speedB){
  if(digitalRead(startMod))
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

void loop(){
  while(atstumasVidurys<150){
    milk = millis();
    speed = (millis()-milk)*pagrKonst;
    drive(speed,speed);}
  while(atstumasKaire45<80){
    drive(40,-40);
  }
}


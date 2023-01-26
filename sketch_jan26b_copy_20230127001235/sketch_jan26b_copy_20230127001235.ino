#include <IRremote.h>
#define AI1 23
#define AI2 22
#define pwmA 17
#define BI1 19
#define BI2 21
#define kaire90 36
#define kaire45 39
#define vidurys 34
#define desine45 35
#define desine90 32
#define startModule 27
#define IRkodopin 26
#define lempute 25 
int kodas = 0;
int a, b, c, d, e;
int atstumasKstatu, atstumasKkampu, atstumasVidurys, atstumasDkampu, atstumasDstatu;
double pagrKonst = 2.1;
double milk;
double greitisPagr;

void pasol(int speedA, int speedB);
void sensors();

void setup() {
  Serial.begin(115200);
  Serial.print(atstumasKstatu);
  Serial.print(" ");
  Serial.print(atstumasKkampu);
  Serial.print(" ");
  Serial.print(atstumasVidurys);
  Serial.print(" ");
  Serial.print(atstumasDkampu);
  Serial.print(" ");
  Serial.println(atstumasDstatu);
  IrReceiver.begin(IRkodopin);
  pinMode(startModule, INPUT);
  pinMode(kaire90, INPUT);
  pinMode(kaire45, INPUT);
  pinMode(vidurys, INPUT);
  pinMode(desine45, INPUT);
  pinMode(desine90, INPUT);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(lempute, OUTPUT);
  while(kodas==0){
    IrReceiver.resume();
    if(IrReceiver.decode()){
      if(IrReceiver.decodedIRData.protocol == NEC){
        code = (IrReceiver.decodedIRData.command);
        break;
      }
    }
  }
  digitalWrite(lempute, HIGH);
  Serial.println(kodas);
  while(digitalRead(startModule)==LOW){
    if(digitalRead(startModule)==HIGH){
      break;
    }
  }
  if(kodas == 69){
    sensors();
    paslo(60,60);
    delay(100);
    paslo(0,0);
    delay(100);

  }
  if(kodas == 70){
    sensors();
    paslo(-50,50);
    delay(70);
    paslo(60,60);
    delay(200);
    paslo(50,-50);
    delay(100);
  }
}

void paslo(int speedA, int speedB){
  if(digitalRead(startModule)){
    if(speedA == 0 && speedB == 0){
      analogWrite(pwmA, 255); //stalina
      analogWrite(pwmB, 255);
      digitalWrite(AI1, HIGH);
      digitalWrite(AI2, HIGH);
      digitalWrite(BI1, HIGH);
      digitalWrite(BI2, HIGH);
    }
    else {
      if(speedA>0){
        if(speedA>255) speedA=255;
        analogWrite(pwmA, speedA);
        digitalWrite(AI1, HIGH);
        digitalWrite(AI2, LOW);
      }
    }
  }
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
    //if star module LOW shoot off motors
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);

  }
}

void sensors(){
  a = pulseIn(kaire90, HIGH);
  atstumasKstatu = (a-1000)/10*20;
  if(atstumasKstatu<0){ atstumasKstatu = 0; }
  b = pulseIn(kaire45, HIGH);
  atstumasKkampu = (a-1000)/10*20;
  if(atstumasKkampu<0){ atstumasKkampu = 0; }
  c = pulseIn(vidurys, HIGH);
  atstumasVidurys = (a-1000)/10*20;
  if(atstumasVidurys<0){ atstumasVidurys = 0; }
  d = pulseIn(desine45, HIGH);
  atstumasDkampu = (a-1000)/10*20;
  if(atstumasDkampu<0){ atstumasDkampu = 0; }
  e = pulseIn(desine90, HIGH);
  atstumasDstatu = (a-1000)/10*20;
  if(atstumasDstatu<0){ atstumasDstatu = 0; }
}

void loop() {
  sensors();
  if(atstumasVidurys<120){
    sensors();
    pasol(110,110);}
  else{
    sensors();
    if(atstumasKkampu<150){
      sensors();
      pasol(-50,50);}
      else{
        sensors();
        if(atstumasDkampu<150){
          sensors();
          pasol(50,-50);}
          else{
            sensors();
            if(atstumasDstatu<150){
            sensors();
            pasol(50,-50);}
            else{
              sensors();
              if(atstumasKstatu<150){
                pasol(-50,50);
              }
            }

            
          }
        }
  }
}


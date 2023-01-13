#include <IrRemote.h>

//sensoriai
#define KaireStatu 36
#define KaireKampu 39
#define Vidurys 34
#define DesineKampu 35
#define DesineStatu 32
int atstumasKaireStatu;
int atstumasKaireKampu;
int atstumasVidurys;
int atstumasDesineKampu;
int atstumasDesineStatu;
int a, b, c, d, e;

//start module
#define DataFromStartModule 26
#define StartModule 27
#define Lempute 25

//motorai
#define AI1 23
#define AI2 22
#define BI1 21
#define BI2 19
#define AI1 23
#define pwmA 17
#define pwmB 16

//linijos
#define leftline 4
#define rightline 18
int klinija;
int dlinija;


//config
int greitis = 60; // defaultinis greitis
int reverseGreitis = -150; // vaziuot atgal greitis
int turnBackTime = 200; // milisekundemis
int posukioGreitis = 100;
int sukimosiLaikas = 100; // milisekundemis

//funkcijos
void drive(int speedA, int speedB);
void sensors();


void drive(int speedA, int speedB)//speed from -255 to 255; if both speeds = to 0 - stall
{
  if(digitalRead(StartModule))
  {
    if(speedA == 0 && speedB == 0)
    {
      analogWrite(pwmA, 255);//motor A speed
      analogWrite(pwmB, 255);//motor B speed
      digitalWrite(AI1, HIGH);//front pin
      digitalWrite(AI2, HIGH);//back pin
      digitalWrite(BI1, HIGH);//front pin
      digitalWrite(BI2, HIGH);//back pin
    }
    else
    {
      if(speedA>0)
      {
        analogWrite(pwmA, speedA*speed_Correct); // KAZKURIAM REIK UZDET SPEED CORRECT
        digitalWrite(AI1, HIGH);
        digitalWrite(AI2, LOW);
      }
      else
      { 
        analogWrite(pwmA, -speedA*speed_Correct);
        digitalWrite(AI1, LOW);
        digitalWrite(AI2, HIGH);
      }
      if(speedB>0)
      {
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


void sensors()
{
  a = pulseIn(KaireStatu, HIGH);
  atstumasKaireStatu = (x-1000)*2;
  if(atstumasKaireStatu<0){ atstumasKaireStatu = 0; }

  b = pulseIn(KaireKampu, HIGH);
  atstumasKaireKampu = (y-1000)*2;
  if(atstumasKaireKampu<0){ atstumasKaireKampu = 0; }

  c = pulseIn(Vidurys, HIGH);
  atstumasVidurys = (y-1000)*2;
  if(atstumasVidurys<0){ atstumasVidurys = 0; }

  d = pulseIn(DesineKampu, HIGH);
  atstumasDesineKampu = (y-1000)*2;
  if(atstumasDesineKampu<0){ atstumasDesineKampu = 0; }
  
  e = pulseIn(DesineStatu, HIGH);
  atstumasDesineStatu = (z-1000)*2;
  if(atstumasDesineStatu<0){ atstumasDesineStatu = 0; }

  klinija = digitalRead(leftLineSensor);
  dlinija = digitalRead(rightLineSensor);
}

void setup() {
  IrReceiver.begin(dataFromStartModule);
  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
  pinMode(StartModule, INPUT);
  //motorai
  pinMode(pwmA, OUTPUT);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);

  pinMode(pwmB, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);

  while(code==0){
    IrReceiver.resume();
    if(IrReceiver.decode()){
      if(IrReceiver.decodedIRData.protocol == NEC){
        code = (IrReceiver.decodedIRData.command);
        break;
      }
    }
  }
  while(digitalRead(StartModule)==LOW){
    if(digitalRead(StartModule)==HIGH){
      break;
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:

}

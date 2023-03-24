#define DIST_0_PIN 22// 90 left
#define DIST_1_PIN 21// sligt left
#define DIST_2_PIN 20// front
#define DIST_3_PIN 19// slight right
#define DIST_4_PIN 18// 90 right


#define LINE_0_PIN 6//left
#define LINE_1_PIN 5//right

#define AIN_F_PIN 8//left motor 8
#define AIN_B_PIN 10
#define BIN_F_PIN 15// right motor
#define BIN_B_PIN 13 //13

#define START_PIN 16
#define IR_PIN 17


#define DEBUG // coment if not needed
//#define DELAY // UNCOMENT IF NEED 5s DELAY
//#define TURN180 

#ifdef DELAY
  #define START_DELAY 5000
#endif

#ifdef DEBUG
  #define BAUD 115200
#endif

// --------------------------tuning parameters------------------------------------------
#define SEARCH_SPEED 60 //maximum speed to manage to not fly out of the ring
#define ATTACK_SPEED 100 //maximum speed without vienkes
#define FULL_SPEED 200 // 255 FW taktika
#define DRIG_SPEED 255 // nu aiskiau paaiskint nei vardas negaliu
#define ROT_SPEED  60 //sukimosi paieskos greitis

#define MAX_SPEED_DISTANCE 100
#define FRONT_OVERRIDE_DISTANCE 300

#define DRIG_TIME 50
#define DRIG_INTERVAL 750
#define LINE_SEE_ROT_TIME 200
#define FULL_ROT_TIME 400
#define MID_TRACK_TIME 500
#define FULL_TURN_DELAY 100 //ms to turn 180 deg



const int lineTreshold = 500;



// absolutelly needed variables DO NOT FUCIKING TOUCH 
// if not know what means ofc
#define DIST_TIMEOUT 25000
#define MAX_RANGE 700 // mm 
#define APROX_SEE 100// maximum diference between sensors to see
int dist[5];
bool see[5];
bool timeout[5];
int a = 1; // Drig - 1; Lankos - 2
int b = 1; // other IR option

bool line[2]; 
int tactic = 0; // 0 is default tactic
bool attack = false;
bool notInProgress = true;
bool inAttack = false;

// functions 
void drive(int spdA, int spdB);
void sensors();
void driger(int Mode);
void lankos(int mode);
void attacker();
void lineAvoid(bool left = false, bool right = false);
void rot180();

void setup() 
{

  pinMode(AIN_F_PIN, OUTPUT);
  pinMode(AIN_B_PIN, OUTPUT);
  pinMode(BIN_F_PIN, OUTPUT);
  pinMode(BIN_B_PIN, OUTPUT);

  pinMode(DIST_0_PIN, INPUT);
  pinMode(DIST_1_PIN, INPUT);
  pinMode(DIST_2_PIN, INPUT);
  pinMode(DIST_3_PIN, INPUT);
  pinMode(DIST_4_PIN, INPUT);

  pinMode(LINE_0_PIN, INPUT);
  pinMode(LINE_1_PIN, INPUT);

  pinMode(START_PIN, INPUT);

  #ifdef DEBUG
    byte errorCode = diagnostics();
    Serial.begin(BAUD);
    if(errorCode == 0)
    {
      Serial.println("INIT complete");
      Serial.println("No errors found");
    }
    else
    {
      Serial.println("INIT complete");
      Serial.print("Error code: ");
      Serial.println(errorCode);
    }
  #endif

  while(!digitalRead(START_PIN))
  {
    #ifdef DEBUG
      Serial.println("WAITING FOR START");
    #endif
    delay(1);
  }

  #ifdef DELAY
    drive(0, 0);
    delay(START_DELAY);
  #endif

  #ifdef TURN180
    rot180();
  #endif
  
}

void loop() {
  sensors();
  if(a == 1)
  {
      driger(1);
  }
  else
  {
      lankos(1);
  }
}

int diagnostics()
{
  byte errorCod = 0;
  return errorCod;
}

void drive(int spdA, int spdB)
{
  //constrains
  #ifdef DEBUG
    Serial.print(spdA);
    Serial.print(" ");
    Serial.println(spdB);
  #endif
  if(digitalRead(START_PIN))
  {
    if(spdA == 0 && spdB == 0)
    {
      digitalWrite(AIN_F_PIN, HIGH);
      digitalWrite(AIN_B_PIN, HIGH);
      digitalWrite(BIN_F_PIN, HIGH);
      digitalWrite(BIN_B_PIN, HIGH);
    }
    else
    {
      if(spdA > 0)// motor A drive
      {
        analogWrite(AIN_F_PIN, spdA);
        digitalWrite(AIN_B_PIN, LOW);
      }
      else
      {
        digitalWrite(AIN_F_PIN, LOW);
        analogWrite(AIN_B_PIN, -spdA);
      }

      if(spdB > 0) // motor B drive
      {
        analogWrite(BIN_F_PIN, spdB);
        digitalWrite(BIN_B_PIN, LOW);
      }
      else
      {
        digitalWrite(BIN_F_PIN, LOW);
        analogWrite(BIN_B_PIN, -spdB);
      }
      
    }
  }
  else //stop
  {
    digitalWrite(AIN_F_PIN, LOW);
    digitalWrite(AIN_B_PIN, LOW);
    digitalWrite(BIN_F_PIN, LOW);
    digitalWrite(BIN_B_PIN, LOW);
  } 
}

void sensors()
{
  line[0] = false;
  line[1] = false;
  if(!digitalRead(LINE_0_PIN))
    line[0] = true;
  if(!digitalRead(LINE_1_PIN))
    line[1] = true;

  if(line[0] || line[1])
  {
    lineAvoid(line[0], line[1]);
  }

  // distance calculatipon should be using interups in the future

  timeout[0] = false;
  timeout[1] = false;
  timeout[2] = false;
  timeout[3] = false;
  timeout[4] = false;
  int t0 = pulseIn(DIST_0_PIN, HIGH, DIST_TIMEOUT);
  if(t0 != 0 && t0 < 2000)
  {
    dist[0] = (t0 - 1000) * 3 / 4;
  }
  else 
    timeout[0]= true;

  int t1 = pulseIn(DIST_1_PIN, HIGH, DIST_TIMEOUT);
  if(t1 != 0 && t1 < 2000)
  {
    dist[1] = (t1 - 1000) * 3 / 4;
  }
  else 
    timeout[1]= true;

  int t2 = pulseIn(DIST_2_PIN, HIGH, DIST_TIMEOUT);
  if(t2 != 0 && t2 < 2000)
  {
    dist[2] = (t2 - 1000) * 3 / 4;
  }
  else 
    timeout[2]= true;

  int t3 = pulseIn(DIST_3_PIN, HIGH, DIST_TIMEOUT);
  if(t3 != 0 && t3 < 2000)
  {
    dist[3] = (t3 - 1000) * 3 / 4;
  }
  else 
    timeout[3]= true;

  int t4 = pulseIn(DIST_4_PIN, HIGH, DIST_TIMEOUT);
  if(t4 != 0 && t4 < 2000)
  {
    dist[4] = (t4 - 1000) * 3 / 4;
  }
  else 
    timeout[4]= true;

  //find min in range
  int min = MAX_RANGE+1;
  int minSens = 5; // which sensor saw
  attack = false;
  for(int i = 0; i < 5; i++)
  {
    if(dist[i] < min && !timeout[i])
    {
      minSens = i;
      min = dist[i];
      attack = true;
    }
  }

  see[4]=false;
  see[3]=false;
  see[2]=false;
  see[1]=false;
  see[0]=false;

  if(min < MAX_RANGE+1)
  {
    see[minSens] = true;

    if(minSens == 0)
    {
      if(dist[0]-APROX_SEE < dist[1] && dist[1] < dist[0]+APROX_SEE)
        see[1]=true;
    }
    else
    {
      if(minSens == 4)
      {
        if(dist[4]-APROX_SEE < dist[3] && dist[3] < dist[4]+APROX_SEE)
          see[3]=true;
      }
      else
      {

        if(dist[minSens]-APROX_SEE < dist[minSens+1] && dist[minSens+1] < dist[minSens]+APROX_SEE && !timeout[minSens+1])
          see[minSens+1]=true;

        if(dist[minSens]-APROX_SEE < dist[minSens-1] && dist[minSens-1] < dist[minSens]+APROX_SEE && !timeout[minSens-1])
          see[minSens-1]=true;

      }
    }
  }
  for(int i = 0; i < 5; i++)
  {
    if(timeout[i])
      see[i] = false;
  }

  if(!see[0] && !see[1] && !see[2] && !see[3] && !see[4])
  {
    attack = false;
    inAttack = false;
  }  
  
  if(attack && !inAttack)
  {
    attacker();
  }

  #ifdef DEBUG
    Serial.print(dist[0]);
    Serial.print(" ");
    Serial.print(dist[1]);
    Serial.print(" ");
    Serial.print(dist[2]);
    Serial.print(" ");
    Serial.print(dist[3]);
    Serial.print(" ");
    Serial.print(dist[4]);
    Serial.println();
    Serial.print(see[0]);
    Serial.print("  ");
    Serial.print(see[1]);
    Serial.print("  ");
    Serial.print(see[2]);
    Serial.print("   ");
    Serial.print(see[3]);
    Serial.print("   ");
    Serial.print(see[4]);
    Serial.println();
  #endif
}

void lineAvoid(bool left, bool right)
{
  if(left && right)
  {
    drive(-SEARCH_SPEED, -SEARCH_SPEED);
    delay(LINE_SEE_ROT_TIME);
    rot180();
  }
  if(left)
  {
    drive(-SEARCH_SPEED, -SEARCH_SPEED);
    delay(LINE_SEE_ROT_TIME);
    drive(-ROT_SPEED, ROT_SPEED);
  }
  if(right)
  {
    drive(-SEARCH_SPEED, -SEARCH_SPEED);
    delay(LINE_SEE_ROT_TIME);
    drive(ROT_SPEED, -ROT_SPEED);
  }

}

void rot180()
{
  int SpdA = ROT_SPEED, SpdB = -ROT_SPEED;
  while(SpdB <= ROT_SPEED)
  {
    SpdB++;
    drive(SpdA, SpdB);
    delay(3);
  }
}

void lankos(int mode = 0)
{
  if(mode == 1 && notInProgress)// mid speed
  {
    notInProgress = false;
    drive(FULL_SPEED, FULL_SPEED);
    delay(MID_TRACK_TIME);
  }
  if(mode == 2 && notInProgress) // full speed
  {
    notInProgress = false;
    drive(ATTACK_SPEED, ATTACK_SPEED);
    delay(MID_TRACK_TIME);
  }
  while(1)
  {
    sensors();
    drive(SEARCH_SPEED, SEARCH_SPEED);
    #ifdef DEBUG
      Serial.println("---SEARCHING---");
    #endif
  }
}

void driger(int Mode = 1)
{
  sensors();
  if(notInProgress)
  {
    notInProgress = false;
    long milk_1 = millis();
    while(!attack)
    {
      sensors();
      if((millis()-milk_1)%DRIG_INTERVAL == 0)
      {
        
        long milk_2 = millis();
        while(millis()-milk_2 < DRIG_TIME)
        {
          sensors();
          drive(DRIG_SPEED, DRIG_SPEED);
        }
      }
      drive(0, 0);
    }
  }
  if(Mode == 1) // rotate
  {
    drive(ROT_SPEED, -ROT_SPEED);
  }
  if(Mode == 2) // skip to lankos
  {
    drive(SEARCH_SPEED, SEARCH_SPEED);
  }
  if(Mode == 3) // wait 
  {
    drive(0, 0);
  }
}

void attacker()
{ 
  inAttack = true;
  int state = 0;
  while(attack)
  {
    sensors();
    #ifdef DEBUG
      Serial.print("ATTACK state - ");
      Serial.println(state);
    #endif
    if(see[0])
      state = 4;

    if(see[1])
      state = 2;

    if(see[2])
      state = 0;

    if(see[3])
      state = -2;

    if(see[4])
      state = -4;

    if(see[0] && see[1])
      state = 3;

    if(see[1] && see[2])
      state = 1;

    if(see[3] && see[2])
      state = -1;

    if(see[4] && see[3])
      state = -3;

    if(see[0] && see[1] && see[2])
      state = 2;

    if(see[3] && see[2] && see[1])
      state = 0;

    if(see[4] && see[3] && see[2])
      state = -2;
    int state0spd = ATTACK_SPEED;
    if(dist[2] < FRONT_OVERRIDE_DISTANCE)
    {
      state = 0;
      if(dist[2] < MAX_SPEED_DISTANCE)
      {
        state0spd = 255;
      }
      else
      {
        state0spd = map(dist[2], FRONT_OVERRIDE_DISTANCE, MAX_SPEED_DISTANCE, ATTACK_SPEED, FULL_SPEED);
      }
    }
    
    switch (state) {
      case 4:
        drive(-ATTACK_SPEED/2, ATTACK_SPEED/2);
        break;
      
      case 3:
        drive(-(ATTACK_SPEED/4), ATTACK_SPEED/2);
        break;

      case 2:
        drive(0, ATTACK_SPEED/2);
        break;

      case 1:
        drive(ATTACK_SPEED/4, ATTACK_SPEED/2);
        break;

      case 0:
        drive(ATTACK_SPEED, ATTACK_SPEED);
        break;

      case -1:
        drive(ATTACK_SPEED/2, ATTACK_SPEED/4);
        break;

      case -2:
        drive(ATTACK_SPEED/2, 0);
        break;
        
      case -3:
        drive(ATTACK_SPEED/2, -(ATTACK_SPEED/4));
        break;

      case -4:
        drive(ATTACK_SPEED/2, -ATTACK_SPEED/2);
        break;

      default:
        drive(ATTACK_SPEED-20, ATTACK_SPEED-20);
        break;
      
    }
  }
}
























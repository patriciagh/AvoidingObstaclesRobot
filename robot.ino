#include <Servo.h>
#include <NewPing.h>

// Pins of motor 1 - STANGA
#define mpin00 5 // INAINTE
#define mpin01 6 // INAPOI
// Pins of motor 2 - DREAPTA
#define mpin10 3 // INAINTE
#define mpin11 11 // INAPOI

// ocolire obstacole - pini

#define trigger_pin  9
#define echo_pin  10
#define distanta_maxima 250

Servo srv;

NewPing sonar(trigger_pin, echo_pin, distanta_maxima);

int distanta=10;
int led=2;
void setup() 
{
  // setup motoare
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT);
 // pin LED

  digitalWrite(led,LOW);
 pinMode(led,  OUTPUT); 

  Serial.begin(115200);
  //ocolire obstacole
  srv.attach(8);
  // setez - orientat la 90 de grade
  srv.write(90);
  delay(200);
  distanta = citireDistanta();
  delay(200);
  
}

void StartMotor (int m1, int m2, int forward, int speed)
{

   if (speed==0) // oprire
   {
    digitalWrite(m1, 0); 
    digitalWrite(m2, 0);
   }
   else
   {
     if (forward)
     {
       digitalWrite(m2, 0);
       analogWrite (m1, speed); // folosire PWM
     }
     else
     {
       digitalWrite(m1, 0);
       analogWrite(m2, speed);
     }
   }
}
// Functie de siguranta
// Executa oprire motoare, urmata de delay
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(ms);
}

int citireDistanta()
{
  delay(100);
  int distanta_cm=sonar.ping_cm();
  if(distanta_cm==0)
    distanta_cm = distanta_maxima;
  return distanta_cm; 
}

int distantaDreapta()
{
  srv.write(0);
  delay(500);
  int distanta_dreapta=citireDistanta();
  delay(100);
  srv.write(90);
  return distanta_dreapta;
}

int distantaStanga()
{
  srv.write(180);
  delay(500);
  int distanta_stanga=citireDistanta();
  delay(100);
  srv.write(90);
  return distanta_stanga;
}


void merge_stop()
{
  digitalWrite(mpin00,0);
  digitalWrite(mpin01,0);
  digitalWrite(mpin10,0);
  digitalWrite(mpin11,0);
   //pin de stop
 digitalWrite(led, HIGH); 
 delay(1000);
 digitalWrite(led,LOW);
}
void merge_inainte(int viteza)
{
  StartMotor (mpin00, mpin01, 1, viteza);
  StartMotor (mpin10, mpin11, 1, viteza);  
}

void merge_spate(int viteza)
{
   StartMotor (mpin00, mpin01, 0, viteza);
   StartMotor (mpin10, mpin11, 0, viteza); 
}

void merge_stanga(int viteza)
{
  StartMotor (mpin00, mpin01, 0, viteza);//stanga
  StartMotor (mpin10, mpin11, 1, viteza); //dreapta
}

void merge_dreapta(int viteza)
{
  StartMotor (mpin00, mpin01, 1, viteza); 
  StartMotor (mpin10, mpin11, 0, viteza); 
}
void loop()
{
  int viteza=128;
  
  int distanta_dreapta=0;
  int distanta_stanga=0;
  
  if(distanta <=30)
  {
      Serial.println("STOP");
      merge_stop();
      delay(300);
      
      distanta_dreapta=distantaDreapta();
      delay(300);
      distanta_stanga=distantaStanga();
      delay(300);
      
      if(distanta_dreapta > distanta_stanga)
      {
        // distanta din dreapta e mai buna -> merge in dreapta
        merge_dreapta(viteza);
        delay(300);
        merge_stop();       
        Serial.println("DREAPTA");
      }
      else
      {
        merge_stanga(viteza);
        delay(300);
        merge_stop();
        Serial.println("STANGA");
      }    
    }
    else
    {
      merge_inainte(viteza);
      delay(500);
    }
    distanta=citireDistanta();
    
}

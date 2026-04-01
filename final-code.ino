#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#define TRIG_PIN 12
#define ECHO_PIN 13
#include <SoftwareSerial.h>

// Bluetooth HC-06
SoftwareSerial BTSerial(2, 3);  // RX, TX

volatile int  Valeur_Capteur_Gauche =1, Valeur_Capteur_Centre = 1 ,Valeur_Capteur_Droit =1; 
volatile int Noir = 1, blanc = 0, Front_Distance = 0;
volatile char boutton = 'C';
int EN1=11;
int IN1=10; 
int IN2=9;
int IN3=8;
int IN4=7;
int EN2=6;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);      // make sure your Serial Monitor is also set at this baud rate.
  BTSerial.begin(9600);
  Dabble.begin(BTSerial);      //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
  pinMode(A0, INPUT); pinMode(4, INPUT); pinMode(5, INPUT);
  pinMode(EN1, OUTPUT); pinMode(EN2, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);pinMode(IN3, OUTPUT);pinMode(IN4, OUTPUT);
  pinMode(12, OUTPUT); pinMode(13, INPUT);
 }

void Rotation_Gauche(int vitesse){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(EN1, vitesse);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN2, vitesse);
  }
  
void Rotation_Droite(int vitesse){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(EN1, vitesse);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(EN2, vitesse);
  }
 
void Marche_Arriere(int vitesse){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(EN1, vitesse);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN2, vitesse);
  }
  
void Marche_Avant(int vitesse){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(EN1, vitesse);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(EN2, vitesse);
  }

void Arret(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(EN1,0);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN2,0);
  }

void Suivi_Infrarouge() {
  Valeur_Capteur_Gauche = digitalRead(A0);
  Valeur_Capteur_Centre = digitalRead(4);
  Valeur_Capteur_Droit = digitalRead(5);
  if (Valeur_Capteur_Gauche != Noir && Valeur_Capteur_Centre == Noir && Valeur_Capteur_Droit != Noir) {
    Marche_Avant(120);  
  }
  else if (Valeur_Capteur_Gauche == Noir && Valeur_Capteur_Droit != Noir) {
    Rotation_Gauche(Valeur_Capteur_Gauche == Noir && Valeur_Capteur_Centre != Noir ? 120 : 80);
  }
  else if (Valeur_Capteur_Droit == Noir && Valeur_Capteur_Gauche != Noir) {
    Rotation_Droite(Valeur_Capteur_Centre == Noir ? 80 : 120);
  }
  else if ((Valeur_Capteur_Gauche == Noir && Valeur_Capteur_Centre == Noir && Valeur_Capteur_Droit == Noir) ) { 
    Rotation_Droite(40);
  }
  else if ((Valeur_Capteur_Gauche != Noir && Valeur_Capteur_Centre != Noir && Valeur_Capteur_Droit != Noir) ) { 
    Arret();
  }
}
/*void Line_et_Obstacle() {

  float distance = checkdistance();

  // ===== OBSTACLE =====
  if (distance <= 15) {

    Marche_Arriere(80);
    delay(200);

    if (random(1, 100) <= 50) {
      Rotation_Gauche(90);
    } else {
      Rotation_Droite(90);
    }
    delay(400);
  }

  // ===== suivre la LIGNE =====
     else {

    Valeur_Capteur_Gauche = digitalRead(A0);
    Valeur_Capteur_Centre = digitalRead(4);
    Valeur_Capteur_Droit = digitalRead(5);

    if (Valeur_Capteur_Gauche != Noir && Valeur_Capteur_Centre == Noir && Valeur_Capteur_Droit != Noir) {
      Marche_Avant(120);
    }
    else if (Valeur_Capteur_Gauche == Noir && Valeur_Capteur_Droit != Noir) {
      Rotation_Gauche(100);
    }
    else if (Valeur_Capteur_Droit == Noir && Valeur_Capteur_Gauche != Noir) {
      Rotation_Droite(100);
    }
    else if (Valeur_Capteur_Gauche == Noir && Valeur_Capteur_Centre == Noir && Valeur_Capteur_Droit == Noir) {
      Rotation_Droite(60);
  }
  else {
      Arret();
    }
  }
}
*/   
void Eviter_Obstacle_Intelligent() {

  // Reculer
  Marche_Arriere(90);
  delay(300);

  // Sortir de la ligne (droite)
  Rotation_Droite(100);
  delay(350);

  // Avancer en contournant
  while (checkdistance() <= 15) {
    Marche_Avant(100);
  }

  delay(200);

  // Revenir vers la ligne
  Rotation_Gauche(100);
  delay(350);
}
void Rechercher_Ligne() {

  while (1) {

    Valeur_Capteur_Gauche = digitalRead(A0);
    Valeur_Capteur_Centre = digitalRead(4);
    Valeur_Capteur_Droit = digitalRead(5);

    if (Valeur_Capteur_Centre == Noir) {
      break;   // Ligne trouvée
    }

    Marche_Avant(80);
  }

  Arret();
  delay(200);
}
void Line_et_Obstacle() {

  float distance = checkdistance();

  if (distance <= 15) {

    Eviter_Obstacle_Intelligent();
    Rechercher_Ligne();

  } 
  else {
    Suivi_Infrarouge();
  }
}   


float checkdistance() {
  digitalWrite(12, LOW); delayMicroseconds(2);
  digitalWrite(12, HIGH); delayMicroseconds(10);
  digitalWrite(12, LOW);
  float distance = pulseIn(13, HIGH) / 58.0;
  delay(10);
  return distance;
}

void Ultrasonic_Avoidance() {
  Front_Distance = checkdistance();
  if (Front_Distance <= 10) {
    Marche_Arriere(100); delay(200);
    random(1, 100) <= 50 ? Rotation_Gauche(100) : Rotation_Droite(100);
    delay(500);
  } else if (Front_Distance <= 20) {
    Arret(); delay(200);
    random(1, 100) <= 50 ? Rotation_Gauche(100) : Rotation_Droite(100);
    delay(500);
  } else {
    Marche_Avant(70);
  }
}

void Ultrasonic_Follow() {
  Front_Distance = checkdistance();
  if (Front_Distance <= 5) {
    Marche_Arriere(80);
  } else if (Front_Distance <= 10) {
    Arret();
  } else if (Front_Distance > 20) {
    Marche_Avant(100);
  }
}

void controle(){
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  Serial.print("KeyPressed: ");
  if (GamePad.isUpPressed())
  {
    boutton = 'U';
    Serial.print("UP");
  }

  if (GamePad.isDownPressed())
  {
    boutton = 'D';
    Serial.print("DOWN");
  }

  if (GamePad.isLeftPressed())
  {
    boutton = 'L';
    Serial.print("Left");
  }

  if (GamePad.isRightPressed())
  {
    boutton = 'R';
    Serial.print("Right");
  }

  if (GamePad.isSquarePressed())
  {
    boutton = 'S';
    Serial.print("Square");
  }

  if (GamePad.isCirclePressed())
  {
    boutton = 'C';
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed())
  {
    boutton = 'X';
    Serial.print("Cross");
  }

  if (GamePad.isTrianglePressed())
  {
    boutton = 'T';
    Serial.print("Triangle");
  }

  if (GamePad.isStartPressed())
  {
    Serial.print("Start");
    boutton='p';
  }

  if (GamePad.isSelectPressed())
  {
    Serial.print("Select");
  }
   Serial.print('\t');

   
  /*int a = GamePad.getAngle();
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');
  int b = GamePad.getRadius();
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');
  float c = GamePad.getXaxisData();
  Serial.print("x_axis: ");
  Serial.print(c);
  Serial.print('\t');
  float d = GamePad.getYaxisData();
  Serial.print("y_axis: ");
  Serial.println(d);
  Serial.println();*/
  
switch (boutton) {
      case 'U': Marche_Avant(110); delay(300); Arret(); boutton= ' '; break;
      case 'D': Marche_Arriere(110); delay(300); Arret();boutton= ' '; break;
      case 'L': Rotation_Gauche(110); delay(300); Arret();boutton= ' '; break;
      case 'R': Rotation_Droite(110); delay(300); Arret();boutton= ' '; break;
      case 'C': Arret();boutton= ' '; break;
    } 
}    
 void loop() {
  switch (boutton) {
    case 'S': Suivi_Infrarouge(); break;
    case 'T': Ultrasonic_Avoidance(); break;
    case 'X': Ultrasonic_Follow(); break;
    case 's': Arret(); boutton= ' ';break;
    case 'p': Eviter_Obstacle_Intelligent();break;
  }
 controle();
 
 }

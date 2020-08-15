#include <Arduino.h>
#include <DueTimer.h>

// Protos
void setBlinkerR();
void unsetBlinkerR();
void setBlinkerL();
void unsetBlinkerL();
void setFernicht();
void unsetFernicht();
void setFernichtLR();
void unsetFernichtLR();
void setFernichtM();
void unsetFernichtM();
void setTagfahrlicht(uint8_t intensity);
void setAbblendlicht();
void unsetAbblendlicht();

// Declare output pins
#define BLINKER_RECHFS_PIN 12
#define BLINKER_LINKS_PIN 11
#define FERNLICHT_PIN 10
#define FERNLICHT_L_R_PIN 9
#define FERNLICHT_M_PIN 8
#define TAGFARLICHT_PIN A0    // Used Analog pin due intensity
#define ABBLENDLICHT_PIN 7

// Timer interrupt interval
#define INTERRUPT_INTERVAL 100000  // uS

// States
uint8_t i_b_r = 0;  // BLINKER_RECHFS counter (1.1.1)
uint16_t i_g = 0;    // Global Counter (1.1.3)
boolean s_f = false;  // Indicate if  fernlicht is on or off
boolean s_f_l_r = false; // Indicate if fernlicht L & R is on or off
boolean s_f_m = false; // Indicate if fernlicht M is on or off

void timer2_interrupt(){
  Serial.println("Counter -> " + String(i_g));

  // Blinker rechfs and links (1.1.1) and (1.1.2
  switch(i_b_r){
    case 0:
      setBlinkerR();
      break;
    case 10:
      unsetBlinkerR();
      break;
    case 20:
      setBlinkerL();
      break;
    case 30:
      unsetBlinkerL();
      break;
    case 39:
      i_b_r = -1;
      break;
  }

  // Fernicht (1.1.3)
  switch (i_g){
    case 200:
      setFernicht();
      break;
    case 400:
      unsetFernicht();
      break;
  }
  if(i_g >= 520 && i_g < 580){
    if(!(i_g % 5)){
      if(s_f){
        unsetFernicht();
      } else {
        setFernicht();
      }
    }
  } else if(i_g >= 580 && i_g < 599){
    if(s_f){
      unsetFernicht();
    } else {
      setFernicht();
    }
  }

  // Ferblicht L & R (1.1.4)
  if(i_g == 200 || i_g == 260 || i_g == 340){
    setFernichtLR();
  } else if (i_g == 220 || i_g == 300 || i_g == 400){
    unsetFernichtLR();
  }
  if((i_g >= 220 && i_g < 260) ||
     (i_g >= 300 && i_g < 340)){
    if(s_f_l_r){
      unsetFernichtLR();
    } else {
      setFernichtLR();
    }
  }

  // Fernlicht M (1.1.5)
  if(i_g == 200 || i_g == 240 || i_g == 320){
    setFernichtM();
  } else if (i_g == 220 || i_g == 300 || i_g == 400){
    unsetFernichtM();
  }
  if((i_g >= 220 && i_g < 240) ||
     (i_g >= 300 && i_g < 320)){
    if(s_f_l_r){
      unsetFernichtM();
    } else {
      setFernichtM();
    }
  }

  // Tagfahrlicht (1.1.6) and abblendlicht (1.1.7)
  switch (i_g){
    case 0:
      setTagfahrlicht(26);  // 10% of intensity
      setAbblendlicht();
      break;
    case 180:
      setTagfahrlicht(255);
      unsetAbblendlicht();
      break;
    case 200:
      setTagfahrlicht(26); // 10% of intensity
      setAbblendlicht();
      break;
    case 400:
      setTagfahrlicht(255);
      unsetAbblendlicht();
      break;
    case 430:
      setTagfahrlicht(26); // 10% of intensity
      setAbblendlicht();
      break;
    case 460:
      setTagfahrlicht(255);
      unsetAbblendlicht();
      break;
  }

  if(i_g == 599){
    unsetFernicht();
    setTagfahrlicht(26); // 10% of intensity
    i_g = -1;
  }
  i_b_r++;
  i_g++;
}

void setup() {
  // Initialize Serial Port
  Serial.begin(115200);
  Serial.println("Starting Blinker Simulation");

  // Initialize Output pins
  pinMode(BLINKER_RECHFS_PIN, OUTPUT);
  pinMode(BLINKER_LINKS_PIN, OUTPUT);
  pinMode(FERNLICHT_PIN, OUTPUT);
  pinMode(FERNLICHT_L_R_PIN, OUTPUT);
  pinMode(FERNLICHT_M_PIN, OUTPUT);
  pinMode(TAGFARLICHT_PIN, OUTPUT);
  pinMode(ABBLENDLICHT_PIN,OUTPUT);

  Timer2.attachInterrupt(timer2_interrupt).start(INTERRUPT_INTERVAL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void setBlinkerR(){
  //Serial.println("Turn ON 1 -> " + String(i_b_r));
  digitalWrite(BLINKER_RECHFS_PIN, HIGH);
}

void unsetBlinkerR(){
  //Serial.println("Turn OFF 1 -> " + String(i_b_r));
  digitalWrite(BLINKER_RECHFS_PIN, LOW);
}

void setBlinkerL(){
  //Serial.println("Turn ON 2 -> " + String(i_b_r));
  digitalWrite(BLINKER_LINKS_PIN, HIGH);
}

void unsetBlinkerL(){
  //Serial.println("Turn OFF 2 -> " + String(i_b_r));
  digitalWrite(BLINKER_LINKS_PIN, LOW);
}

void setFernicht(){
  //Serial.println("Turn ON 3 -> " + String(i_g));
  digitalWrite(FERNLICHT_PIN, HIGH);
  s_f = true;
}

void unsetFernicht(){
  //Serial.println("Turn OFF 3 -> " + String(i_g));
  digitalWrite(FERNLICHT_PIN, LOW);
  s_f = false;
}

void setFernichtLR(){
  //Serial.println("Turn ON 4 -> " + String(i_g));
  digitalWrite(FERNLICHT_L_R_PIN, HIGH);
  s_f_l_r = true;
}

void unsetFernichtLR(){
  //Serial.println("Turn OFF 4 -> " + String(i_g));
  digitalWrite(FERNLICHT_L_R_PIN, LOW);
  s_f_l_r = false;
}

void setFernichtM(){
  //Serial.println("Turn ON 5 -> " + String(i_g));
  digitalWrite(FERNLICHT_M_PIN, HIGH);
  s_f_m = true;
}

void unsetFernichtM(){
  //Serial.println("Turn OFF 5 -> " + String(i_g));
  digitalWrite(FERNLICHT_M_PIN, LOW);
  s_f_m = false;
}

/**
 * Set Tagfahrlicht intensity
 *  uint8_t intensity - Intensity from 0 to 255
 */
void setTagfahrlicht(uint8_t intensity){
  analogWrite(TAGFARLICHT_PIN, intensity);
}

void setAbblendlicht(){
  //Serial.println("Turn ON 7 -> " + String(i_g));
  digitalWrite(ABBLENDLICHT_PIN, HIGH);
}

void unsetAbblendlicht(){
  //Serial.println("Turn OFF 7 -> " + String(i_g));
  digitalWrite(ABBLENDLICHT_PIN, LOW);
}
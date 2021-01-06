
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Adafruit_PWMServoDriver.h>

SoftwareSerial mySerial(10, 11);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

int Second ;
const int buttonPin1 = 4;
const int buttonPin2 = 3; 
const int buttonPin3 = 2;
const int buttonPinR = 5;
const int buttonPinG = 6;
int buttonState1,buttonState2,buttonState3,buttonStateR,buttonStateG = 0;
int Drugt ;
int W = 0; //สวิตเลือกยา
int SUM = 0;  //ใช้งานกับ servo
int YN = 0; //ต้วแปรเก็บ yes or no

void Green(){
  Drugt = SUM; //Drugt เก็บค่าของ SUM
  Wire.beginTransmission(9); //ส่งข้อมูลให้ตัวที่ Arduino ตัวที่ 1 ให้ตัวที่ 9
  Wire.write(Drugt); //ส่งข้อมูล Drugt ให้ตัวที่ 2
   Wire.endTransmission(); //การส่งข้อมูล
  SUM =0 ;
  Second = 0;
  Serial.println(SUM);
  mp3_play(5) ;//play 0005
  delay(1000);
  
}

void setup() { 
    
   Wire.begin();  
  Wire.begin(8); //ตัวที่ 8
   Wire.onReceive(receiveEvent); 
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
  pinMode(buttonPin1, INPUT);
   pinMode(buttonPin2, INPUT);
    pinMode(buttonPin3, INPUT);
     pinMode(buttonPinR, INPUT);
      pinMode(buttonPinG, INPUT);
       Serial.begin(9600);
       mySerial.begin (9600);
         mp3_set_serial (mySerial);
       mp3_set_volume(30);////////////////ปรับเสียง ลำโพง0-30/////////////////////
  
} 
void receiveEvent(int howMany){
  Second = Wire.read();
}
void loop() {
  
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonStateR = digitalRead(buttonPinR);
  buttonStateG = digitalRead(buttonPinG);

if(SUM == 0){
    pwm.setPWM(0, 0, pulseWidth(120));
     pwm.setPWM(1, 0, pulseWidth(120));
     pwm.setPWM(2, 0, pulseWidth(120));
}
 if(Second == 1){

  if(buttonState1 == LOW){ // 1
    mp3_stop (); 
    W = 1 ; 
    SUM=W;
    Serial.println(W);
     mp3_play (4); // 0004
   
      
  }if(buttonState2 == LOW){ // 2
     mp3_stop ();
     W = 2;
    SUM=W;
    Serial.println(W);
    mp3_play (2); // 0002
  }if(buttonState3 == LOW){ // 3
    mp3_stop (); 
    W = 3;
    SUM=W;
    Serial.println(W);
    mp3_play (3); //0003
  }
 
  if(buttonStateG== LOW){
      mp3_stop (); 
    switch (SUM){ //chack SUM
    case 1:
    Serial.println(SUM);
    pwm.setPWM(0, 0, pulseWidth(0));// servo 1 start
     Green();
    break;
    case 2 :
    Serial.println(SUM);
      pwm.setPWM(1, 0, pulseWidth(0));// servo 2 start
      Green();
     break;
    case 3:
      Serial.println(SUM);

      pwm.setPWM(2, 0, pulseWidth(0)); // servo 2 start
     Green();
      break;
    default:
    Serial.println(SUM);
}if (Drugt > 0){// Drugt have data
  mp3_stop();
  Serial.println("Successful");
}
  } 
if(buttonStateR == LOW){ // R
    mp3_pause();
    W = 0 ;
    mp3_play(6); // 0006
    Serial.println(W);
    Second = 0;
    }
  
}}
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

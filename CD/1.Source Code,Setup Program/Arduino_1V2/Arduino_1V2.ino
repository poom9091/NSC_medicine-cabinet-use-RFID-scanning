#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h> 
SoftwareSerial ArduinoSerial(3, 2); // RX, TX

#define SS_PIN 8
#define RST_PIN 9
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key;
File file;
const int limswitch = 6 ;
int buzzer = 5 ;
int  sum = 0;
int AA = 1 ;
int  tim  = 1 ; 
long duration,cm ;
int  relay1 = 4; 
int  Water  = 0; 
int  Second,BB,tine ; 
int  SC = 10 ; 
long x,y,Hour,Minute,Time,Day,Month,Year ;
 String z,in_rfid ;
  int sd_sum,ER,led,Drugt,Use,Sum ; 
  char in_id[20],in_use[20];
  int LED = 7;
 float  allwater = 0 ;
 //star READ SD CARD
bool readLine(File &f, char* line, size_t maxLen) {
  for (size_t n = 0; n < maxLen; n++) {
    int c = f.read();
    if ( c < 0 && n == 0) return false;  // EOF
    if (c < 0 || c == '\n') {
      line[n] = 0;
      return true;
    }
    line[n] = c;
  }
  return false; // line too long
}

bool readVals(long* v1,long* v2,  String* v3) {
  char line[200], *ptr, *str;
  if (!readLine(file, line, sizeof(line))) {
    return false;  // EOF or too long
  }
  *v1 = strtol(line, &ptr, 10);
  if (ptr == line) return false;  // bad number if equal
   while (*ptr) {
    if (*ptr++ == ',') break;
  }
   *v2 = strtol(ptr, &str, 10);
   while (*ptr) {
    if (*ptr++ == ',') break;
  }
  String a = strtok_r(ptr, ",", &str);
  String first(a);
  *v3 = first;
  return str != ptr;  // true if number found
}
// END READ
// Start Water
void water(){
  digitalWrite(relay1,1);
  int switch1 = digitalRead(limswitch); 
  if (BB < 3 ) {
    
  if (switch1 == LOW) {
   
    sum ++ ;
    delay (900);
     Serial.println(sum);
     if (tine != 0){
      BB += 1 ;
       Serial.println(BB);
       
       tine = 0 ;
       delay (2000);
     }
    if (sum >= 15){
      BB = 4 ;
      AA = 5;
      Serial.println("Bye");
    }
  }
  if (switch1 == HIGH){
    sum = 0 ;  
  
    tine ++ ;
    digitalWrite(relay1,0);
    delay (900);
     Serial.print(tine);
    if (tine >= 7){
      allwater += 0.125 ;
       Serial.print(allwater);
       BB += 1 ;
        Serial.println(BB);
        digitalWrite(relay1,1);
       tine = 0; 
       delay (4000);
    }
  }
}if(BB >= 3){

      AA = 5;
    }
  }
void sdcard(){
  Second = 0 ;
  file = SD.open("USE.txt",FILE_READ); //read file USE
  if (!file){
    Serial.println(F("open error"));
    return ;
  }
  while (readVals(&x,&y,&z)) { 

   Serial.print(F("Order :"));
   Serial.println(x);
     Serial.print(F("ID :"));
   Serial.println(y);
   Use = y ;
   Serial.print(F("RFID :"));
   Serial.print(z);
   Serial.println(F(""));  
   Serial.println(z);
   if(z.indexOf(in_rfid) >= 0) { // == RFID
    digitalWrite(buzzer,HIGH);
    delay(300);
    digitalWrite(buzzer,LOW);
    delay(300);
     digitalWrite(buzzer,HIGH);
    delay(300);
    digitalWrite(buzzer,LOW);
    delay(300);
   Serial.println(F("HELLO"));
   led = 1 ;
   break ;
   }  Serial.println(F("--------------------------"));
  }ER = 1;
 }//END
/////////////////////////////NodeMCU/////////////////////////////
void NodeMCU(){
 

//    file.print(F("Use "));
    ArduinoSerial.print(y);
   ArduinoSerial.print("/t");
    ArduinoSerial.print(Drugt);
    ArduinoSerial.print("/t");
   ArduinoSerial.print(allwater);
   Serial.print(allwater);
   allwater = 0;


//    file.print(F("ID RFID = "));

    Serial.print("Hi");
}
/////////////////////////////RFID/////////////////////////////
 void Read_rfid(){
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();


  Serial.print("Tap card key: ");
  Serial.println(strID);
  in_rfid = strID ;
  Water = 0 ;
  sd_sum = 1;

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void setup() {
  Serial.begin(9600);
  ArduinoSerial.begin(4800);
  SPI.begin();
  Wire.begin(); // master
  Wire.begin(9); //baby 9
  Wire.onReceive(receiveEvent);
   rfid.PCD_Init();
   pinMode(LED,OUTPUT);
   pinMode(relay1,OUTPUT);
   pinMode(buzzer,OUTPUT);
   pinMode(limswitch,INPUT);
   digitalWrite(relay1,1); 
    if (!SD.begin(SC)) { // NO SD CARD
    Serial.println("begin error");
    return;
  }
  }
  void receiveEvent(){
    Drugt = Wire.read();
  }
  void loop(){
    Read_rfid(); //read rfid
    if(sd_sum == 1){
    Serial.println(in_rfid);
    sdcard();
    file.close();
    if(ER == 1){
    Serial.println(F("Stop"));
    digitalWrite(buzzer,HIGH);
    delay(2000);
    digitalWrite(buzzer,LOW);
    sd_sum = 0;
    }
    }if(led == 1){
    Wire.beginTransmission(8);
    Second = 1;
    Wire.write(Second);
    Serial.println(Second);
    Wire.endTransmission();
    led = 0;
    }
    if(Drugt != 0){
    tmElements_t tm;
    if(RTC.read(tm)){
    Hour = tm.Hour; 
    Minute = tm.Minute;
    Second = tm.Second;
    Day = tm.Day;
    Month = tm.Month;
    Year = tmYearToCalendar(tm.Year);
    }
    Serial.println(Drugt);
    file = SD.open("Lise.txt",FILE_WRITE);
    if(file){
      
    file.print(F("Date (D/M/Y) = "));
    file.print(Day);
    file.print(F("/"));
    file.print(Month);
    file.print(F("/"));
    file.print(Year);
    file.print(F("   "));

    file.print(F("Time (H/M/S) = "));
    file.print(Hour);
    file.print(F(":"));
    file.print(Minute);
    file.print(F(":"));
    file.print(Second);
    file.print(F("   "));

    file.print(F("Use "));
    file.print(y);
    file.print(F("//"));
    file.print(Drugt);
    file.print(F("//"));
    file.print(F("ID RFID = "));
    file.println(z);
    Serial.println(F("Do"));
    file.close();
    NodeMCU();
    Drugt = 0;
    Water = 1;
    
    
}
    if(Water == 1){
    AA = 0;
    sum = 0;
    BB = 0 ;
    tine = 0;
    Water = 0;
    while(AA < 3){    
    water();
}
}
}
}



 


#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(D3, D4); // RX | TX
// --------- NodeMCU to WiFi -------------------------
  #include <ESP8266WiFi.h>
#define WIFI_SSID "LED ROOM.2"
#define WIFI_PASSWORD "ledroom725"
// --------- Firebase -------------------------------------
#include <FirebaseArduino.h>
#define FIREBASE_HOST "hospital-eb0df.firebaseio.com"
#define FIREBASE_AUTH  "Hi4xuwFigaYeIv3W3FksBXj2j4XZcsRzMzUTRWe3"
// ---------- OLED --------------------------------------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);
// ---------- OLED -----------------------------------------
String LINE_TOKEN = "1hSWGsPMgsvaTRUCIIfLAFSpn2ZhyWfVDSSVAZ2A35Z";

int Day = 0;
int Month = 0;
int Year = 0;

int Hour = 0;
int Minute = 0;
int Second = 0;

int y = 0;
int Drugt = 0;
float allwater = 0 ;
float waterall = 0 ;
int value = 0 ;
String named = " " ;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
pinMode(D2, INPUT);
pinMode(D3, OUTPUT);
Serial.begin(115200);
NodeSerial.begin(4800);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) { //รอจนกว่าจะเชื่อมต่อสำเร็จ 
    Serial.print("."); //แสดง ... ไปเรื่อยๆ จนกว่าจะเชื่อมต่อได้
    digitalWrite(LED_BUILTIN, LOW); 
    delay(500);
  } //ถ้าเชื่อมต่อสำเร็จก็จะหลุก loop while ไปค่ะ

  Serial.println(""); 
  Serial.println("Wi-Fi connected"); //แสดงว่าเชื่อมต่อ Wi-Fi ได้แล้ว
  Serial.print("IP Address : ");
   digitalWrite(LED_BUILTIN, HIGH); 
  Serial.println(WiFi.localIP()); //แสดง IP ของบอร์ดที่ได้รับแจกจาก AP
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  
}
void Dr1(){
  String D1 = Firebase.getString("Drug1");
  String DD1 = Firebase.getString("DD1");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Aspirin");
  display.print("Remain : ");
  display.println(D1);

  display.println(DD1);
  display.display();
  delay(3000);
}
void Dr2(){
  String D2 = Firebase.getString("Drug2");
  String DD2 = Firebase.getString("DD2");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Anti-allergy pills");
  display.print("Remain : ");
  display.println(D2);

  display.println(DD2);
  display.display();
  delay(3000);
}
void Dr3(){
  String D3 = Firebase.getString("Drug3");
  String DD3= Firebase.getString("DD3");
   display.clearDisplay();
   display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Flatulence");
  display.print("Remain : ");
  display.println(D3);
  display.println(DD3);
  display.display();
  delay(3000);
}
void WT(){
  String D4 = Firebase.getString("Water");
  String DD4= Firebase.getString("DW1");
   display.clearDisplay();
   display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Water");
  display.println("Remain : ");
  display.print(D4);
  display.println(" Litre");
  display.println(DD4);
  display.display();
  delay(3000);
}
void loop() {
  Dr1();
  Dr2();
  Dr3();
  WT() ;
 if (NodeSerial.available() > 0) {

 y = NodeSerial.parseFloat();
 Serial.println(y); 
 Firebase.setInt("7-User", y);
 Drugt = NodeSerial.parseFloat();
 Serial.println(Drugt); 
 Firebase.setInt("8-Drug",Drugt);
 
allwater = NodeSerial.parseFloat();
String waterall= Firebase.getString("Water");
Serial.println(allwater); 
 float  wa = waterall.toFloat ();
 wa -= allwater ;
 String WA1 = (String)wa;
  Firebase.setString("Water", WA1);
  Serial.println(waterall); 
  
 String Drug1 = Firebase.getString("Drug1");
 int n10 = Drug1.toInt();
 String Drug2 = Firebase.getString("Drug2");
 int n20 = Drug2.toInt();
 String Drug3= Firebase.getString("Drug3");
 int n30 = Drug3.toInt();
 
// int Test = Firebase.getInt("Test");
if(Drugt == 1){
   n10--;
   String D1 = (String)n10;
   named =" ยาลดไข้ ";
    Firebase.setString("Drug1", D1);
    Firebase.setInt("8-Drug", 0);
    
    value = 1 ;
}if(Drugt ==2){
   n20--;
   String D2 = (String)n20;
   named =" ยาแก้แพ้" ;
    Firebase.setString("Drug2", D2);
    Firebase.setInt("8-Drug", 0);
    value = 1 ;
}if(Drugt ==3){
   n30--;
   String D3 = (String)n30;
   named =" ยาแก้ท้องอึด ";
    Firebase.setString("Drug3", D3);
    Firebase.setInt("8-Drug", 0);
    value = 1 ;
}
//---------------------------------ส่งเข้าLine-----------------------------
String s = "ผู้ใช้งาน :"+(String)y+" \n "
            "ยาที่รับ :"+(String)named+" \n "
            "จำนวนยาที่เหลือ"" \n "
            "   ยาลดไข้เหลือ : " +(String)n10+" \n "
            "   ยาแก้แพ้เหลือ : " +(String)n20+" \n "
            "   ยาแก้ท้องอึดเหลือ : " +(String)n30+" \n " ;
if (value == 1) {
      Line_Notify(LINE_TOKEN, s); // ข้อความที่ต้องการส่ง
      
      Firebase.setInt("7-User", 0);
      y = 0 ;
      value  = 0 ;      
  }
}
}
//---------------------------------Line-----------------------------
void Line_Notify(String LINE_Token, String message) {

  String msg = String("message=") + message;

  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Authorization: Bearer " + String(LINE_Token) + "\r\n";
  req += "Content-Length: " + String(msg.length()) + "\r\n";
  req += "\r\n";
  req +=  msg;

  client.print(req);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}





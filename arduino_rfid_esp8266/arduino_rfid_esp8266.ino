
#include "ESP8266WiFi.h"
#include "FirebaseArduino.h"

#include <time.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


#define FIREBASE_HOST "akillikapi-3cbe8-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "jrQvXmQqqAPxMNIYCBddCVndS1ZaAdLK8j4YY1Tp"
#define WIFI_SSID "TP-LINK_D51ABA" 
#define WIFI_PASSWORD "Spfksyn320."

#include "SPI.h"
#include "MFRC522.h"

#define  RST_PIN  D3  // Configurable, see typical pin layout above
#define  SS_PIN  D4 // Configurable, see typical pin layout above

#define BLUE_LED_PIN D2
#define GREEN_LED_PIN D1
#define RED_LED_PIN D0

#define BUZZER_PIN D8

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;


int timezone = 3 * 3600;
int dst = 0;

 WiFiClient client;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
 pinMode(BLUE_LED_PIN, OUTPUT);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());


configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
   
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
       digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
       digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
       digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
       digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
       digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
       digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
       digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
       digitalWrite(GREEN_LED_PIN, LOW);
      delay(100);
    
   
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

String UID="";
String buyuk_harf[]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","R","S","T","U","Y","V","W","X","Z"};
String kucuk_harf[]={"a","b","c","d","e","f","g","h","i","j","k","l","m","m","o","p","r","s","t","u","y","v","w","x","z"};
for(int i=0;i<8;i++){
  int sayi_1=random(1,10);
  String buyuk=buyuk_harf[random(0,25)];
  String kucuk=kucuk_harf[random(0,25)];
  int sira=random(1,4);
  if(sira==1){
  UID+=sayi_1+buyuk+kucuk;
  }else if(sira==2){
    UID+=buyuk+sayi_1+kucuk;
    }else{
    UID+=kucuk+buyuk+sayi_1;
    }
}



  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

int h=p_tm->tm_hour;
int m=p_tm->tm_min;
int s=p_tm->tm_sec;

  String saat=String(p_tm->tm_hour);
  String dakika=String(p_tm->tm_min);
  String saniye=String(p_tm->tm_sec);

  String gun=String(p_tm->tm_mday);
  String ay=String(p_tm->tm_mon + 1);
  String yil=String(p_tm->tm_year + 1900);

if(p_tm->tm_mday<10){
  gun="0"+gun;
  }

  if(p_tm->tm_mon+1<10){
  ay="0"+ay;
  }

if(h<10){
  saat="0"+saat;
  }
  
if(m<10){
  dakika="0"+dakika;
  }

  if(s<10){
  saniye="0"+saniye;
  }

  delay(1000);

Firebase.setString(UID+"/tag",tag);  // tag adlı değişkeni UID/tag isimli klasörde tutuyoruz!
Firebase.setString(UID+"/time",saat+":"+dakika+":"+saniye);
Firebase.setString(UID+"/date",gun+"/"+ay+"/"+yil);



  if (Firebase.failed()) {
      Serial.print("Bir hata oluştu:");
      Serial.println(Firebase.error()); 
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
      delay(700);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
  }
  delay(1000);
  Serial.println("Firebase'den gelen Kayıtlar : "+Firebase.getString(UID)); // UID adlı değişkenimizi çağırıyoruz!
     tag = "";

     sendPushNotification("Akıllı Kapı","Evinize "+gun+"/"+ay+"/"+yil+" tarihinde, "+saat+":"+dakika+":"+saniye+"'da "+tag+" numaralı etiketle giriş yapıldı! ","https://static.ariste.info/wp-content/uploads/2020/04/1200px-Antu_dialog-warning.svg_-1.png");
     
  }



  }

const char* host = "api.pushbullet.com"; 
const char* apiKey = "o.xq0AJCsQvxbAsgZ2Sd2ZoDN1aqyCxFG7";


void sendPushNotification(String baslik,String mesaj,String image_url) {
  
  WiFiClientSecure client;
  client.setInsecure();
  if(!client.connect(host,443)) {
    digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
      delay(700);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
    Serial.println("Sunucuya bağlanılamadı!");
    return;
  }

String url = "/v2/pushes";
  String message = "{\"type\": \"note\", \"title\": \""+baslik+"\", \"image_url\": \""+image_url+"\", \"body\": \""+mesaj+"\"}\r\n";
  Serial.print("requesting URL: ");
  Serial.println(url);
  //send a simple note
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + apiKey + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(message.length()) + "\r\n\r\n");
  client.print(message);

  
  delay(2000);

  
   digitalWrite(BLUE_LED_PIN, HIGH);
     digitalWrite(BUZZER_PIN, HIGH);
     delay(50);
     digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
        delay(50);
        digitalWrite(BLUE_LED_PIN, HIGH);
     digitalWrite(BUZZER_PIN, HIGH);
     delay(50);
     digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
        delay(50);
        digitalWrite(BLUE_LED_PIN, HIGH);
     digitalWrite(BUZZER_PIN, HIGH);
     delay(50);
     digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
        delay(50);
        digitalWrite(BLUE_LED_PIN, HIGH);
     digitalWrite(BUZZER_PIN, HIGH);
     delay(50);
     digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
        delay(500);
        

  /*  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
       
  }  */

  //Herşey hazır tekrardan okutabiliriz..

  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
    digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
    delay(50);
     digitalWrite(BLUE_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(80);
  digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(80);
  digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(80);
  digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(30);
  digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}
  





 

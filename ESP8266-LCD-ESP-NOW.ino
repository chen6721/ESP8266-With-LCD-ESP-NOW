#include <espnow.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int m=0;

typedef struct struct_message {
    char text[32];
    char text1[16];
    char text2[16];
} struct_message;

struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Text: ");
  Serial.println(myData.text);
  Serial.println(myData.text1);
  Serial.println(myData.text2);

  String tmpText=String(myData.text);
  tmpText.replace(" ","");
  if(tmpText!=""){
    if(sizeof(myData.text)>16){
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(String(myData.text).substring(0,15));
      lcd.setCursor(0,1);
      lcd.print(String(myData.text).substring(16));
    }else{
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(myData.text);
        }
    return;
    }
  
  String tmpText1=String(myData.text1);
  tmpText1.replace(" ","");
  if(tmpText1=""){
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print(myData.text1);
  }
  String tmpText2=String(myData.text2);
  tmpText2.replace(" ","");
  if(tmpText2!=""){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(myData.text2);
  }
  
}



void setup(){
  Serial.begin(115200);
  
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  
  Serial.println("Start");
  lcd.setCursor(0,0);
  lcd.print("Start!");

  m=millis();


  //ESP NOW
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}


void loop(){

}

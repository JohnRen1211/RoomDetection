// #include<WiFi.h>
// #include<Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// #define WIFI_SSID "PLDTHOMEFIBR67508"
// #define WIFI_PASSWORD "PLDTWIFI7ghd3"
// #define API_KEY "AIzaSyDG-PGx4nvsPsvXLNoJOlD92MuSEl8iKsY"
// #define DATABASE_URL "https://connectesp-fdf9b-default-rtdb.asia-southeast1.firebasedatabase.app/"

// #define LED1_PIN 12
// #define LED_PIN 14
// #define LDR_PIN 36

// FirebasedDta fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// unsigned long sendDataPrevMillis = 0;
// bool singupOK = false;
// int ldrData = 0; 
// float voltage = 0.0;



// void setup() {
//   Serial.begin(115200);
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("Connecting to Wi-Fi")
//   while(Wifi.status () != WL_CONNECTED){
//     Serial.print("."); delay(300);
//   }
//   Serial.println();
//   Serial.print("Connected with IP: ");
//   Serial.println(WiFi.localIP());
//   Serial.println();

//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;
//   if(Firebase.singUp(&config, &auth, "", "")){
//     Serial.println("SignUp OK");
//     signupOK = true;
//   }
//   else {
//     Serial.printf("%s\n", config.signer.signupError.message.c_str());
//   }
//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);
  
// }

// void loop(){
//   if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 50000 || sendDataPrevMillis == 0)  ){ 
//     sendDataPrevMillis = millis();
//     // To store data in firebase
//     ldrData = analogRead(LDR_PIN);
//     voltage = (float)analogReadMilliVolts(LDR_PIN)/1000;
//     if(Firebase.RTDB.setInt(&fbdo, "Sensor/ldr_data", ldrData)){
//       Serial.println(); Serial.print(ldrdata);
//       Serial.print(" - successfully saved to: " + fbdo.datapath());
//       Serial.println(" (" + fbdo.dataType() + ")");
//     } else{
//       Serial.println("NOT: " + fbdo.errorReason()); 
//     }
//     if(Firebase.RTDB.setFloat(&fbdo, "Sensor/voltage", voltage)){
//       Serial.println(); Serial.print(voltage);
//       Serial.print(" - successfully saved to: " + fbdo.datapath());
//       Serial.println(" (" + fbdo.dataType() + ")");
//     } else{
//       Serial.println("NOT: " + fbdo.errorReason()); 
//     }
    

//   }

// }

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "PLDTHOMEFIBR67508"
#define WIFI_PASSWORD "PLDTWIFI7ghd3"
#define API_KEY "AIzaSyDG-PGx4nvsPsvXLNoJOlD92MuSEl8iKsY"
#define DATABASE_URL "https://connectesp-fdf9b-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define LED1_PIN 12
#define LED_PIN 14
#define LDR_PIN 36

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0; 
float voltage = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); 
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 50000 || sendDataPrevMillis == 0)) { 
    sendDataPrevMillis = millis();
    // To store data in Firebase
    ldrData = analogRead(LDR_PIN);
    voltage = (float)analogReadMilliVolts(LDR_PIN) / 1000;

if (Firebase.RTDB.setInt(&fbdo, "Sensor/ldr_data", ldrData)) {
  Serial.println(); 
  Serial.print(ldrData);
  Serial.print(" - successfully saved to: " + fbdo.dataPath()); // Change here
  Serial.println(" (" + fbdo.dataType() + ")");
} else {
  Serial.println("NOT: " + fbdo.errorReason()); 
}

if (Firebase.RTDB.setFloat(&fbdo, "Sensor/voltage", voltage)) {
  Serial.println(); 
  Serial.print(voltage);
  Serial.print(" - successfully saved to: " + fbdo.dataPath()); // Change here
  Serial.println(" (" + fbdo.dataType() + ")");
} else {
  Serial.println("NOT: " + fbdo.errorReason()); 
}
  }
}
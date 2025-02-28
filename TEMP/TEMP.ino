// // //TEMPERATURE SETUP
// #define WIFI_SSID "PLDTHOMEFIBR67508"
// #define WIFI_PASSWORD "PLDTWIFI7ghd3"
// #define API_KEY "AIzaSyCpx8WLJsKnpKgm5qluMuSJlDAH3cpeLp4"
// #define DATABASE_URL "https://temperature-8a8a0-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #include <DHT.h>

// DHT dht(26, DHT11);

// void setup() {
//   dht.begin();
//   delay(2000);

//   Serial.begin(115200);

// }

// void loop() {
//   float temp = dht.readTemperature();
//   float humidity = dht.readHumidity();
//   Serial.print("Temp: ");
//   Serial.print(temp);
//   Serial.print(" C ");
//   Serial.print("Humidity: ");
//   Serial.print(humidity);
//   Serial.println(" % ");
//   delay(2000);
// }

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <DHT.h>

// Wi-Fi and Firebase credentials
#define WIFI_SSID "PLDTHOMEFIBR67508"
#define WIFI_PASSWORD "PLDTWIFI7ghd3"
#define API_KEY "AIzaSyCpx8WLJsKnpKgm5qluMuSJlDAH3cpeLp4"
#define DATABASE_URL "https://temperature-8a8a0-default-rtdb.asia-southeast1.firebasedatabase.app/"

// DHT11 Sensor setup
#define DHT_PIN 26
DHT dht(DHT_PIN, DHT11);

// Firebase setup
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(2000);

  // Initialize Wi-Fi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Set up Firebase configuration
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

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 50000 || sendDataPrevMillis == 0)) { 
    sendDataPrevMillis = millis();

    // Read temperature and humidity data
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Print data to Serial Monitor
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % ");

    // Send data to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "Sensor/temperature", temp)) {
      Serial.println("Temperature data sent successfully!");
    } else {
      Serial.println("Error sending temperature data: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Sensor/humidity", humidity)) {
      Serial.println("Humidity data sent successfully!");
    } else {
      Serial.println("Error sending humidity data: " + fbdo.errorReason());
    }
  }
}

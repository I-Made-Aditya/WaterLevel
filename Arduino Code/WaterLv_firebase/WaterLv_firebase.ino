#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// // Network credentials
// #define WIFI_SSID "GLF36KOS"
// #define WIFI_PASSWORD "SelamatPaka1"

// Network credentials
#define WIFI_SSID "Rumah Kos Oren"
#define WIFI_PASSWORD "jagalahkebersihan!"

// Firebase API Key & URL
#define API_KEY "AIzaSyCztyzTjcDiOZucr8elpd5rY4vodp52r1Y"
#define DATABASE_URL "https://kost-oren-kpri-sekawan-default-rtdb.asia-southeast1.firebasedatabase.app/"

// ESP32 serial port for A02YYUW sensor
#define A02YYUW_TX 17
#define A02YYUW_RX 16

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600);

HardwareSerial mySerial(2);
bool signupOK = false;
float distance;
unsigned long lastSensorUpdate = 0;

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(800);
  }
  Serial.println("Connected! IP: " + WiFi.localIP().toString());
}

void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("Firebase Signup: OK");
  } else {
    Serial.printf("Firebase Error: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

float getSensorData() {
  if (mySerial.available() >= 4) {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = mySerial.read();
    }

    if (data[0] == 0xFF && ((data[0] + data[1] + data[2]) & 0xFF) == data[3]) {
      float dist = (data[1] << 8) + data[2];
      lastSensorUpdate = millis();
      return (dist > 30) ? dist / 10.0 : -1;  // Validasi jarak
    }
  }
  return -1;
}

void sendDataToFirebase(float dist) {
  static float lastSentValue = -1;                                         // Simpan nilai terakhir yang dikirim
  if (Firebase.ready() && signupOK && fabs(dist - lastSentValue) > 0.2) {  // Hanya kirim jika berubah > 0.5cm
    timeClient.update();
    unsigned long unixTimestamp = timeClient.getEpochTime();
    if (Firebase.RTDB.setFloat(&fbdo, "realtime/dist", dist)) {
      lastSentValue = dist;  
      Serial.println("Data Sent: " + String(dist) + " cm");
    }
    Firebase.RTDB.setInt(&fbdo, "realtime/timestamps", unixTimestamp);
  }
}


void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, A02YYUW_RX, A02YYUW_TX);

  initWiFi();
  timeClient.begin();
  initFirebase();
}

void loop() {
  float sensorValue = getSensorData();
  if (sensorValue >= 0) {
    sendDataToFirebase(sensorValue);
  }
  if (millis() - lastSensorUpdate > 5000){
    mySerial.end();
    delay(200);
    mySerial.begin(9600, SERIAL_8N1, A02YYUW_RX, A02YYUW_TX);
  }
  delay(100); 
}

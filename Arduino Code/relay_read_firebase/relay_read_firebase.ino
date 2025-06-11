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
#define WIFI_SSID "Kos Rumah Oren"
#define WIFI_PASSWORD "BlokNno22"

// Firebase API Key & URL
#define API_KEY "AIzaSyCztyzTjcDiOZucr8elpd5rY4vodp52r1Y"
#define DATABASE_URL "https://kost-oren-kpri-sekawan-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define pi 3.1416

const int relay = 25;
unsigned long previousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya
const long interval = 1000; // Interval cek Firebase (2 detik)

//convert
float H_water = 0;
float V_water = 0;
float Htank = 26.0; // cm
float radius = 13.5; // cm
float C_water = 0;

// Objek Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false; 

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

bool getRelayState() {
  if (Firebase.ready() && signupOK) {
    if (Firebase.RTDB.getBool(&fbdo, "/realtime/relay")) {
      return fbdo.boolData(); 
    } else {
      Serial.print("Gagal membaca data: ");
      Serial.println(fbdo.errorReason());
    }
  }
  return false; // Default jika gagal membaca data
}

int getTimestamp() {
  if (Firebase.ready() && signupOK) {
    if (Firebase.RTDB.getInt(&fbdo, "/realtime/timestamps")) {
      return fbdo.intData(); // Mengembalikan nilai timestamp sebagai integer
    } else {
      Serial.print("Gagal membaca data: ");
      Serial.println(fbdo.errorReason());
    }
  }
  return 0; // Default jika gagal membaca data
}

float getDist() {
  if (Firebase.ready() && signupOK) {
    if (Firebase.RTDB.getFloat(&fbdo, "/realtime/dist")) {
      return fbdo.floatData(); // Mengembalikan nilai sebagai float
    } else {
      Serial.print("Gagal membaca data: ");
      Serial.println(fbdo.errorReason());
    }
  }
  return 0.0; // Default jika gagal membaca data
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initFirebase(); // Wajib agar Firebase bisa digunakan

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  unsigned long currentMillis = millis(); // Ambil waktu saat ini

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Perbarui waktu sebelumnya

    bool relayState = getRelayState();
    int Time = getTimestamp();
    float Dist = getDist();

    H_water = Htank - Dist;
    C_water = (H_water / Htank) * 100;

    Serial.print("relayState: " + String(relayState));
    Serial.print("\t Timestamps: " + String(Time));
    Serial.print("\t Distance: " + String(Dist));
    Serial.print("\t Capacity: " + String(C_water));

    if (relayState) {
      Serial.println(" Relay Aktif");
      digitalWrite(relay, HIGH);
    } else {
      Serial.println(" Relay Nonaktif");
      digitalWrite(relay, LOW);
    }
  }
}

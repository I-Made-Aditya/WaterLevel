#include <Arduino.h>

#define Htank 10 // Heigh of tank
#define pi 3.1416
#define radius 5.0 //radius of tank

// Define ESP32 hardware serial port for the A02YYUW sensor
#define A02YYUW_TX 1  // ESP32 TX connected to Sensor RX
#define A02YYUW_RX 3   // ESP32 RX connected to Sensor TX

HardwareSerial mySerial(0);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, A02YYUW_RX, A02YYUW_TX);
}

void loop() {
  if (mySerial.available() >= 4) {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = mySerial.read();
    }
    if (data[0] == 0xFF) {  // Check packet start byte
      int sum = (data[0] + data[1] + data[2]) & 0xFF;
      if (sum == data[3]) {  // Validate checksum
        int distance = (data[1] << 8) + data[2];  // Calculate distance
        if (distance > 30) {  // Ensure valid reading
          int distanceCM = distance / 10.0;
          int H_water = Htank - distance;
          float V_water = pi * radius * radius * H_water / 1000; // in liter

          Serial.print("Distance: ");
          Serial.print(distanceCM);
          Serial.print(" cm, Heigh of Water: ");
          Serial.print(H_water);
          Serial.print(" cm, Volume: ");
          Serial.print(V_water);
          Serial.println(" L");
        }
        else {
          Serial.println("Below the lower limit");
        }
      } else {
        Serial.println("Checksum error");
      }
    }
  }
  delay(500);
}

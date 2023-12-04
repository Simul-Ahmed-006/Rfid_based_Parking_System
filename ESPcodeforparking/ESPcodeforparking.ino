#include <ESP8266WiFi.h>
int sensorValues[6] = {1, 1, 1, 1, 1, 1}; // Array to store sensor values received from Arduino

void setup() {
  Serial.begin(115200);
  // Serial.swap(); // Swap RX/TX pins for ESP8266 SoftwareSerial

  // Initialize communication with Arduino
  Serial.begin(115200);
  // Serial1.begin(115200, RX_PIN, TX_PIN);
  // Serial1.begin(115200);
  // Connect to Wi-Fi
  // WiFi.begin("YourSSID", "YourPassword");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");
}

void loop() {

ReadSpace();
delay(1000);

}


void ReadSpace()
{
  if (Serial.available()) 
  {
    String sensorData = Serial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(sensorData);

    // Parse and store received sensor data
    for (int i = 0; i < 6; i++) {
      // Example format: ir[0]: value
      String irString = "ir[" + String(i) + "]:";
      int startIndex = sensorData.indexOf(irString);
      if (startIndex != -1) {
        int endIndex = sensorData.indexOf('\n', startIndex);
        String valueString = sensorData.substring(startIndex + irString.length(), endIndex);
        sensorValues[i] = valueString.toInt();
      }
    }

    // Display stored sensor values
    Serial.println("Stored sensor values:");
    for (int i = 0; i < 6; i++) {
      Serial.print("Sensor");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(sensorValues[i]);
    }
  }
}
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4); // Change the I2C address if needed

#include <Servo.h>
Servo myServo; // Create a servo object

#define NUM_SENSORS 7

int sensorPins[NUM_SENSORS] = {2, 3, 4, 5, 6, 7, 8};
int ir[NUM_SENSORS];

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
   myServo.attach(9); // Attach the servo to pin 9

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  // Read values from all IR sensors and store in ir array
  for (int i = 0; i < NUM_SENSORS; i++) {
    ir[i] = digitalRead(sensorPins[i]);
  
  
  
  }
  exit();
  updateLCD();

  // Send sensor data to ESP8266
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("ir[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(ir[i]);


  }


}




void updateLCD()
{
    // Segment 1
  lcd.setCursor(0, 0);
  lcd.print("SL:0");

  lcd.setCursor(1, 1);
  lcd.print(ir[0] == 1 ? " AV" : "NAV");

  // Segment 2
  lcd.setCursor(7, 0);
  lcd.print("SL:1");

  lcd.setCursor(8, 1);
  lcd.print(ir[1] == 1 ? " AV" : "NAV");

  // Segment 3
  lcd.setCursor(14, 0);
  lcd.print("SL:2");

  lcd.setCursor(15, 1);
  lcd.print(ir[2] == 1 ? " AV" : "NAV");

  // Segment 4
  lcd.setCursor(0, 2);
  lcd.print("SL:3");

  lcd.setCursor(1, 3);
  lcd.print(ir[3] == 1 ? " AV" : "NAV");

  // Segment 5
  lcd.setCursor(7, 2);
  lcd.print("SL:4");

  lcd.setCursor(8, 3);
  lcd.print(ir[4] == 1 ? " AV" : "NAV");

  // Segment 6
  lcd.setCursor(14, 2);
  lcd.print("SL:5");

  lcd.setCursor(15, 3);
  lcd.print(ir[5] == 1 ? " AV" : "NAV");


}

void exit()
{

   // Check the value of ir[7]
  if (ir[6] == 0) {
    // Open the servo
    myServo.write(70); // Set servo angle to 180 degrees (open position)
    delay(2000); // Wait for 1 second
  } else {
    // Close the servo
    myServo.write(180); // Set servo angle to 0 degrees (closed position)
    delay(1000); // Wait for 1 second
  }
}






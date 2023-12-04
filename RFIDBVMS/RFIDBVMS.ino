//AKfycbx2lKUCD_6mEDcWxLCGn6g6Lnt0RCFRxnngXqXocst9GwVlLL-RkVYc_nW7X9Gc22gTLQ
///////////////////////// /////////////////////////////        libraries

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//////////////////////////////////////// /////////////        pin variables
// Declare IR sensor pins
#define EnIRpin D4
// Declare servo pins
#define servo1Pin  D1 // Entry
// rfid

#define RST_PIN D3     // Define the RST_PIN for the RFID reader
#define SS_PIN D2      // Define the SS_PIN (CS) for the RFID reader
//https://script.google.com/macros/s/AKfycbyglyeDQlJ3K0nSnH7PEqy7Oh7WFlCvePdveXkXff25bYYe7jsEgfSIZSwWo4OI2Ujt/exec?sts=write&space0=CAR-NUMBER

/////////////////////////////////////////////////////      data variables  
String Web_App_URL = "https://script.google.com/macros/s/AKfycbx2lKUCD_6mEDcWxLCGn6g6Lnt0RCFRxnngXqXocst9GwVlLL-RkVYc_nW7X9Gc22gTLQ/exec?sts=write&"; // ADD ?sts=write&
//https://script.google.com/macros/s/AKfycby5jO7VpOQNS9ibzUWo0jzz5VQJfkjVVgJ_eu28Px-98VVIZHlnbFAQyCkkkQ6EjIsb/exec?sts=write&space0=Off&space1=Success&space2=32.5&space3=95&space4=Off&space5=Off
//PixelS
//pixel5678
//----------------------------------------SSID and PASSWORD of your WiFi network.

const char* ssid = "PixelS";  //--> Your wifi name
const char* password = "pixel5678"; //--> Your wifi password
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.
//----------------------------------------
int openAngle = 0;
int closeAngle = 180;
int writeState = 2;
String Free = "FREE"; // Add a semicolon at the end of this line
String space[6] = {Free, Free, Free, Free, Free, Free};
String oldspace[6] = {"Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"};
String CarNumber ="Unknown";




/////////////////////////////////////////////              servo class

Servo servo1; 
 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() 
{
  Serial.begin(115200);
  // Serial.swap(); // Swap RX/TX pins for ESP8266 SoftwareSerial
  servo1.attach(servo1Pin);
  servo1Control(closeAngle);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready to write or read data from an RFID card!");

  Serial.println(); 
  pinMode(D4,INPUT);



   WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  // pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  // digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    // digitalWrite(ON_Board_LED, LOW);
    delay(250);
    // digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  // digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  client.setInsecure();
  // Serial.swap(); // Swap RX/TX pins for ESP8266 SoftwareSerial cheak it ////



}

void loop() 
{
  
  if (writeState ==3)
  {
    sendData();

  }
  // sendData();
  Serial.print("Write State: ");
  Serial.print(writeState);


//   String RFIDData = ReadRF();
// if (RFIDData.length() > 0) 
// {
//     Serial.print("RFID Data: ");
//     Serial.println(RFIDData);
//   }
//   else
//   {
//     Serial.print("No Data: ");

    //   }
    delay(100);
    String RFIDData = ReadRF();
    if (RFIDData.length() > 0) 
    {
        Serial.print("RFID Data: ");
        Serial.println(RFIDData);
        CarNumber = RFIDData;
        servo1Control(openAngle);
        Serial.println("servo1  : open");
          int sensorValue = digitalRead(EnIRpin);
          Serial.print("GATE E :");
          Serial.println(sensorValue);
          int i=0;
          while(sensorValue != 0)
          {
            delay(50);
            i = i + 5;
            if(i>3000)
            {
              break;
            }
            sensorValue = digitalRead(EnIRpin);
            Serial.print("GATE E :");
          Serial.println(sensorValue);
          }
          servo1Control(closeAngle);
          Serial.println("servo1  : closed");
          if(i<3000)
          { 
            
            writeState = 3;
            Serial.print("writeState :");
            Serial.println(writeState);
          }
          else
          {
            writeState = 2;
            Serial.print("writeState :");
            Serial.println(writeState);
          }
        }
        else
        {
          Serial.println("No card");

        }
}

    
 


 


////////////Servo motor control functions
void servo1Control(int angle) 
{
  servo1.write(angle);
}



//////////////////////////////////////////// rf reader function

String ReadRF() {
  // Look for a card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");

    // Authenticate using key A
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF; // Use 0xFF for all key bytes by default
    }

    if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
      Serial.println("Authentication successful!");

      // Read data from the card
      byte buffer[18];
      byte bufferSize = sizeof(buffer);

      MFRC522::StatusCode status = mfrc522.MIFARE_Read(1, buffer, &bufferSize);
      if (status == MFRC522::STATUS_OK) {
        // Store the read data in a string variable
        char readData[16];
        String RFIDData = "";
        for (int i = 0; i < 15; i++) {
          readData[i] = buffer[i];
          RFIDData += readData[i];
        }

        // Halt PICC
        mfrc522.PICC_HaltA();
        // Stop encryption on PCD
        mfrc522.PCD_StopCrypto1();

        // Return the RFID data as a string
        return RFIDData;
      } else {
        Serial.println("Error reading data from the card!");
      }
    } else {
      Serial.println("Authentication failed!");
    }

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }

  delay(100); // Avoid reading cards too fast

  return ""; // Return an empty string if no card is detected or reading fails
}



// Subroutine for sending data to Google Sheets
void sendData() {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    writeState =3;
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  
  String url = Web_App_URL + "space0=" + CarNumber;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
    writeState = 2;
  } else {
    Serial.println("esp8266/Arduino CI has failed");
    writeState = 2;
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
//==============================================================================
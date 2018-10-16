#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include "config.h" 

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SimpleDHT.h>

#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

//////RFID 13.67MHZ (NFC)
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         4          // Configurable, see typical pin layout above
#define SS_PIN          5         // Configurable, see typical pin layout above
// MOSI 12
// MISO 13
// SCK 14
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

////DHT11
const int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

const int piezoPin = 0;


ADC_MODE(ADC_VCC);

WiFiClientSecure client;

const char* host = AWS_URL;
String url = AWS_ENDPOINT;
String ssid = WIFI_SSID;
String password = KEY;

String RFID_tag = "";      
bool sendnewRequest = false;

// Data to put in the HTTPS request
String data;
boolean httpsReceived = false;
int count = 0;


// constants won't change. They're used here to set pin numbers:
const int buttonPin = 15;     // the number of the pushbutton pin
const int ledPin =  16;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  wifiConnection();

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  
//  Serial.println(F("RFID Setup: \nScan PICC to see UID, SAK, type, and data blocks... "));
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  
  tone(piezoPin, 1000, 500);
  delay(1000);
  noTone(piezoPin);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (sendnewRequest == true) {
    digitalWrite(ledPin, LOW);
    Serial.println("\nHTTP Request \nPOST to https://" + String(host) + url +"\n");
    Serial.println(RFID_tag);
    if (wifiConnection()) {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      
      root["RFID_tag"] = RFID_tag; 
      root["temperature"] = String(temperatureValue());
      
      root.prettyPrintTo(data);
      Serial.println(url);
      Serial.println(data);

//      printDots();
      Serial.print("Result(response): ");
      Serial.println(httpsPost(url, data) + "\n");
          digitalWrite(ledPin, HIGH);
      tone(piezoPin, 500, 500);
      delay(200);
      noTone(piezoPin);
      data = "";
      sendnewRequest = false;
      }
  }
  
   
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  if(String(mfrc522.PICC_GetTypeName(piccType)) == "MIFARE Ultralight or Ultralight C"){
    RFID_tag = "Black Tag";
    sendnewRequest = true;
  }
  if(String(mfrc522.PICC_GetTypeName(piccType)) == "MIFARE 1KB"){
    RFID_tag = "Blue Tag";
    sendnewRequest = true;
  }
  
  digitalWrite(ledPin, LOW);
  tone(piezoPin, 1000, 500);
  delay(500);
  digitalWrite(ledPin, HIGH); 
  delay(500);
  noTone(piezoPin);
}

boolean wifiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Waiting for Wi-Fi connection");
  while ( count < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected! \n");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

String httpsPost(String url, String data) {
  if (client.connect(host, 443)) {
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + (String)host);
    client.println("User-Agent: ESP8266/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/json;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    delay(10);
    String response = client.readString();
    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    if(String(response.substring(bodypos)) == "{'Status': 'Success!'}"){
      httpsReceived = true;
    }
    return response.substring(bodypos);
  }
  else {
    return "ERROR";
  }
}

String temperatureValue(){
    // read without samples.
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Temperature Reading: \nRead DHT11 failed, err="); Serial.println(err);delay(1000);
//      return "ERROR Temperature Sensor";
    }
    
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, "); 
    Serial.print((int)humidity); Serial.println(" H\n");
    return String(temperature);
    // DHT11 sampling rate is 1HZ.
    delay(1500);
}

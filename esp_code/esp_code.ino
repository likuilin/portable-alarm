/**
   BasicHTTPSClient.ino
    Created on: 20.08.2018
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
const uint8_t fingerprint[20] = {0x86, 0x07, 0x66, 0x1a, 0x0c, 0x4d, 0x55, 0x69, 0x8e, 0xbd, 0x33, 0xaf, 0x65, 0xda, 0xd7, 0xfd, 0x85, 0xa9, 0xd6, 0x8c};

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  
  pinMode(14, INPUT);
  pinMode(5, OUTPUT);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Kuilin's iPad", "asdfghjkl");
}


//last write was a SEND in the high continuous
int lastSent = 0;

void loop() {
  if (digitalRead(14) == HIGH) {
    Serial.println("Motion: HIGH");
    if (lastSent)
      makeRequest("https://kuilin.net/portalert/write.php?data=ALARM_ON");
    else 
      makeRequest("https://kuilin.net/portalert/write.php?data=ALARM_ON&send");
    lastSent = 1;
  } else {
    Serial.println("Motion: LOW");
    lastSent = 0;
    makeRequest("https://kuilin.net/portalert/alert.txt");
  }
  delay(500);
}

void makeRequest(String url) {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    //Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, url)) {  // HTTPS

      //Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          if (payload.equals("ALARM_ON")) {
            Serial.println("Alarming!");
            digitalWrite(5, HIGH);
          } else {
            digitalWrite(5, LOW);
          }
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

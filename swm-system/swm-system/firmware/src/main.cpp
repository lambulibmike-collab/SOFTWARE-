#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../include/fill_logic.h"
#include "config.h" // copy config.h.example to config.h and set real values

// Firmware scaffold: this file intentionally requires your actual modem/APN setup.
// SIM800L GPRS TLS limitations are described in the project README. Do not use plain MQTT
// over an untrusted network in production. Implement modem power gating for battery targets.
WiFiClient net; PubSubClient mqtt(net); unsigned long cycleCount=0;
float sampleDistance(){
  float samples[5]; int n=0;
  for(int i=0;i<5;i++){digitalWrite(TRIG_PIN,LOW);delayMicroseconds(3);digitalWrite(TRIG_PIN,HIGH);delayMicroseconds(10);digitalWrite(TRIG_PIN,LOW);unsigned long us=pulseIn(ECHO_PIN,HIGH,30000);float d=swm::pulseToCm(us);if(swm::validRange(d))samples[n++]=d;delay(60);}
  return n?swm::medianOf(samples,n):-1.0f;
}
void setup(){
  Serial.begin(115200);pinMode(TRIG_PIN,OUTPUT);pinMode(ECHO_PIN,INPUT);
  if(!SPIFFS.begin(true))Serial.println("SPIFFS init failed");
  const float distance=sampleDistance();const float fill=distance<0?-1:swm::fillPercent(distance,BIN_HEIGHT_CM);
  StaticJsonDocument<256> doc;doc["node_id"]=NODE_ID;doc["ts"]=(uint32_t)time(nullptr);doc["raw_distance_cm"]=distance;doc["fill_level_percent"]=fill;doc["battery_v"]=analogReadMilliVolts(BAT_PIN)*2.0f/1000.0f;
  String payload;serializeJson(doc,payload);Serial.println(payload);
  // TODO: connect the SIM800L via TinyGSM, connect MQTT, publish to swm/<NODE_ID>/telemetry.
  // When unavailable, append payload to a bounded SPIFFS queue (max 50 records) and flush on reconnect.
  cycleCount++;esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_MINUTES*60ULL*1000000ULL);esp_deep_sleep_start();
}
void loop(){}

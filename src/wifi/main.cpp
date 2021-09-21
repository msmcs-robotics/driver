#include <Arduino.h>
#include <ESP8266WiFi.h>

const auto BAUD_RATE = 115200;

// cppcheck-suppress unusedFunction
void setup() {
    Serial.begin(BAUD_RATE);

    if (!WiFi.softAP((const char*)WIFI_SSID, (const char*)WIFI_PASS)) {
        Serial.println("Error: Starting AP failed");
    } else {
        Serial.println("Started AP");
    }
}

// cppcheck-suppress unusedFunction
void loop() {}

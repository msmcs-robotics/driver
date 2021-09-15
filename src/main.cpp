#include <Arduino.h>

#define BAUD_RATE 9600

// cppcheck-suppress unusedFunction
void setup() {
    Serial.begin(BAUD_RATE);
    Serial.print("Hello, world!");
}

// cppcheck-suppress unusedFunction
void loop() {}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <pb_decode.h>

#include "motor_voltage.pb.h"

const auto BAUD_RATE = 115200;
const auto LISTEN_PORT = 1496;

WiFiUDP Udp;

// cppcheck-suppress unusedFunction
void setup() {
    Serial.begin(BAUD_RATE);

    if (!WiFi.softAP((const char*)WIFI_SSID, (const char*)WIFI_PASS)) {
        Serial.println("Error: Starting AP failed");
    } else {
        Serial.println("Started AP");
    }

    if (Udp.begin(LISTEN_PORT) != 1) {
        Serial.println("Error: Binding to UDP socket failed");
    } else {
        Serial.println("Started UDP listener");
    }
}

// cppcheck-suppress unusedFunction
void loop() {
    auto packet_len = Udp.parsePacket();

    if (packet_len > 0) {
        unsigned char packet_data[packet_len];

        // There is no need to check the return value here since we know a
        // buffer is available after calling Udp.parsePacket().
        Udp.read(packet_data, packet_len);

        MotorVoltage voltage = MotorVoltage_init_zero;
        auto stream = pb_istream_from_buffer(packet_data, packet_len);
        if (!pb_decode(&stream, &MotorVoltage_msg, &voltage)) {
            Serial.printf(
                "Warning: Message decoding failed: %s\n",
                PB_GET_ERROR(&stream));
        } else {
            Serial.printf("Left voltage: %d\n", voltage.left);
            Serial.printf("Right voltage: %d\n", voltage.right);
        }
    }
}

#include <Arduino.h>
#include <Wire.h>
#include <pb_decode.h>

#include "motor_voltage.pb.h"

const auto BAUD_RATE = 115200;
const auto I2C_ADDRESS = 8;

void handle_packet(int packet_len);

// cppcheck-suppress unusedFunction
void setup() {
    Serial.begin(BAUD_RATE);
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(handle_packet);
}

// cppcheck-suppress unusedFunction
void loop() {}

void handle_packet(int packet_len) {
    unsigned char packet_data[packet_len];

    for (auto i = 0; i < packet_len; i++) {
        packet_data[i] = Wire.read();
    }

    MotorVoltage voltage = MotorVoltage_init_zero;
    auto stream = pb_istream_from_buffer(packet_data, packet_len);
    if (!pb_decode(&stream, &MotorVoltage_msg, &voltage)) {
        Serial.print("Warning: Message decoding failed: ");
        Serial.println(PB_GET_ERROR(&stream));
    } else {
        Serial.print("Left voltage: ");
        Serial.println(voltage.left);
        Serial.print("Right voltage: ");
        Serial.println(voltage.right);
    }
}

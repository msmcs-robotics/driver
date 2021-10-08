#include <Arduino.h>
#include <Wire.h>
#include <pb_decode.h>

#include "motor_voltage.pb.h"

const auto BAUD_RATE = 115200;
const auto I2C_ADDRESS = 8;

const auto MOTOR_LFORWARD = 9;
const auto MOTOR_LBACK = 8;
const auto MOTOR_LVOLTAGE = 5;
const auto MOTOR_RFORWARD = 7;
const auto MOTOR_RBACK = 3;
const auto MOTOR_RVOLTAGE = 6;

void handle_packet(int packet_len);
void drive(int left_voltage, int right_voltage);

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
        drive(voltage.left, voltage.right);
    }
}

// Values outside of the range [-255, 255] will produce unexpected behavior. It
// is up to the caller to ensure values outside of this range are not passed.
void drive(int left_voltage, int right_voltage) {
    digitalWrite(MOTOR_LFORWARD, left_voltage >= 0 ? HIGH : LOW);
    digitalWrite(MOTOR_LBACK, left_voltage < 0 ? HIGH : LOW);
    digitalWrite(MOTOR_RFORWARD, right_voltage >= 0 ? HIGH : LOW);
    digitalWrite(MOTOR_RBACK, right_voltage < 0 ? HIGH : LOW);

    analogWrite(MOTOR_LVOLTAGE, abs(left_voltage));
    analogWrite(MOTOR_RVOLTAGE, abs(right_voltage));
}

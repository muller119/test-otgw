#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace otgw {

const int OTGW_BUFFER_SIZE = 128;
const int OTGW_BUFFER_INVALID = -1;
const int OTGW_MAX_LINE_DURATION_MS = 150;
const int OTGW_COMMAND_RESPONSE_MAX_DURATION_MS = 5000;
const int OTGW_OTMESSAGE_TIMEOUT_MS = 2000; // spec: 1 sec +/- 15%

class OpenThermGateway : public Component, public uart::UARTDevice {
public:
    OpenThermGateway() {
        this->buffer_pos = 0;
        this->state = 0;
    }
    void setup() override;
    void loop() override;

    void set_version_sensor(text_sensor::TextSensor *sensor) { this->sensor_version_ = sensor; }
    void set_sensor_burner_operation_hours(sensor::Sensor *sensor) { this->sensor_burner_operation_hours_ = sensor; }
protected:
    int buffer_pos;
    char buffer[OTGW_BUFFER_SIZE];
    uint32_t buffer_start_time;

    int state;
    const uint8_t* last_command_sent{nullptr};
    uint32_t command_request_start_time;

    uint32_t last_valid_otmessage;

    text_sensor::TextSensor *sensor_version_{nullptr};
    sensor::Sensor *sensor_burner_operation_hours_{nullptr};

    void read_incoming_data();
    void parse_buffer();
    bool parse_command_response();
    void parse_otmessage();
    void send_command(const uint8_t* command, const uint8_t* data, size_t datalen);
    bool should_send_command();
    bool response_is_from_last_command();
    void request_version();
    void set_printsummary();
    bool command_response_startswith(const char* startstring, int startstringlen);
    bool command_response_equals(const char* contents, int contentslen);
    void go_idle();
    void check_otmessage_timeout();
};

}
}
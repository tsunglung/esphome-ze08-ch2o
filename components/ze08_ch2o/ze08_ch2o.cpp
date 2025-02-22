#include "ze08_ch2o.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ze08_ch2o {

static const char *const TAG = "ze08_ch2o";

static const uint8_t ZE08_SET_QA_MODE[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
static const uint8_t ZE08_SET_ACTIVE_MODE[] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};
static const uint8_t ZE08_QUESTION[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};


float Ze08CH2OComponent::get_setup_priority() const { return setup_priority::DATA; }

void Ze08CH2OComponent::setup() {
    if (this->mode_ == ZE08_MODE_QA) {
        this->write_array(ZE08_SET_QA_MODE, sizeof(ZE08_SET_QA_MODE));
    } else if (this->mode_ == ZE08_MODE_ACTIVE) {
        this->write_array(ZE08_SET_ACTIVE_MODE, sizeof(ZE08_SET_ACTIVE_MODE));
    }
}

void Ze08CH2OComponent::update() { 
    int16_t ppb = this->get_ze08_ch2o_ppb();
    if(ppb > -1) {
        this->formaldehyde_sensor_->publish_state(ppb);
    }
}

void Ze08CH2OComponent::loop() {

}

//checksum_calc = ~(buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]) + 1;
uint8_t Ze08CH2OComponent::check_checksum_(uint8_t* response, size_t len) {
    uint8_t crc = 0;
    // last byte of response is checksum, don't calculate it
    for(int i = 1; i < len - 1; i++) {
        crc += response[i];
    }
    return ~crc + 1;
}

int16_t Ze08CH2OComponent::get_ze08_ch2o_ppb() {
    uint8_t concentration_ugm3 = 0;
    uint8_t concentration_ppb = 0;
    uint8_t checksum_calc = 0;
    uint8_t response[9] = {0};

    this->write_array(ZE08_QUESTION, sizeof(ZE08_QUESTION));
    if (this->available() != sizeof(response)) {
        ESP_LOGE(TAG, "Bad response from ZE08-CH2O! received %d bytes.", this->available());
        return -1;
    }
    this->read_array(response, sizeof(response));
    // message of 9 bytes, same reading two units:
    //       ug/m3            ppb
    // st cm high low res res high low ch
    // ##   0   ##   1  ##   2  ##    3   ##   4   ##   5   ##      6     ##      7     ##   8   ##
    // ## Start ## Gas  ## Unit ## No dec ## Conc. ## Conc. ## Full Range ## Full Range ## Check ##
    // ## Byte  ## name ## ppb  ## -imal  ##  Hi   ##   Lo  ##     Hi     ##     Lo     ##  -sum ##
    concentration_ugm3 = (response[4] << 8) | response[5];
    concentration_ppb = (response[6] << 8) | response[7];
    ESP_LOGD(TAG, "Received %d %d, Unit %d.", concentration_ugm3, concentration_ppb, response[2]);

    checksum_calc = this->check_checksum_(response, sizeof(response));
    if (response[8] != checksum_calc) {
        ESP_LOGE(TAG, "Bad checksum from ZE08-CH20! received %d != %d.", response[8], checksum_calc);
    }

    return concentration_ppb;
}

void Ze08CH2OComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "ZE08-CH2O:");
#ifdef USE_SENSOR
    if (this->formaldehyde_sensor_ != nullptr)
        LOG_SENSOR("  ", "Formaldehyde", this->formaldehyde_sensor_);
#endif
    this->check_uart_settings(9600);
}

}  // namespace ze08_ch2o
}  // namespace esphome

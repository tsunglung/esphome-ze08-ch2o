#pragma once

#include <iomanip>
#include <map>
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace ze08_ch2o {

enum ZE08CH2OType {
  ZE08_MODE_QA = 0,
  ZE08_MODE_ACTIVE,
};

class Ze08CH2OComponent : public PollingComponent, public uart::UARTDevice {
  public:
  Ze08CH2OComponent() = default;

  #ifdef USE_SENSOR
    SUB_SENSOR(formaldehyde)
  #endif

    void set_mode(ZE08CH2OType mode) { mode_ = mode; }

    uint8_t check_checksum_(uint8_t* response, size_t len);
    int16_t get_ze08_ch2o_ppb();

    void dump_config() override;
    void setup() override;
    void update() override;
    void loop() override;

    float get_setup_priority() const override;

  protected:
    ZE08CH2OType mode_;
};

}  // namespace ze08_ch2o
}  // namespace esphome

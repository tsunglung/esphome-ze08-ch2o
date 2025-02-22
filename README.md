ESPHome Winsen ZE08-CH2O Formaldehyde Sensor Custom Component


example:

```
external_components:
  - source: github://tsunglung/esphome-ze08-ch2o@master
    components: [ ze08_ch2o ]

uart:
  - id: ze08_ch2o_uart
    tx_pin: 5
    rx_pin: 18
    baud_rate: 9600

ze08_ch2o:
  id: ze08_ch2o_id
  uart_id: ze08_ch2o_uart

sensor:
  - platform: ze08_ch2o
    formaldehyde:
      name: "Formaldehyde Sensor"
    mode: QA

```
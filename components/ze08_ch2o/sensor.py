import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_FORMALDEHYDE,
    CONF_MODE,
    DEVICE_CLASS_GAS,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_BILLION
)
from . import Ze08CH2OComponent, CONF_ZE08_CH2O_ID, ze08_ch2o_ns

TYPES = [
    CONF_FORMALDEHYDE,
]
ICON_FORMALDEHYDE = "mdi:chemical-weapon"

ZE08_MODE_QA = "QA"
ZE08_MODE_ACTIVE = "ACTIVE"

ZE08CH2OSensor = ze08_ch2o_ns.class_("ZE08CH2OSensor", sensor.Sensor)
ZE08CH2OType = ze08_ch2o_ns.enum("ZE08CH2OType")

ZE08_TYPES = {
    ZE08_MODE_QA: ZE08CH2OType.ZE08_MODE_QA,
    ZE08_MODE_ACTIVE: ZE08CH2OType.ZE08_MODE_ACTIVE
}

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_ZE08_CH2O_ID): cv.use_id(Ze08CH2OComponent),
            cv.Optional(CONF_FORMALDEHYDE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_BILLION,
                icon=ICON_FORMALDEHYDE,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_GAS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Required(CONF_MODE): cv.enum(ZE08_TYPES, upper=True),
        }
    ).extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = await cg.get_variable(config[CONF_ZE08_CH2O_ID])

    cg.add(var.set_mode(config[CONF_MODE]))

    for key in TYPES:
        if sensor_config := config.get(key):
            sens = await sensor.new_sensor(sensor_config)
            cg.add(getattr(var, f"set_{key}_sensor")(sens))

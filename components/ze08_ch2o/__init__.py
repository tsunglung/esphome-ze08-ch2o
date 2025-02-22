import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@tsunglung"]
MULTI_CONF = True
DEPENDENCIES = ["uart"]

CONF_ZE08_CH2O_ID = "ze08_ch2o_id"

ze08_ch2o_ns = cg.esphome_ns.namespace("ze08_ch2o")
Ze08CH2OComponent = ze08_ch2o_ns.class_(
    "Ze08CH2OComponent", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Ze08CH2OComponent)
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

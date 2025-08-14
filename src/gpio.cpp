#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "gpio.hpp"

LOG_MODULE_DECLARE(main);

#define SW0_NODE DT_ALIAS(sw0)
#define LED0_NODE DT_ALIAS(led0)

struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
struct gpio_callback button_cb_data;
struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);


namespace gpio {

void isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    LOG_INF("Button ISR");
}

GPIO::GPIO()
{
    GPIO::GPIO(isr);
}

GPIO::GPIO(gpio_callback_handler_t isr_handler)
{
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    gpio_pin_configure_dt(&button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&button_cb_data, isr_handler, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);
}

GPIO* GPIO::getAddress()
{
    return this;
}

void GPIO::setLed(bool state)
{
    gpio_pin_set_dt(&led, state);
}
}
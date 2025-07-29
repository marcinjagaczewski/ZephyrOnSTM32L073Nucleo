#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "gpio.hpp"

namespace gpio
{

Gpio::Gpio(gpio_callback_handler_t handlerGpio, k_work_handler_t handlerK_work)
{
    gpio_pin_configure_dt(&button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE); // reaguj na wciśnięcie i puszczenie

    gpio_init_callback(&button_cb_data, handlerGpio, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);

    k_work_init_delayable(&debounce_work, handlerK_work);
}


}
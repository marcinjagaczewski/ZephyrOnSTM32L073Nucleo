#pragma once

namespace gpio
{

#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

class Gpio
{
    public:
        Gpio(gpio_callback_handler_t handler, );

    private:
        static constexpr struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
        static constexpr struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
        static struct gpio_callback button_cb_data;
        static struct k_work_delayable debounce_work;
};

};







void gpioInit();
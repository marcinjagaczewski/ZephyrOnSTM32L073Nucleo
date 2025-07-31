#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <functional>

namespace gpio
{
    
    void isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

    class GPIO
    {
        public:
            GPIO();
            GPIO(gpio_callback_handler_t isr_handler);
            GPIO* getAddress();
            void setLed(bool state);
    };
};

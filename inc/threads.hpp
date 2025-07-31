#pragma once 
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <string>
#include "main.hpp"
#include "gpio.hpp"

#define STACK_SIZE 512
#define PRIORITY 5

namespace os
{
    
    void callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    void led_thread(void *arg1, void *arg2, void *arg3);
    void logger_thread(void *arg1, void *arg2, void *arg3);
    
    class Thread
    {
        public:
            Thread(gpio::GPIO &instance);
            
            void createLedThread();
            void createLogThread();

            // struct k_thread led_thread_data;
            // struct k_thread log_thread_data;
            gpio::GPIO &gpioInstance;
    };
}

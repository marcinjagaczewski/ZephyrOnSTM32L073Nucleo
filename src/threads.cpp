#include "threads.hpp"
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(main);

namespace os
{
struct k_thread led_thread_data;
struct k_thread log_thread_data;
    
K_THREAD_STACK_DEFINE(led_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(log_stack, STACK_SIZE);

K_MSGQ_DEFINE(button_msgq, sizeof(bool), 10, 4);
    
void callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    static bool pressed = false;
    
    pressed = !pressed;
    
    if (k_msgq_put(&button_msgq, &pressed, K_NO_WAIT) != 0) {
        LOG_WRN("Queue is full");
    }
}

Thread::Thread(gpio::GPIO &instance):gpioInstance(instance){
}

void led_thread(void *arg1, void *arg2, void *arg3)
{
    LOG_INF("led_thread start");
    bool button_is_pressed;
    gpio::GPIO *gc = (gpio::GPIO*)arg1;
    std::string text;

    while (1) {
        k_msgq_get(&button_msgq, &button_is_pressed, K_FOREVER);
        gc->setLed(button_is_pressed);
        text.clear();
        button_is_pressed == true ? text.append("Sw is pressed and LED turn ON") : text.append("Switch is pushed and LED turn OFF");
        LOG_INF("%s", text.c_str());
    }
}

void logger_thread(void *arg1, void *arg2, void *arg3)
{
    LOG_INF("logger_thread start");
    while (1) {
        LOG_INF("Secound thread is working");
        k_sleep(K_SECONDS(2));
    }
}

void Thread::createLedThread()
{
    k_thread_create(&led_thread_data, led_stack, STACK_SIZE,
                led_thread, gpioInstance.getAddress(), NULL, NULL,
                PRIORITY, 0, K_NO_WAIT);
}

void Thread::createLogThread()
{
    k_thread_create(&log_thread_data, log_stack, STACK_SIZE,
                logger_thread, NULL, NULL, NULL,
                PRIORITY, 0, K_NO_WAIT);
}

}
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <string>
#include "main.hpp"
#include "gpio.hpp"
#include "threads.hpp"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void)
{
    gpio::GPIO gpio(os::callback);
    os::Thread threads(gpio);
    threads.createLogThread();
    threads.createLedThread();

    LOG_INF("Threads are run");

    return 0;
}
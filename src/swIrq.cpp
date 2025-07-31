#include "swIrq.hpp"

using namespace appSw;

void SW::debounce_work_handler([[maybe_unused]]struct k_work *work)
{
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
}

void SW::button_callback([[maybe_unused]]const struct device *dev, [[maybe_unused]]struct gpio_callback *cb, [[maybe_unused]]uint32_t pins)
{
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_DISABLE);
    
    static bool pressed = 1;
    
    pressed = pressed ^ 1;

    // LOG_INF(">>button_callback: pressed state: %d", pressed);
    if (k_msgq_put(&button_msgq, &pressed, K_NO_WAIT) != 0) {
        LOG_WRN("Kolejka komunikatów jest pełna!");
    }
    k_work_schedule(&debounce_work, K_MSEC(100));               // opóźnione ponowne włączenie
    
}
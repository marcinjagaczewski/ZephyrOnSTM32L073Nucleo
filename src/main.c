#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)


#define STACK_SIZE 512
#define PRIORITY 5

K_MSGQ_DEFINE(button_msgq, sizeof(bool), 10, 4);

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct gpio_callback button_cb_data;
static struct k_work_delayable debounce_work;

void debounce_work_handler(struct k_work *work)
{
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
}

void button_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_DISABLE);
    
    static bool pressed = 1;
    
    pressed = pressed ^ 1;

    LOG_INF(">>button_callback: pressed state: %d", pressed);
    if (k_msgq_put(&button_msgq, &pressed, K_NO_WAIT) != 0) {
        LOG_WRN("Kolejka komunikatów jest pełna!");
    }
    k_work_schedule(&debounce_work, K_MSEC(100));               // opóźnione ponowne włączenie
    
}

// Wątek do migania LED
void led_thread(void *arg1, void *arg2, void *arg3)
{
    bool button_is_pressed;
    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Nie udało się skonfigurować LED");
        return;
    }

    while (1) {
        k_msgq_get(&button_msgq, &button_is_pressed, K_FOREVER);
         if (button_is_pressed) {
            LOG_INF("Przycisk wciśnięty -> Włączam LED");
            gpio_pin_set_dt(&led, 1); // Włącz LED
        } else {
            LOG_INF("Przycisk puszczony -> Wyłączam LED");
            gpio_pin_set_dt(&led, 0); // Wyłącz LED
        }
        // gpio_pin_toggle_dt(&led);
        // LOG_INF("LED");
        // k_sleep(K_SECONDS(1));
    }
}

// Wątek do logowania
void logger_thread(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        LOG_INF("Działa drugi wątek logujący");
        k_sleep(K_SECONDS(2));
    }
}

// Stosy i definicje wątków
K_THREAD_STACK_DEFINE(led_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(log_stack, STACK_SIZE);

static struct k_thread led_thread_data;
static struct k_thread log_thread_data;

void main(void)
{
    // Konfiguracja przycisku z przerwaniem
    gpio_pin_configure_dt(&button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE); // reaguj na wciśnięcie i puszczenie

    gpio_init_callback(&button_cb_data, button_callback, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);

    k_work_init_delayable(&debounce_work, debounce_work_handler);

    k_thread_create(&led_thread_data, led_stack, STACK_SIZE,
                    led_thread, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&log_thread_data, log_stack, STACK_SIZE,
                    logger_thread, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    LOG_INF("Wątki zostały uruchomione");
}
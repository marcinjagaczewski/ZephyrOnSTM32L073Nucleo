#pragma once

namespace appSw
{
    class SW
    {
        public:
            void debounce_work_handler([[maybe_unused]]struct k_work *work)
            void button_callback([[maybe_unused]]const struct device *dev, [[maybe_unused]]struct gpio_callback *cb, [[maybe_unused]]uint32_t pins);
        private:
            struct k_work_delayable debounce_work;
    };
}


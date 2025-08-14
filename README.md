This project is only template to launch Zephyr OS on stm32L073RZ nucleo

[!TIP]
cmd to build from zephyr project (with debug):

  west build -b nucleo_l073rz L073RZ/ -p always -DCMAKE_BUILD_TYPE=Debug

cmd to flash:

west flash

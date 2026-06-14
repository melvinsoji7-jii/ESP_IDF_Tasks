#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define LED_PIN 2 // define the GPIO pin number for the LED

void app_main(void)
{
    gpio_reset_pin(LED_PIN); // reset the GPIO pin to its default state
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); // set the GPIO pin as an output

    while(1){ // to run continuously
        gpio_set_level(LED_PIN, 1); // turn on led (sets voltage to high [3.3V])
        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds (1 second)

        gpio_set_level(LED_PIN, 0); // turn off led (sets voltage to low [0V])
        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds (1 second)
    }
}

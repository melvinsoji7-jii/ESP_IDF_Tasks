/* UART Loopback Test Application
   
   This application demonstrates UART communication with loopback verification.
   It sends test data and reads back responses to verify correct UART operation.
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"

// UART Configuration Constants
#define UART_PORT       UART_NUM_1
#define TX_PIN          GPIO_NUM_17
#define RX_PIN          GPIO_NUM_16
#define BUFFER_SIZE     1024
#define READ_TIMEOUT    1000    // milliseconds
#define LOOP_DELAY      2000    // milliseconds

static const char *TAG = "UART_LOOPBACK";

void app_main(void)
{
    // Configure UART settings structure
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install UART driver with specified buffer size
    uart_driver_install(UART_PORT, BUFFER_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Test message to send and verify
    const char *test_message = "ESP32_UART_TEST\r\n";

    ESP_LOGI(TAG, "UART Loopback test started");

    while (1)
    {
        // Send test message via UART
        uart_write_bytes(UART_PORT, test_message, strlen(test_message));

        // Prepare receive buffer
        uint8_t received_data[BUFFER_SIZE];

        // Read response with timeout
        int bytes_received = uart_read_bytes(
                                UART_PORT,
                                received_data,
                                BUFFER_SIZE - 1,
                                pdMS_TO_TICKS(READ_TIMEOUT));

        if (bytes_received > 0)
        {
            // Null-terminate received data
            received_data[bytes_received] = '\0';

            // Display sent and received data
            printf("Sent     : %s", test_message);
            printf("Received : %s\n", received_data);

            // Verify loopback integrity
            if (strcmp((char *)received_data, test_message) == 0)
            {
                ESP_LOGI(TAG, "✓ UART LOOPBACK TEST PASSED\n");
            }
            else
            {
                ESP_LOGW(TAG, "✗ UART LOOPBACK TEST FAILED\n");
            }
        }
        else
        {
            printf("No data received (timeout)\n\n");
        }

        // Delay before next test cycle
        vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY));
    }
}

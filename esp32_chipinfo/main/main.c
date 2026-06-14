#include <stdio.h> 
#include <inttypes.h> // Support for PRIu32 format macros
#include "sdkconfig.h" // Project configuration macros
#include "freertos/FreeRTOS.h" // FreeRTOS core definitions
#include "freertos/task.h" // FreeRTOS task utilities
#include "esp_chip_info.h" // Chip information API
#include "esp_flash.h" // Flash size API
#include "esp_system.h" // System functions like esp_restart

void app_main(void) 
{
    /* Print chip information */ 
    esp_chip_info_t chip_info; // Variable to store chip info of type esp_chip_info_t
    uint32_t flash_size; // Variable to store flash size in bytes of type uint32_t(unsigned 32-bit integer)
    esp_chip_info(&chip_info); // Fill chip_info with hardware details (esp_chip_info is a function that takes a pointer to esp_chip_info_t and stores it with the current chip's information)
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", // Print chip model and feature summary
           CONFIG_IDF_TARGET, // Target chip name from config
           chip_info.cores, // Number of CPU cores
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "", // Check for WiFi feature and print "WiFi/" if present
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "", // Check for Bluetooth feature 
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "", // Check for Bluetooth Low Energy feature (BLE) 
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : ""); // Check for IEEE 802.15.4 feature 
    unsigned major_rev = chip_info.revision / 100; 
    unsigned minor_rev = chip_info.revision % 100; 
    /* Extract major silicon revision number Example: if revision = 123 
    then major_rev = 1 (123 / 100) and minor_rev = 23 (123 % 100) */
    printf("silicon revision v%d.%d, ", major_rev, minor_rev); // Print silicon revision no.
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) { // Get flash size and check any errors
        printf("Get flash size failed"); // Print error msg
        return; // Exit the task early
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024), // Print flash size in MB
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external"); // Show whether flash is embedded or external

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size()); // Print minimum free heap size

    for (int i = 10; i >= 0; i--) { // Countdown before restart
        printf("Restarting in %d seconds...\n", i); // Print each countdown value
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait one second
    }
    printf("Restarting now.\n"); // Print final restart message
    fflush(stdout); // Flush output buffer
    esp_restart(); // Restart the ESP32
}


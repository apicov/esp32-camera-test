#include <esp_system.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "driver/gpio.h"



#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_client.h"


//#include "mqtt_client.h"
#include "private_data.h"
#include "camera_ctl.h"
#include "sd_card.h"


static const char *TAG = "WIFI_STA"; // Tag for logging

//static atomic_bool is_wifi_connected = false; // Atomic flag for Wi-Fi connection status

//static esp_mqtt_client_handle_t mqtt_client = NULL; // Global MQTT client handle


void mqtt_test(void *p);



extern "C" void app_main()
{
    ESP_LOGI(TAG, "application started");

    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);

    esp_err_t err;
    err = initi_sd_card();
    if (err != ESP_OK)
    {
        printf("err: %s\n", esp_err_to_name(err));
        return;
    }


    xTaskCreate(mqtt_test, "main", 4096, NULL, 5, NULL);
} // end of app_main   



void mqtt_test(void *p)
{
    CameraCtl cam;
    char photo_name[50];

    esp_err_t err;
    err = cam.init_camera();
    if (err != ESP_OK)
    {
        printf("err: %s\n", esp_err_to_name(err));
        return;
    }


    while(1)
    {
        gpio_set_level(GPIO_NUM_33, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_33, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        sprintf(photo_name, "/sdcard/pic_%li.jpg", (long int) cam.pic->timestamp.tv_sec);
        cam.capture_to_file(photo_name);
    }
}
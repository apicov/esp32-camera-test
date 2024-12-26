#include "camera_ctl.h"



esp_err_t CameraCtl::init_camera(void)
{
    camera_config_t camera_config = {
    CAM_PIN_PWDN,        // pin_pwdn
    CAM_PIN_RESET,       // pin_reset
    CAM_PIN_XCLK,        // pin_xclk
    CAM_PIN_SIOD,        // pin_sccb_sda
    CAM_PIN_SIOC,        // pin_sccb_scl
    CAM_PIN_D7,          // pin_d7
    CAM_PIN_D6,          // pin_d6
    CAM_PIN_D5,          // pin_d5
    CAM_PIN_D4,          // pin_d4
    CAM_PIN_D3,          // pin_d3
    CAM_PIN_D2,          // pin_d2
    CAM_PIN_D1,          // pin_d1
    CAM_PIN_D0,          // pin_d0
    CAM_PIN_VSYNC,       // pin_vsync
    CAM_PIN_HREF,        // pin_href
    CAM_PIN_PCLK,        // pin_pclk
    CONFIG_XCLK_FREQ,    // xclk_freq_hz
    LEDC_TIMER_0,        // ledc_timer
    LEDC_CHANNEL_0,      // ledc_channel
    PIXFORMAT_JPEG,      // pixel_format
    FRAMESIZE_QQVGA,      // frame_size
    12,                   // jpeg_quality
    1,                    // fb_count
    CAMERA_FB_IN_PSRAM,   // fb_location (or CAMERA_FB_IN_DRAM)
    CAMERA_GRAB_WHEN_EMPTY // grab_mode
    };

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        return err;
    }
    return ESP_OK;
}


void CameraCtl::capture(void *arg)
{    
    ESP_LOGI(CAM_TAG, "Starting Taking Picture!");

    camera_fb_t *pic = esp_camera_fb_get();
    
    esp_camera_fb_return(pic);
    //vTaskDelete(NULL);
    ESP_LOGI(CAM_TAG, "Finished Taking Picture!");
}



void CameraCtl::capture_to_file(char *fname)
{
    ESP_LOGI(CAM_TAG, "Starting Taking Picture!");


    pic = esp_camera_fb_get();


    FILE *file = fopen(fname, "w");
    if (file == NULL)
    {
        printf("err: fopen failed\n");
    }
    else
    {
        fwrite(pic->buf, 1, pic->len, file);
        fclose(file);
    }
    
    esp_camera_fb_return(pic);
    //vTaskDelete(NULL);
    ESP_LOGI(CAM_TAG, "Finished Taking Picture!");
}


void CameraCtl::free_buffer()
{
    esp_camera_fb_return(pic);
}

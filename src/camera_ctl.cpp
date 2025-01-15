#include "camera_ctl.h"

//uint8_t img[96*96];
uint8_t img_color[96 * 96 * 3]; // Allocate for color image (RGB)


// Resize color image using nearest-neighbor interpolation
void resizeColorImage(uint8_t *src, int srcWidth, int srcHeight, 
                      uint8_t *dst, int dstWidth, int dstHeight) {
    for (int y = 0; y < dstHeight; y++) {
        for (int x = 0; x < dstWidth; x++) {
            // Map destination coordinates to source coordinates
            int srcX = x * srcWidth / dstWidth;
            int srcY = y * srcHeight / dstHeight;

            // Calculate source index in 8-bit array
            int srcIndex = (srcY * srcWidth + srcX) * 2; // Each pixel is 2 bytes

            // Extract the RGB565 value
            uint16_t rgb565 = (src[srcIndex + 1] << 8) | src[srcIndex];

            // Extract RGB components from RGB565
            uint8_t r = (rgb565 >> 11) & 0x1F; // Extract red (5 bits)
            uint8_t g = (rgb565 >> 5) & 0x3F;  // Extract green (6 bits)
            uint8_t b = rgb565 & 0x1F;         // Extract blue (5 bits)

            // Scale components to 8-bit values
            r = r << 3; // Scale 5-bit red to 8 bits
            g = g << 2; // Scale 6-bit green to 8 bits
            b = b << 3; // Scale 5-bit blue to 8 bits

            // Write to the destination array (3 bytes per pixel in 24-bit RGB)
            int dstIndex = (y * dstWidth + x) * 3;
            dst[dstIndex] = r;     // Red
            dst[dstIndex + 1] = g; // Green
            dst[dstIndex + 2] = b; // Blue
        }
    }
}






/*

// Resize grayscale image using nearest-neighbor interpolation
void resizeGrayImage(uint8_t *src, int srcWidth, int srcHeight, 
                     uint8_t *dst, int dstWidth, int dstHeight) {
    for (int y = 0; y < dstHeight; y++) {
        for (int x = 0; x < dstWidth; x++) {
            // Map destination coordinates to source coordinates
            int srcX = x * srcWidth / dstWidth;
            int srcY = y * srcHeight / dstHeight;

            // Copy the closest pixel value
            dst[y * dstWidth + x] = src[srcY * srcWidth + srcX];
        }
    }
}




// Function to write BMP header
void writeBMPHeader(FILE *file, int width, int height) 
{
    uint32_t fileSize = 54 + (width * height); // Header size + pixel data
    uint16_t bfType = 0x4D42; // "BM" identifier
    uint32_t bfReserved = 0;
    uint32_t bfOffBits = 54; // Pixel data offset

    uint32_t biSize = 40; // Info header size
    int16_t biPlanes = 1;
    int16_t biBitCount = 8; // Grayscale
    uint32_t biCompression = 0; // No compression
    uint32_t biSizeImage = width * height; // Image size
    int32_t biXPelsPerMeter = 2835; // 72 DPI
    int32_t biYPelsPerMeter = 2835;
    uint32_t biClrUsed = 256; // 256 grayscale colors
    uint32_t biClrImportant = 256;

    // Write BMP file header
    fwrite(&bfType, 2, 1, file);
    fwrite(&fileSize, 4, 1, file);
    fwrite(&bfReserved, 4, 1, file);
    fwrite(&bfOffBits, 4, 1, file);

    // Write BMP info header
    fwrite(&biSize, 4, 1, file);
    fwrite(&width, 4, 1, file);
    fwrite(&height, 4, 1, file);
    fwrite(&biPlanes, 2, 1, file);
    fwrite(&biBitCount, 2, 1, file);
    fwrite(&biCompression, 4, 1, file);
    fwrite(&biSizeImage, 4, 1, file);
    fwrite(&biXPelsPerMeter, 4, 1, file);
    fwrite(&biYPelsPerMeter, 4, 1, file);
    fwrite(&biClrUsed, 4, 1, file);
    fwrite(&biClrImportant, 4, 1, file);

    // Write color palette for grayscale (0-255)
    for (int i = 0; i < 256; i++) {
        uint8_t color[4] = { (uint8_t)i, (uint8_t)i, (uint8_t)i, 0 };
        fwrite(color, 4, 1, file);
    }
}

*/

// Function to write BMP header for a 24-bit RGB image
void writeBMPHeader(FILE *file, int width, int height) {
    // Calculate row size (padded to a multiple of 4 bytes)
    int rowSize = (width * 3 + 3) & ~3;

    // Calculate file size
    uint32_t fileSize = 54 + rowSize * height;

    // BMP file header
    uint16_t bfType = 0x4D42;  // "BM" identifier
    uint32_t bfReserved = 0;
    uint32_t bfOffBits = 54;   // Pixel data offset (54 bytes for header)

    // BMP info header
    uint32_t biSize = 40;      // Info header size
    int16_t biPlanes = 1;
    int16_t biBitCount = 24;   // 24 bits per pixel (RGB)
    uint32_t biCompression = 0;  // No compression
    uint32_t biSizeImage = rowSize * height;  // Image size (including padding)
    int32_t biXPelsPerMeter = 2835; // 72 DPI
    int32_t biYPelsPerMeter = 2835; // 72 DPI
    uint32_t biClrUsed = 0;       // No color palette
    uint32_t biClrImportant = 0;  // All colors are important

    // Write BMP file header
    fwrite(&bfType, 2, 1, file);
    fwrite(&fileSize, 4, 1, file);
    fwrite(&bfReserved, 4, 1, file);
    fwrite(&bfOffBits, 4, 1, file);

    // Write BMP info header
    fwrite(&biSize, 4, 1, file);
    fwrite(&width, 4, 1, file);
    fwrite(&height, 4, 1, file);
    fwrite(&biPlanes, 2, 1, file);
    fwrite(&biBitCount, 2, 1, file);
    fwrite(&biCompression, 4, 1, file);
    fwrite(&biSizeImage, 4, 1, file);
    fwrite(&biXPelsPerMeter, 4, 1, file);
    fwrite(&biYPelsPerMeter, 4, 1, file);
    fwrite(&biClrUsed, 4, 1, file);
    fwrite(&biClrImportant, 4, 1, file);
}


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
    PIXFORMAT_RGB565,//PIXFORMAT_GRAYSCALE, //PIXFORMAT_JPEG,      // pixel_format
    FRAMESIZE_QQVGA,      // frame_size
    12,                   // jpeg_quality
    1,                    // fb_count
    CAMERA_FB_IN_PSRAM,   // fb_location (or CAMERA_FB_IN_DRAM)
    CAMERA_GRAB_WHEN_EMPTY // grab_mode
    };


    esp_err_t err = camera_xclk_init(20000000);
    if (err != ESP_OK)
    {
        return err;
    }

    err = esp_camera_init(&camera_config);
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


/*
void CameraCtl::capture_to_file(char *fname)
{
    ESP_LOGI(CAM_TAG, "Starting Taking Picture!");


    pic = esp_camera_fb_get();

    resizeGrayImage(pic->buf,
                    160, //srcWidth
                    120, //srcHeight 
                    img,  
                    96, //dstWidth 
                    96 );  //dstHeight


    if (pic->format == PIXFORMAT_GRAYSCALE) 
    {
        FILE *file = fopen(fname, "w");
        if (file) 
        {
            //writeBMPHeader(file, pic->width, pic->height);
            writeBMPHeader(file, 96, 96);


            for (int y = 96 - 1; y >= 0; y--) {
                fwrite(img + (y * 96), 1, 96, file);
            }

            // Write pixel data (bottom-to-top for BMP format)
            //for (int y = pic->height - 1; y >= 0; y--) {
                //fwrite(pic->buf + (y * pic->width), 1, pic->width, file);
           // }

            fclose(file);

        
            ESP_LOGI(CAM_TAG,"Image saved as BMP");
        } 
        else 
        {
            ESP_LOGI(CAM_TAG,"Failed to open file");
        }
    } 
    else 
    {
        ESP_LOGI(CAM_TAG,"Image format is not grayscale");
    }
    
    esp_camera_fb_return(pic);
    //vTaskDelete(NULL);
    ESP_LOGI(CAM_TAG, "Finished Taking Picture!");
}
*/


void CameraCtl::capture_to_file(char *fname) {
    ESP_LOGI(CAM_TAG, "Starting Taking Picture!");

    camera_fb_t *pic = esp_camera_fb_get();

    resizeColorImage(pic->buf, 160 /*srcWidth*/, 120 /*srcHeight*/, 
                     img_color, 96 /*dstWidth*/, 96 /*dstHeight*/);

    if (pic->format == PIXFORMAT_RGB565) {
        FILE *file = fopen(fname, "wb");
        if (file) {
            writeBMPHeader(file, 96, 96);

            // Write pixel data (bottom-to-top for BMP format)
            // Write pixel data with row padding
            uint8_t padding[3] = {0, 0, 0}; // Max padding size is 3 bytes
            int rowSize = (96 * 3 + 3) & ~3;
            for (int y = 96 - 1; y >= 0; y--) {
                fwrite(img_color + (y * 96 * 3), 1, 96 * 3, file);
                fwrite(padding, 1, rowSize - (96 * 3), file);
            }

            fclose(file);
            ESP_LOGI(CAM_TAG, "Image saved as BMP");
        } else {
            ESP_LOGI(CAM_TAG, "Failed to open file");
            esp_camera_fb_return(pic);
        }
    }

    esp_camera_fb_return(pic);
    ESP_LOGI(CAM_TAG, "Finished Taking Picture!");
}



void CameraCtl::free_buffer()
{
    esp_camera_fb_return(pic);
}


esp_err_t CameraCtl::camera_xclk_init(uint32_t freq_hz) {

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE, // High-speed mode
        .duty_resolution = LEDC_TIMER_1_BIT, // Minimal duty resolution for clock
        .timer_num = LEDC_TIMER_0,         // Use LEDC_TIMER_0
        .freq_hz = freq_hz,                // Set the desired frequency
        .clk_cfg = LEDC_AUTO_CLK           // Automatically select clock source
    };

    // Configure the LEDC timer
    esp_err_t err = ledc_timer_config(&ledc_timer);
    if (err != ESP_OK) {
        ESP_LOGE("camera_xclk", "LEDC timer config failed, rc=%d", err);
        return err;
    }

    // Configure the LEDC channel for XCLK pin
    ledc_channel_config_t ledc_channel = {
        .gpio_num = CAM_PIN_XCLK, // Replace with your XCLK GPIO number
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 1, // Minimal duty cycle for clock generation
        .hpoint = 0
    };

    err = ledc_channel_config(&ledc_channel);
    if (err != ESP_OK) {
        ESP_LOGE("camera_xclk", "LEDC channel config failed, rc=%d", err);
        return err;
    }

    return ESP_OK;
}

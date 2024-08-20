#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_log.h"

#include <esp_system.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// support IDF 5.x
#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

#include "esp_camera.h"

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

static const char *TAG = "camera_init";
static const char *TAG_1 = "camera_capture";

#if ESP_CAMERA_SUPPORTED 
static camera_config_t camera_config = { 
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QQVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

    .jpeg_quality = 50, //0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,

};
static esp_err_t init_camera(void){ 
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK){ 
        ESP_LOGE(TAG, "camera init failed");
        return err;
    }
    return ESP_OK;
}
#endif
void app_main(void)
{
    #if ESP_CAMERA_SUPPORTED
        if(ESP_OK != init_camera()){
            return;
        }
    
    sensor_t *s =esp_camera_sensor_get();
    s->set_gain_ctrl(s, 0);                       // auto gain off
    s->set_awb_gain(s, 1);                        // Auto White Balance enable (0 or 1)
    s->set_exposure_ctrl(s, 0);                   // auto exposure off
    s->set_brightness(s, 2);                     // (-2 to 2) - set brightness
    s->set_agc_gain(s, 10);          // set gain manually (0 - 30)
    s->set_aec_value(s, 500);

    esp_err_t err_nvs = nvs_flash_init();
    if (err_nvs != ESP_OK){ 
        ESP_LOGE(TAG_1, "Error (%s) initializing NVS!", esp_err_to_name(err_nvs));
        return;
    }
    nvs_handle_t my_handle; 
    err_nvs = nvs_open("storage", NVS_READWRITE, &my_handle);
    if(err_nvs != ESP_OK){ 
        ESP_LOGE(TAG_1, "error (%s) opening NVS Handle", esp_err_to_name(err_nvs));
        return;
    }

    int file_index = 0;
    err_nvs = nvs_get_i32(my_handle, "")



    #else
        ESP_LOGE(TAG, "CAMERA NOT SUPPORTED");
        return;
    #endif


}
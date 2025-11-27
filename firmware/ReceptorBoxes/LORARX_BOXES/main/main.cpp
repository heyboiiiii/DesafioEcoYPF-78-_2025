#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_littlefs.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_http_server.h"

// LoRa includes
#include "driver/spi_master.h"
#include "src/LoRa.h"

// LoRa pins
#define LORA_MISO 22
#define LORA_MOSI 23
#define LORA_SCK  15
#define LORA_SS   4
#define LORA_DIO0 21
#define LORA_RST  3

static const char *TAG = "MAIN";

// LoRa parameters
float tension = 0.0;
float corriente = 0.0;

// LittleFS configuration
static const esp_vfs_littlefs_conf_t littlefs_conf = {
    .base_path = "/littlefs",
    .partition_label = "littlefs",
    .format_if_mount_failed = true,
    .dont_mount = false
};

// ==================== HTTP HANDLER ====================
esp_err_t data_get_handler(httpd_req_t *req) {
    char buf[128];
    snprintf(buf, sizeof(buf), "{\"voltage\":%.2f,\"current\":%.2f}", tension, corriente);
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, buf, strlen(buf));
}

httpd_uri_t data_uri = {
    .uri       = "/data",
    .method    = HTTP_GET,
    .handler   = data_get_handler,
    .user_ctx  = NULL
};

// Simple file handler for static files
esp_err_t file_get_handler(httpd_req_t *req) {
    char filepath[1000];
    snprintf(filepath, sizeof(filepath), "/littlefs/%s", req->uri);

    FILE *f = fopen("index.html", "r");
    if (!f) {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    char buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, n);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0); // end response
    return ESP_OK;
}

httpd_uri_t file_uri = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = file_get_handler,
    .user_ctx  = NULL
};

// ==================== MAIN TASK ====================
extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize default WiFi AP network interface
    esp_netif_create_default_wifi_ap();

    // WiFi initialization
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set WiFi mode to AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // Configure AP
    wifi_config_t ap_config = {};
    strcpy((char*)ap_config.ap.ssid, "Monitoreo");
    strcpy((char*)ap_config.ap.password, "19791979");
    ap_config.ap.ssid_len = strlen("Monitoreo");
    ap_config.ap.max_connection = 4;
    ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Access Point started");

    // Initialize LittleFS
    ESP_ERROR_CHECK(esp_vfs_littlefs_register(&littlefs_conf));
    ESP_LOGI(TAG, "LittleFS mounted at %s", littlefs_conf.base_path);

    // Start HTTP server
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = nullptr;
    ESP_ERROR_CHECK(httpd_start(&server, &server_config));
    httpd_register_uri_handler(server, &data_uri);
    httpd_register_uri_handler(server, &file_uri);
    ESP_LOGI(TAG, "HTTP server started");

    // Initialize LoRa (example only)
    /*
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(433E6)) {
        ESP_LOGE(TAG, "LoRa init failed");
        while (1);
    }
    ESP_LOGI(TAG, "LoRa Receiver ready");
    */

    // Application main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

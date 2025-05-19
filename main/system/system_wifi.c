#include "system_wifi.h"

#define WIFI_SSID "CAKIRCA"  // 📌 Wi-Fi SSID
#define WIFI_PASS "qaz-wsx-edc-123" // 📌 Wi-Fi Şifresi

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI("WiFi", "Wi-Fi bağlantısı başarılı!");
    }
}

 void get_wifi_mac_address(){
    uint8_t mac[6];
    esp_err_t err = esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);  // Wi-Fi STA (istemci) interface için MAC adresi
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Wi-Fi MAC Adresi: %02X:%02X:%02X:%02X:%02X:%02X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        ESP_LOGE(TAG, "Wi-Fi MAC adresi okunamadı (err=%d)", err);
    }
}

void wifi_init() {
    ESP_ERROR_CHECK(nvs_flash_init());
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    //esp_wifi_set_channel(5, WIFI_SECOND_CHAN_NONE);

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    ESP_ERROR_CHECK( esp_wifi_set_ps(WIFI_PS_NONE));
    get_wifi_mac_address();
}

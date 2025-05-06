#include "driver/gpio.h"
#include "waveshare_rgb_lcd_port.h"
#include "ui/ui.h"
#include "system_wifi.h"
#include "system_time.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui_helpers.h"
#define PULSE_GPIO 6  // GPIO6'dan pulse okuyacağız
#define DEBOUNCE_TIME_MS 1500  // 1500ms içinde yalnızca bir sayım yapılabilir

volatile int pulse_count = 0;  // Pulse sayacı
static lv_timer_t *counter_timer; // LVGL label güncelleme için timer
static lv_timer_t *daily_timer;
static uint32_t last_interrupt_time = 0; // Son tetikleme zamanı (ms)

static bool timer_started = false;  // Timer’ın bir kez başlatıldığını takip edecek

// 📌 **Kesme Servis Fonksiyonu (ISR)**
void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t current_time = esp_log_timestamp(); // Mevcut zaman (ms)
    
    if ((current_time - last_interrupt_time) > DEBOUNCE_TIME_MS) {
        pulse_count++;  // **1500ms içinde yalnızca bir kez artır**
        last_interrupt_time = current_time; // Zaman damgasını güncelle
    }
}

// 📌 **LVGL Label Güncelleme Fonksiyonu**
void update_pulse_label(lv_timer_t * timer) {
    static char buffer[16];  
    sprintf(buffer, "%d", pulse_count);  // Sayıyı string'e çevir
    lv_label_set_text(ui_counterDataLabel, buffer);  // Label'ı güncelle
}
void daily_goal_update(){
    update_expected_work_label(ui_dailyGoalDataLabel, login_timestamp);
}

// 📌 **GPIO Kurulumu**
void init_pulse_counter() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE, // Yükselen kenarda tetiklen
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << PULSE_GPIO),
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_conf);

    // 📌 **Kesme Servisini Aktifleştir**
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PULSE_GPIO, gpio_isr_handler, NULL);

    ESP_LOGI("PULSE", "GPIO6 Pulse Sayacı Başlatıldı!");

    // 📌 **LVGL Label Güncelleme Timer'ı Başlat**
    counter_timer = lv_timer_create(update_pulse_label, 500, NULL);  // 500ms aralıklarla günceller
}

void daily_goal_update_init(){
    daily_timer = lv_timer_create(daily_goal_update, 500, NULL); 
    ESP_LOGI("UPDATE", "UPDATE DAİLY");
}


// 📌 **LVGL Timer Görevini Başlatmak İçin FreeRTOS Görevi**
void lvgl_timer_task(void *pvParameters) {
    while (1) {
        lvgl_port_lock(-1);  // LVGL portunu kilitle

        lv_timer_handler();  // Timer’ları çalıştır

        lvgl_port_unlock();  // Portu serbest bırak

        vTaskDelay(pdMS_TO_TICKS(5));  // 5ms bekle
    }
}

static void log_login_time_task(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        // 1) Epoch değerini yazdır
        ESP_LOGW(TAG, "login_timestamp (epoch): %lld", (long long)login_timestamp);

        // 2) İnsan okunur formata çevir ve yazdır
        struct tm tm_info;
        localtime_r(&login_timestamp, &tm_info);
        ESP_LOGW(TAG,
            "login_timestamp (human): %04d-%02d-%02d %02d:%02d:%02d",
            tm_info.tm_year + 1900,
            tm_info.tm_mon  + 1,
            tm_info.tm_mday,
            tm_info.tm_hour,
            tm_info.tm_min,
            tm_info.tm_sec
        );

        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 saniye bekle
    }
}

void app_main() {   
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    ESP_ERROR_CHECK(ret);
    wifi_init();  // 🌐 Wi-Fi başlat
    obtain_time(); // ⏰ NTP sunucusundan saat al

    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD
    ESP_LOGI(TAG, "Display LVGL demos");
    
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1)) {
        ui_init();  // UI'yi başlat
        start_time_update_task();  // Zaman güncelleme görevini başlat
        init_pulse_counter();  // Pulse sayacı başlat  
        daily_goal_update_init();
        lvgl_port_unlock();
    }
}

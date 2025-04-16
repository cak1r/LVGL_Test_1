/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "driver/gpio.h"
#include "waveshare_rgb_lcd_port.h"
#include "ui/ui.h"
#include "system_wifi.h"
#include "system_time.h"
#include "nvs_flash.h"

#define PULSE_GPIO 6  // GPIO6'dan pulse okuyacağız
#define DEBOUNCE_TIME_MS 1500  // 1500ms içinde yalnızca bir sayım yapılabilir

volatile int pulse_count = 0;  // Pulse sayacı
static lv_timer_t *counter_timer; // LVGL label güncelleme için timer
static uint32_t last_interrupt_time = 0; // Son tetikleme zamanı (ms)

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
    
    //ESP_LOGI("PULSE", "Pulse Count: %d", pulse_count);
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
    counter_timer = lv_timer_create(update_pulse_label, 500, NULL);
}


void app_main()
{   
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    ESP_ERROR_CHECK(ret);
    wifi_init();  // 🌐 Wi-Fi başlat
    obtain_time(); // ⏰ NTP sunucusundan saat al

    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 
    // wavesahre_rgb_lcd_bl_on();  //Turn on the screen backlight 
    // wavesahre_rgb_lcd_bl_off(); //Turn off the screen backlight 
    //lv_label_set_text(ui_counterDataLabel2,"deneme1");
    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1)) {
        // lv_demo_stress();
        //lv_demo_benchmark();
        // lv_demo_music();
        //lv_demo_widgets();
        //example_lvgl_demo_ui();
        // Release the mutex
        //lv_label_set_text(ui_counterDataLabel2,"deneme2");
        ui_init();
        
        
        start_time_update_task();
        //lv_label_set_text(ui_counterDataLabel2,"deneme3");
         // 📌 **GPIO'dan pulse okumayı başlat**
        init_pulse_counter();
        lvgl_port_unlock();
    }
    
}

#include "time_manager.h"
#include "ui/ui.c"
#include <time.h>

void time_sync_notification_cb(struct timeval *tv) {
    ESP_LOGI("NTP", "Saat senkronize edildi!");
}

void obtain_time() {
    ESP_LOGI("NTP", "Zaman senkronizasyonu başlatılıyor...");

    esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org"); // 🌍 Global NTP sunucusu
    esp_sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_init();

    // 📌 **GMT+3 Saat Dilimi Ayarı**
    setenv("TZ", "EET-3EEST,M3.5.0/3,M10.5.0/4", 1); 
    tzset(); // 📌 Değişiklikleri uygula

    // 📌 Güncellenene kadar bekleyelim
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count) {
        ESP_LOGI("NTP", "Saat bilgisi bekleniyor (%d/%d)...", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2020 - 1900)) {
        ESP_LOGE("NTP", "Zaman alınamadı!");
    } else {
        ESP_LOGI("NTP", "Güncellenen Saat: %d:%d:%d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }
}

void update_time_and_date(lv_timer_t * timer) {
    time_t now;
    struct tm timeinfo;
    char time_str[10];
    char date_str[20];

    time(&now);
    localtime_r(&now, &timeinfo);

    // 📌 Formatlı saat ve tarih oluştur
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    strftime(date_str, sizeof(date_str), "%d-%m-%y", &timeinfo);

    // 📌 LVGL label'ları güncelle
    lv_label_set_text(ui_timeLabel, time_str);
    lv_label_set_text(ui_dateLabel, date_str);
    
    ESP_LOGI("TIME", "Updated Time: %s | Date: %s", time_str, date_str);
}

void start_time_update_task() {
    lv_timer_create(update_time_and_date, 1000, NULL);
}

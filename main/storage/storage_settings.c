#include "storage_settings.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>
#include "esp_log.h"

#define STORAGE_NAMESPACE "settings"

void save_machine_name(const char *name) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_OK) {
        nvs_set_str(handle, "machine_name", name);
        nvs_commit(handle);
        nvs_close(handle);
        ESP_LOGI("SETTINGS", "Saved machine name: %s", name);
    }
}

void load_machine_name(char *buf, size_t buf_len) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &handle);
    if (err == ESP_OK) {
        size_t required_size;
        err = nvs_get_str(handle, "machine_name", NULL, &required_size);
        if (err == ESP_OK && required_size < buf_len) {
            nvs_get_str(handle, "machine_name", buf, &required_size);
        } else {
            strcpy(buf, "Makine");
        }
        nvs_close(handle);
    } else {
        strcpy(buf, "Makine");
    }
}

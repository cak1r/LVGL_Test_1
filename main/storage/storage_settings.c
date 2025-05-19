#include "storage_settings.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>
#include "esp_log.h"
#include "ui.h"

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
            // Assign the loaded machine name to the global variable
            strncpy(global_machine_name, buf, buf_len);  // Store it in the global variable
        } else {
            // If not found or there's an error, set default name
            strcpy(buf, "Makine");
            strncpy(global_machine_name, "Makine", buf_len);  // Store default name globally
        }
        nvs_close(handle);
    } else {
        // If error occurs, set default
        strcpy(buf, "Makine");
        strncpy(global_machine_name, "Makine", buf_len);  // Store default name globally
    }
}
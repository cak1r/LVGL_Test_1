#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_netif.h"
#define TAG1 "MAC_ADDRESS"

// Wi-Fi MAC adresini almak için:
void get_wifi_mac_address();
void wifi_init(); // Wi-Fi başlatma fonksiyonu

#endif // WIFI_MANAGER_H

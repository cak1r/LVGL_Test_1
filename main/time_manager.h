#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "esp_sntp.h"
#include "lvgl.h"

void obtain_time(); // NTP sunucusundan zaman al
void start_time_update_task(); // LVGL ekrana zamanı yazdır

#endif // TIME_MANAGER_H

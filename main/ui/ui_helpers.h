
#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"
#include <time.h>



//Function Prototypes
void update_count_label(lv_obj_t *label, int hour, int minute, int second);
void update_expected_work_label(lv_obj_t *label, time_t start_time);






#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

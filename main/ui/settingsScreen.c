#include "ui.h"

lv_obj_t *screen_settings;
static lv_obj_t *ta_machine_name;

static void back_btn_event_cb(lv_event_t *e) {
    lv_disp_load_scr(tileview);
}



void ui_screen_settings_init(void) {
    screen_settings = lv_obj_create(NULL);
    lv_obj_set_size(screen_settings, 800, 480);
    lv_obj_clear_flag(screen_settings, LV_OBJ_FLAG_SCROLLABLE);

    // Geri Butonu
    lv_obj_t *back_btn = lv_btn_create(screen_settings);
    lv_obj_set_size(back_btn, 100, 50);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(back_btn);
    lv_label_set_text(label, "Geri");
    lv_obj_center(label);

    // Makine Adı TextArea
    lv_obj_t *label_name = lv_label_create(screen_settings);
    lv_label_set_text(label_name, "Makine Adi:");
    lv_obj_align(label_name, LV_ALIGN_TOP_MID, 0, 80);

    ta_machine_name = lv_textarea_create(screen_settings);
    lv_obj_set_size(ta_machine_name, 300, 50);
    lv_obj_align_to(ta_machine_name, label_name, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_textarea_set_placeholder_text(ta_machine_name, "orn: Makine-1");
    



}

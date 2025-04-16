#include "ui.h"
#include "../storage/storage_settings.h"

lv_obj_t *screen_settings;
static lv_obj_t *ta_machine_name;

static void back_btn_event_cb(lv_event_t *e) {
    lv_disp_load_scr(tileview);
}

static void save_btn_event_cb(lv_event_t *e) {
    const char *text = lv_textarea_get_text(ta_machine_name);
    save_machine_name(text);
    lv_disp_load_scr(tileview);  // Kaydettikten sonra çık
}

static lv_obj_t *kb = NULL;

static void kb_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *keyboard = lv_event_get_target(e);

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_del(keyboard);  // klavyeyi sil
        kb = NULL;
    }
}

static void ta_event_cb(lv_event_t *e) {
    if (kb == NULL) {
        kb = lv_keyboard_create(screen_settings);
        lv_keyboard_set_textarea(kb, ta_machine_name);
        lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL, NULL);

    }
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
    lv_label_set_text(lv_label_create(back_btn), "Geri");

    // Makine Adı Başlığı
    lv_obj_t *label_name = lv_label_create(screen_settings);
    lv_label_set_text(label_name, "Makine Adi:");
    lv_obj_align(label_name, LV_ALIGN_TOP_MID, 0, 80);

    // TextArea
    ta_machine_name = lv_textarea_create(screen_settings);
    lv_obj_set_size(ta_machine_name, 300, 50);
    lv_obj_align_to(ta_machine_name, label_name, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_textarea_set_placeholder_text(ta_machine_name, "orn: Makine-1");
    lv_obj_add_event_cb(ta_machine_name, ta_event_cb, LV_EVENT_FOCUSED, NULL);

    // NVS'den yükle
    char buf[64];
    load_machine_name(buf, sizeof(buf));
    lv_textarea_set_text(ta_machine_name, buf);

    // Kaydet Butonu
    lv_obj_t *save_btn = lv_btn_create(screen_settings);
    lv_obj_set_size(save_btn, 120, 50);
    lv_obj_align(save_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(save_btn, save_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text(lv_label_create(save_btn), "Kaydet");
}

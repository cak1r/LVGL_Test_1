#include "ui.h"

typedef struct {
    int id;
    const char *name;
} Bant;


static const Bant bant_list[] = {
    {1, "Bant 1"}, {2, "Bant 2"}, {3, "Bant 3"},
    {4, "Bant 4"}, {5, "Bant 5"}, {6, "Bant 6"},
};

static void bant_btn_event_cb(lv_event_t *e) {
    selected_bant_id = (int)(intptr_t)lv_event_get_user_data(e);
    ui_screen_order_select_init();  // Sonraki ekranı başlat
    lv_disp_load_scr(screen_order);
}

void ui_screen_bant_select_init(void) {
    screen_bant = lv_obj_create(NULL);
    lv_obj_set_size(screen_bant, 800, 480);

    lv_obj_t *label = lv_label_create(screen_bant);
    lv_label_set_text(label, "Bant Secin");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    for (int i = 0; i < sizeof(bant_list) / sizeof(Bant); i++) {
        lv_obj_t *btn = lv_btn_create(screen_bant);
        lv_obj_set_size(btn, 250, 40);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 70 + i * 50);
        lv_obj_add_event_cb(btn, bant_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)bant_list[i].id);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, bant_list[i].name);
        lv_obj_center(lbl);
    }
}

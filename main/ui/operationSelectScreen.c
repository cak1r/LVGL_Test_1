#include "ui.h"

typedef struct {
    int id;
    const char *name;
    int order_id;
} Operation;

static const Operation operation_list[] = {
    {1, "Kesim", 101}, {2, "Montaj", 101},
    {3, "Test", 201}, {4, "Paketleme", 601},
    {5, "Etiketleme", 601},
};

static void operation_btn_event_cb(lv_event_t *e) {
    // En son operasyon seçildi -> Ana ekrana geç
    lv_disp_load_scr(ui_MainScreen);
}

static void back_to_order_cb(lv_event_t *e) {
    lv_disp_load_scr(screen_order);
}

void ui_screen_operation_select_init(void) {
    screen_operation = lv_obj_create(NULL);
    lv_obj_set_size(screen_operation, 800, 480);

    lv_obj_t *label = lv_label_create(screen_operation);
    lv_label_set_text(label, "Operasyon Seçin");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    int y = 70;
    for (int i = 0; i < sizeof(operation_list) / sizeof(Operation); i++) {
        if (operation_list[i].order_id != selected_order_id) continue;

        lv_obj_t *btn = lv_btn_create(screen_operation);
        lv_obj_set_size(btn, 250, 40);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, y);
        lv_obj_add_event_cb(btn, operation_btn_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, operation_list[i].name);
        lv_obj_center(lbl);
        y += 50;
    }

    lv_obj_t *back_btn = lv_btn_create(screen_operation);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(back_btn, back_to_order_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text(lv_label_create(back_btn), "Geri");
}

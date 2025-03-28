#include "ui.h"

typedef struct {
    int id;
    const char *code;
    int bant_id;
} Order;

static const Order order_list[] = {
    {101, "Order 101", 1}, {102, "Order 102", 1},
    {201, "Order 201", 2}, {202, "Order 202", 2},
    {601, "Order 601", 6},
};

static void order_btn_event_cb(lv_event_t *e) {
    selected_order_id = (int)(intptr_t)lv_event_get_user_data(e);
    ui_screen_operation_select_init();
    lv_disp_load_scr(screen_operation);
}

static void back_to_bant_cb(lv_event_t *e) {
    lv_disp_load_scr(screen_bant);
}

void ui_screen_order_select_init(void) {
    screen_order = lv_obj_create(NULL);
    lv_obj_set_size(screen_order, 800, 480);

    lv_obj_t *label = lv_label_create(screen_order);
    lv_label_set_text(label, "Order Seçin");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    int y = 70;
    for (int i = 0; i < sizeof(order_list) / sizeof(Order); i++) {
        if (order_list[i].bant_id != selected_bant_id) continue;

        lv_obj_t *btn = lv_btn_create(screen_order);
        lv_obj_set_size(btn, 250, 40);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, y);
        lv_obj_add_event_cb(btn, order_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)order_list[i].id);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, order_list[i].code);
        lv_obj_center(lbl);
        y += 50;
    }

    lv_obj_t *back_btn = lv_btn_create(screen_order);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(back_btn, back_to_bant_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text(lv_label_create(back_btn), "Geri");
}

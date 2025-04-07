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
    {101, "Order 101", 1}, {102, "Order 102", 1},
    {101, "Order 101", 1}, {102, "Order 102", 1},
    {101, "Order 101", 1}, {102, "Order 102", 1},
    {101, "Order 101", 1}, {102, "Order 102", 1},
    {101, "Order 101", 1}, {102, "Order 102", 1},
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

    // Başlık
    lv_obj_t *label = lv_label_create(screen_order);
    lv_label_set_text(label, "Order Secin");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    // Renk listesi
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_ORANGE),
        lv_palette_main(LV_PALETTE_PURPLE),
        lv_palette_main(LV_PALETTE_CYAN),
        lv_palette_main(LV_PALETTE_PINK),
        lv_palette_main(LV_PALETTE_YELLOW),
        lv_palette_main(LV_PALETTE_GREY)
    };

    // Flex container
    lv_obj_t *cont = lv_obj_create(screen_order);
    lv_obj_set_size(cont, 800, 400);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(cont, 20, 0);
    lv_obj_set_style_pad_column(cont, 20, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0); // Şeffaf arka plan

    // Filtrelenmiş sipariş butonları
    int visible_index = 0;
    for (int i = 0; i < sizeof(order_list) / sizeof(Order); i++) {
        if (order_list[i].bant_id != selected_bant_id) continue;

        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 345, 80);  // 2 sütunlu
        lv_obj_set_style_bg_color(btn, colors[visible_index % (sizeof(colors) / sizeof(lv_color_t))], LV_PART_MAIN);
        lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, order_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)order_list[i].id);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, order_list[i].code);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(lbl);

        visible_index++;
    }

    // Geri Butonu
    lv_obj_t *back_btn = lv_btn_create(screen_order);
    lv_obj_set_size(back_btn, 110, 50);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_bant_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text(lv_label_create(back_btn), "Geri");
    lv_obj_set_style_text_font(back_btn, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_disp_load_scr(screen_order);
}

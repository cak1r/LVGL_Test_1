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
    lv_disp_load_scr(tileview);
}

static void back_to_order_cb(lv_event_t *e) {
    lv_disp_load_scr(screen_order);
}

void ui_screen_operation_select_init(void) {
    screen_operation = lv_obj_create(NULL);
    lv_obj_set_size(screen_operation, 800, 480);

    // Başlık
    lv_obj_t *label = lv_label_create(screen_operation);
    lv_label_set_text(label, "Operasyon Secin");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    // Renk listesi
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_ORANGE),
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_TEAL),
        lv_palette_main(LV_PALETTE_PURPLE)
    };

    // Flex container
    lv_obj_t *cont = lv_obj_create(screen_operation);
    lv_obj_set_size(cont, 800, 400);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(cont, 20, 0);
    lv_obj_set_style_pad_column(cont, 20, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0); // Arka plan şeffaf

    // Operasyon butonları
    int visible_index = 0;
    for (int i = 0; i < sizeof(operation_list) / sizeof(Operation); i++) {
        if (operation_list[i].order_id != selected_order_id) continue;

        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 345, 80);
        lv_obj_set_style_bg_color(btn, colors[visible_index % (sizeof(colors) / sizeof(lv_color_t))], LV_PART_MAIN);
        lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, operation_btn_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, operation_list[i].name);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(lbl);

        visible_index++;
    }

    // Geri butonu
    lv_obj_t *back_btn = lv_btn_create(screen_operation);
    lv_obj_set_size(back_btn, 110, 50);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_order_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text(lv_label_create(back_btn), "Geri");
    lv_obj_set_style_text_font(back_btn, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Ekranı göster
    lv_disp_load_scr(screen_operation);
}

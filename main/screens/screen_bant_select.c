#include "ui.h"
#include "system_http.h" 
static void bant_btn_event_cb(lv_event_t *e) {
    selected_bant_id = (int)(intptr_t)lv_event_get_user_data(e);

    ui_screen_order_select_init();  // Yeni ekran oluştur
    if (screen_order) {
        lv_disp_load_scr(screen_order);     // Eski ekran otomatik kaldırılır
    }
}


void ui_screen_bant_select_init(void) {
    screen_bant = lv_obj_create(NULL);
    lv_obj_set_size(screen_bant, 800, 480);

    lv_obj_t *label = lv_label_create(screen_bant);
    lv_label_set_text(label, "Bant Secin");
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
    lv_obj_t *cont = lv_obj_create(screen_bant);
    lv_obj_set_size(cont, 800, 400);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 30);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(cont, 20, 0);
    lv_obj_set_style_pad_column(cont, 20, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);

    for (int i = 0; i < bant_count; i++) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 345, 80); // genişlik 2 sütuna göre
        lv_obj_set_style_bg_color(btn, colors[i % (sizeof(colors) / sizeof(lv_color_t))], LV_PART_MAIN);
        lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(btn, bant_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)bant_list[i].bant_id);
    
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, bant_list[i].bant_tanim);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(lbl);
    }
    
}

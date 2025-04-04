#include "ui.h"




void ui_tileTestScreen_init(void) {
    tileview_test = lv_tileview_create(NULL);
    lv_obj_set_size(tileview, 800, 480);

    // Ana ekran tile'ı
    tile_main_test = lv_tileview_add_tile(tileview_test, 0, 0, LV_DIR_BOTTOM);
    lv_obj_t *label_main = lv_label_create(tile_main_test);
    lv_label_set_text(label_main, "Ana Ekran");
    lv_obj_center(label_main);

}

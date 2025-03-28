idf_component_register(SRCS 
    "ui/ui.c" 
    "waveshare_rgb_lcd_port.c" 
    "main.c" 
    "lvgl_port.c" 
    "ui/ui_img_logout_png.c"
    "ui/ui_img_setting_png.c"
    "ui/ui_img_973080792.c"
    "ui/ui_MainScreen.c"
    "ui/ui_LoadingScreen.c"
    "ui/bantSelectScreen.c"
    "ui/orderSelectScreen.c"
    "ui/operationSelectScreen.c"
    "ui/ui_loginScreen.c"
    "ui/ui_helpers.c"
    "ui/ui_events.c"  # <-- BU SATIR BURADA OLMALI
    "wifi_manager.c"
    "time_manager.c"
    "ui/custom_fonts/lv_font_tr_16.c"
    "ui/custom_fonts/lv_font_tr_20.c"
    "ui/custom_fonts/lv_font_tr_24.c"
    "ui/custom_fonts/lv_font_tr_30.c"
    "ui/custom_fonts/lv_font_tr_36.c"
    "ui/custom_fonts/lv_font_tr_48.c"
    "ui/custom_fonts/lv_font_tr_60.c"
    "ui/custom_fonts/lv_font_tr_72.c"
    "ui/custom_fonts/lv_font_tr_96.c"
    "ui/custom_fonts/lv_font_tr_120.c"   
)


idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)


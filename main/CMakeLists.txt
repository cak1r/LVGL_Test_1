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
    "ui/ui_helpers.c"
    "ui/ui_events.c"  # <-- BU SATIR BURADA OLMALI
)


idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)

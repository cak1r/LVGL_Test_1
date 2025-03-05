idf_component_register(
    SRCS "ui/ui.c" "waveshare_rgb_lcd_port.c" "main.c" "lvgl_port.c" "ui/ui_LoadingScreen.c"
    "ui/ui_MainScreen.c" "ui/ui_helpers.c"   "ui/ui_img_973080792.c" "ui/montserrat_custom_100_8bbp.c" )

idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)

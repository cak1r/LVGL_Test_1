SET(SOURCES 
    ui_loginScreen.c
    ui_LoadingScreen.c
    ui_MainScreen.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_973080792.c
    montserrat_custom_100_8bbp.c
    images/ui_img_973080792.c
    images/ui_img_logout_png.c
    images/ui_img_setting_png.c
    images/ui_img_441374382.c
    custom_fonts/lv_font_tr_16.c
    custom_fonts/lv_font_tr_20.c
    custom_fonts/lv_font_tr_24.c
    custom_fonts/lv_font_tr_30.c
    custom_fonts/lv_font_tr_36.c
    custom_fonts/lv_font_tr_48.c
    custom_fonts/lv_font_tr_60.c
    custom_fonts/lv_font_tr_72.c
    custom_fonts/lv_font_tr_96.c
    custom_fonts/lv_font_tr_120.c
    ui/bantSelectScreen.c
    ui/orderSelectScreen.c
    ui/operationSelectScreen.c
    ui/settingsScreen.c
)

add_library(ui ${SOURCES})


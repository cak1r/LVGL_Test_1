
idf_component_register(
    SRCS
        # Main Folder
        main.c
        waveshare_rgb_lcd_port.c
        
        #ping
        #ping/ping_test.c

        # Ekranlar (screens)
        screens/screen_login.c
        screens/screen_bant_select.c
        screens/screen_order_select.c
        screens/screen_operation.c
        screens/screen_main.c
        screens/screen_settings.c
        screens/screen_loading.c

        # Depolama işlemleri (storage)
        storage/storage_settings.c

        # Sistem yöneticileri (system)
        system/system_wifi.c
        system/system_time.c
        system/system_http.c

        # UI temel ve yardımcı dosyalar
        ui/ui.c
        ui/ui_helpers.c
        ui/ui_events.c
        ui/ui_tileTestScreen.c
        lvgl_port.c

        # Assets
        assets/bg_image.c
        assets/ui_img_logout_png.c
        assets/ui_img_setting_png.c
        assets/ui_img_973080792.c

    INCLUDE_DIRS
        .
        ui
        screens
        storage
        system  
        assets
)
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)      

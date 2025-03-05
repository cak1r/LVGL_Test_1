#include "ui.h"
#include <stdio.h> // sprintf için

LV_FONT_DECLARE(lv_font_montserrat_120);

int counter = 0; // Sayacı tutan global değişken
lv_obj_t * ui_arc; // Çeyrek daire progress bar için global değişken

// **BÜYÜK LABEL OLUŞTURMA FONKSİYONU**
void create_big_label(lv_obj_t * parent) {
    lv_obj_t * label = lv_label_create(parent);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, LV_SIZE_CONTENT);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -150); // Yukarı hizala
    
    lv_label_set_text(label, "HELLO WORLD!"); // Büyük metin
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT); // Siyah yazı
    lv_obj_set_style_text_font(label, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT); // **Özel büyük font kullan**
}

// **Butona basıldığında çalışacak fonksiyon**
void counter_btn_event_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e); // Hangi butona basıldığını al

    if(btn == ui_incBtn && counter < 100) counter++;  // "+" butonuna basılırsa artır
    else if(btn == ui_decBtn && counter > 0) counter--; // "-" butonuna basılırsa azalt

    static char buffer[10];  // Sayıyı string'e çevirmek için
    sprintf(buffer, "%d", counter);  // Sayıyı string olarak oluştur
    lv_label_set_text(ui_counterData, buffer); // Label içeriğini güncelle

    // **Çeyrek Daire Progress Bar'ı Güncelle**
    lv_arc_set_value(ui_arc, counter);
}

// **Ekran başlatma fonksiyonunu güncelle**
void ui_MainScreen_screen_init(void)
{
    ui_MainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MainScreen, LV_OBJ_FLAG_SCROLLABLE);  

    // **SAYAÇ LABEL**
    ui_counterLabel = lv_label_create(ui_MainScreen);
    lv_obj_set_width(ui_counterLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_counterLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_counterLabel, -1);
    lv_obj_set_y(ui_counterLabel, -187);
    lv_obj_set_align(ui_counterLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_counterLabel, "COUNTER");
    lv_obj_set_style_text_font(ui_counterLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // **DEĞER LABEL (Sayacı gösterir)**
    ui_counterData = lv_label_create(ui_MainScreen);
    lv_obj_set_width(ui_counterData, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_counterData, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_counterData, 14);
    lv_obj_set_y(ui_counterData, -96);
    lv_obj_set_align(ui_counterData, LV_ALIGN_CENTER);
    lv_label_set_text(ui_counterData, "0"); // Başlangıç değeri 0
    lv_obj_set_style_text_font(ui_counterData, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // **ÇEYREK DAİRE (ARC) PROGRESS BAR**
    ui_arc = lv_arc_create(ui_MainScreen);
    lv_obj_set_size(ui_arc, 150, 150);  // Boyut
    lv_obj_align(ui_arc, LV_ALIGN_CENTER, 0, 50);  // Ortala
    lv_arc_set_range(ui_arc, 0, 100); // 0-100 arası değerler
    lv_arc_set_bg_angles(ui_arc, 135, 405); // Çeyrek daire (135° ile 405° arasında)
    lv_arc_set_value(ui_arc, 0); // Başlangıç değeri
    lv_obj_set_style_arc_width(ui_arc, 10, LV_PART_MAIN); // Çizgi kalınlığı
    lv_obj_set_style_arc_color(ui_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR); // Renk
    lv_obj_remove_style(ui_arc, NULL, LV_PART_KNOB); // **Mavi noktayı kaldır**

    // **ARTTIRMA BUTONU (+)**
    ui_incBtn = lv_btn_create(ui_MainScreen);
    lv_obj_set_width(ui_incBtn, 154);
    lv_obj_set_height(ui_incBtn, 81);
    lv_obj_set_x(ui_incBtn, -188);
    lv_obj_set_y(ui_incBtn, 37);
    lv_obj_set_align(ui_incBtn, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_incBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_incBtn, LV_OBJ_FLAG_SCROLLABLE);

    ui_Label4 = lv_label_create(ui_incBtn);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "+");
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // **AZALTMA BUTONU (-)**
    ui_decBtn = lv_btn_create(ui_MainScreen);
    lv_obj_set_width(ui_decBtn, 154);
    lv_obj_set_height(ui_decBtn, 81);
    lv_obj_set_x(ui_decBtn, 148);
    lv_obj_set_y(ui_decBtn, 34);
    lv_obj_set_align(ui_decBtn, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_decBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_decBtn, LV_OBJ_FLAG_SCROLLABLE);

    ui_Label3 = lv_label_create(ui_decBtn);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_Label3, 0);
    lv_obj_set_y(ui_Label3, -2);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "-");
    lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    // **Butonlara event (callback) ekle**
    lv_obj_add_event_cb(ui_incBtn, counter_btn_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_decBtn, counter_btn_event_handler, LV_EVENT_CLICKED, NULL);

    // **HELLO WORLD LABEL'İ OLUŞTUR**
    create_big_label(ui_MainScreen);
}

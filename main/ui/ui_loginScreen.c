#include "ui.h"
#include <string.h>
#include "custom_fonts/custom_fonts.h"
lv_obj_t *ta_user = NULL;
lv_obj_t *ta_pass = NULL;

extern const lv_font_t lv_font_montserrat_20;
extern lv_obj_t * screen_bant;

#define MAX_USERS 10
static const char *user_list[MAX_USERS] = {
    "admin", "ahmet", "ayse", "ali", "arda", "berat", "berke", "burak", "yusufreis", "yusufbaba"
};

LV_IMG_DECLARE(bg_image); 

// === GLOBAL OBJE TANIMLARI ===
static lv_obj_t *keyboard;
static lv_obj_t *scrollable_cont;
static lv_obj_t *listbox;

// === KLAVYE CALLBACK ===
static void kb_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_user_data(e);

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_del(keyboard);
        keyboard = NULL;
    }
}

// === USERNAME TIKLANINCA KLAVYE AÇILSIN ===
static void ta_event_cb(lv_event_t * e) {
    lv_obj_t * ta = lv_event_get_target(e);

    if (keyboard == NULL) {
        keyboard = lv_keyboard_create(ui_loginScreen);
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_add_event_cb(keyboard, kb_event_cb, LV_EVENT_ALL, ta);
        lv_obj_set_style_text_font(keyboard, &lv_font_montserrat_20, LV_PART_ITEMS | LV_STATE_DEFAULT);
    } else {
        lv_keyboard_set_textarea(keyboard, ta);
    }

    lv_obj_scroll_to_view(ta, LV_ANIM_ON);
}

// === LISTE ITEM SEÇİMİ ===
static void list_item_event_cb(lv_event_t * e) {
    lv_obj_t *btn = lv_event_get_target(e);
    const char *selected = lv_list_get_btn_text(listbox, btn);
    lv_textarea_set_text(ta_user, selected);
    lv_obj_add_flag(listbox, LV_OBJ_FLAG_HIDDEN);
}

// === KULLANICI ADI GİRİŞİ — autocomplete ===
static void user_input_event_cb(lv_event_t *e) {
    lv_obj_t *ta = lv_event_get_target(e);
    const char *input = lv_textarea_get_text(ta);

    if (strlen(input) < 2) {
        lv_obj_add_flag(listbox, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(listbox, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clean(listbox);

    for (int i = 0; i < MAX_USERS; i++) {
        if (strncasecmp(user_list[i], input, strlen(input)) == 0) {
            lv_obj_t *btn = lv_list_add_btn(listbox, NULL, user_list[i]);
            lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    lv_obj_align_to(listbox, ta_user, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_move_foreground(listbox);
}

// === GİRİŞ BUTONU ===
static void login_btn_event_cb(lv_event_t * e) {
    const char * user = lv_textarea_get_text(ta_user);
    const char * pass = lv_textarea_get_text(ta_pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        lv_disp_load_scr(screen_bant); 
    } else {
        lv_obj_t *msg = lv_label_create(scrollable_cont);
        lv_label_set_text(msg, "Hatalı giriş!");
        lv_obj_align(msg, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_textarea_set_text(ta_pass, ""); // şifreyi sil
    }
}

// === EKRANI OLUŞTUR ===
void ui_loginScreen_screen_init(void)
{
    ui_loginScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_loginScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_loginScreen, 800, 480);

    lv_obj_t *bg = lv_img_create(ui_loginScreen);
    lv_img_set_src(bg, &bg_image);
    lv_obj_set_size(bg, 800, 480);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_move_background(bg);  // Tüm objelerin arkasına al


    scrollable_cont = lv_obj_create(ui_loginScreen);
    lv_obj_set_size(scrollable_cont, 800, 600);
    lv_obj_align(scrollable_cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_scroll_dir(scrollable_cont, LV_DIR_VER);
    lv_obj_add_flag(scrollable_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(scrollable_cont, LV_OPA_0, 0);
    lv_obj_set_style_border_width(scrollable_cont, 0, 0);

    // === Username Input (textarea)
    ta_user = lv_textarea_create(scrollable_cont);
    lv_obj_set_size(ta_user, 300, 60);
    lv_obj_align(ta_user, LV_ALIGN_TOP_MID, 0, 60);
    lv_textarea_set_placeholder_text(ta_user, "Username");
    lv_obj_add_event_cb(ta_user, ta_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ta_user, user_input_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_style_text_font(ta_user, &lv_font_montserrat_20, 0);

    // === Liste: autocomplete önerileri
    listbox = lv_list_create(scrollable_cont);
    lv_obj_set_size(listbox, 300, 120);
    lv_obj_add_flag(listbox, LV_OBJ_FLAG_HIDDEN);  // başlangıçta görünmesin
    lv_obj_align_to(listbox, ta_user, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    // === Şifre Girişi
    ta_pass = lv_textarea_create(scrollable_cont);
    lv_obj_set_size(ta_pass, 300, 60);
    lv_obj_align(ta_pass, LV_ALIGN_TOP_MID, 0, 140);
    lv_textarea_set_placeholder_text(ta_pass, "Password");
    lv_textarea_set_password_mode(ta_pass, true);
    lv_obj_add_event_cb(ta_pass, ta_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_style_text_font(ta_pass, &lv_font_montserrat_20, 0);

    // === Giriş Butonu
    lv_obj_t * btn = lv_btn_create(scrollable_cont);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 230);
    lv_obj_add_event_cb(btn, login_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Login");
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
}

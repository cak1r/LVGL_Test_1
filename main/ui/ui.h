// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include <time.h>

#include "ui_helpers.h"
#include "ui_events.h"
#include "esp_log.h"

#define MAX_BANT_COUNT 50
#define MAX_ORDER_COUNT 50
#define MAX_ORDER_CODE_LENGTH 64
#define MAX_OPERATION_COUNT 50
#define MAX_OPERATION_NAME_LENGTH 64

typedef struct {
    int id;
    const char *name;
    int order_id;
    int unit_time;  // Eklediğimiz yeni üye
    int cut_count;  // Eklediğimiz yeni üye
    int unit_time_tol;  // Add the new field

} Operation;

typedef struct {
    int bant_id;
    char bant_tanim[64];
} Bant;

typedef struct {
    int id;
    char code[MAX_ORDER_CODE_LENGTH];
    int bant_id;
} Order;

extern Order order_list[MAX_ORDER_COUNT];
extern int order_count;

extern Bant bant_list[MAX_BANT_COUNT];
extern int bant_count;
extern int selected_bant_id;
//update daily goal variable
extern int expected;
extern long counter;
extern Operation operation_list[MAX_OPERATION_COUNT];
extern int operation_count;
extern int current_logged_user_id;
extern int selected_cut_count;
extern int selected_unit_time;
extern int selected_unit_time_tol;
extern char global_machine_name[100];  // You can adjust the size as necessary

extern void update_count_label(lv_obj_t *label, int hour, int minute, int second);
// SCREEN: ui_LoadingScreen
void ui_screen_loading_init(void);
extern lv_obj_t * ui_LoadingScreen;
extern lv_obj_t * ui_merkotechLogo;
extern lv_obj_t * ui_loadingSpinner;

// CUSTOM VARIABLES
extern lv_obj_t *tileview_test;
extern lv_obj_t *tile_main_test;
extern lv_obj_t *tile_durus_test;
void ui_tileTestScreen_init(void);

extern time_t login_timestamp;

// SCREEN: ui_MainScreen
void ui_screen_main_init(void);
extern lv_obj_t *tileview;
extern lv_obj_t *tile_main;
extern lv_obj_t *tile_durus;

extern lv_obj_t * ui_MainScreen;
extern lv_obj_t * ui_headerContainer;
extern lv_obj_t * ui_timeLabel;
extern lv_obj_t * ui_dateLabel;
extern lv_obj_t * ui_mainContainer;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_leftSideContainer;
extern lv_obj_t * ui_performanceLabel;
extern lv_obj_t * ui_performanceDataLabel;
extern lv_obj_t * ui_goalLabel;
extern lv_obj_t * ui_goalDataLabel;
extern lv_obj_t * ui_dailyGoalLabel;
extern lv_obj_t * ui_dailyGoalDataLabel;
extern lv_obj_t * ui_rightSideContainer;
extern lv_obj_t * ui_nameLabel;
extern lv_obj_t * ui_surnameLabel;
extern lv_obj_t * ui_counterContainer;
extern lv_obj_t * ui_outerPanel;
extern lv_obj_t * ui_innerPanel;
extern lv_obj_t * ui_counterDataLabel;
extern lv_obj_t * ui_PulseLabel;
extern lv_obj_t * ui_checkerBar;
extern lv_obj_t * ui_footerContainer;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_exitButton;
extern lv_obj_t * ui_settingsButton;
extern lv_obj_t *ta_user;
extern lv_obj_t *ta_pass;
// CUSTOM VARIABLES

// SCREEN: ui_loginScreen
void ui_screen_login_init(void);
extern lv_obj_t * ui_loginScreen;

// CUSTOM VARIABLES

void ui_screen_bant_select_init(void);
void ui_screen_order_select_init(void);
void ui_screen_operation_select_init(void);
void ui_screen_settings_init(void);

extern lv_obj_t * screen_bant;
extern lv_obj_t * screen_order;
extern lv_obj_t * screen_operation;

extern int selected_bant_id;
extern int selected_order_id;

extern lv_obj_t *screen_settings;

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_973080792);    // assets/Merkotech_logo-removebg-preview.png
LV_IMG_DECLARE(ui_img_logout_png);    // assets/logout.png
LV_IMG_DECLARE(ui_img_setting_png);    // assets/setting.png
LV_IMG_DECLARE(ui_img_441374382);    // assets/logout-512.png

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.3
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"


///////////////////// SCREENS ////////////////////

#include "screens/ui_home.h"
#include "screens/ui_home1.h"
#include "screens/ui_home2.h"

///////////////////// VARIABLES ////////////////////


// EVENTS

extern lv_obj_t * ui____initial_actions0;

// FONTS
LV_FONT_DECLARE(ui_font_Fontcn16);
LV_FONT_DECLARE(ui_font_Fontcn20);
LV_FONT_DECLARE(ui_font_Fontcn32);

// UI INIT
void ui_init(void);
void ui_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

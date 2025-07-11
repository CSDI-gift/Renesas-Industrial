/**
 * @file lv_demo_music.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_rtt_music.h"

#if LV_USE_DEMO_RTT_MUSIC

#include "lv_demo_rtt_music_main.h"
#include "lv_demo_rtt_music_list.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if LV_DEMO_RTT_MUSIC_AUTO_PLAY
static void auto_step_cb(lv_timer_t * timer);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * ctrl;
static lv_obj_t * list;

static const char * title_list[] = {
    "Waiting for true love",
    "Need a Better Future",
    "Vibrations",
    "Why now?",
    "Never Look Back",
    "It happened Yesterday",
    "Feeling so High",
    "Go Deeper",
    "Find You There",
    "Until the End",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
};

static const char * artist_list[] = {
    "The John Smith Band",
    "My True Name",
    "Robotics",
    "John Smith",
    "My True Name",
    "Robotics",
    "Robotics",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
    "Unknown artist",
};

static const char * genre_list[] = {
    "Rock - 1997",
    "Drum'n bass - 2016",
    "Psy trance - 2020",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
    "Metal - 2015",
};

static const uint32_t time_list[] = {
    1 * 60 + 14,
    2 * 60 + 26,
    1 * 60 + 54,
    2 * 60 + 24,
    2 * 60 + 37,
    3 * 60 + 33,
    1 * 60 + 56,
    3 * 60 + 31,
    2 * 60 + 20,
    2 * 60 + 19,
    2 * 60 + 20,
    2 * 60 + 19,
    2 * 60 + 20,
    2 * 60 + 19,
};

#if LV_DEMO_RTT_MUSIC_AUTO_PLAY
    static lv_timer_t * auto_step_timer;
#endif

static lv_color_t original_screen_bg_color;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_music(void)
{
    ui_init();
}

void lv_demo_music_close(void)
{
    /*Delete all aniamtions*/
    lv_anim_del(NULL, NULL);

#if LV_DEMO_RTT_MUSIC_AUTO_PLAY
    lv_timer_del(auto_step_timer);
#endif
    _lv_demo_music_list_close();
    _lv_demo_music_main_close();

    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), original_screen_bg_color, 0);
}

const char * _lv_demo_music_get_title(uint32_t track_id)
{
    if(track_id >= sizeof(title_list) / sizeof(title_list[0])) return NULL;
    return title_list[track_id];
}

const char * _lv_demo_music_get_artist(uint32_t track_id)
{
    if(track_id >= sizeof(artist_list) / sizeof(artist_list[0])) return NULL;
    return artist_list[track_id];
}

const char * _lv_demo_music_get_genre(uint32_t track_id)
{
    if(track_id >= sizeof(genre_list) / sizeof(genre_list[0])) return NULL;
    return genre_list[track_id];
}

uint32_t _lv_demo_music_get_track_length(uint32_t track_id)
{
    if(track_id >= sizeof(time_list) / sizeof(time_list[0])) return 0;
    return time_list[track_id];
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#if LV_DEMO_RTT_MUSIC_AUTO_PLAY
static void auto_step_cb(lv_timer_t * t)
{
    LV_UNUSED(t);
    static uint32_t state = 0;

#if LV_DEMO_RTT_MUSIC_LARGE
    // const lv_font_t * font_small = &lv_font_montserrat_22;
    const lv_font_t * font_large = &lv_font_montserrat_32;
#else
    // const lv_font_t * font_small = &lv_font_montserrat_12;
    const lv_font_t * font_large = &lv_font_montserrat_16;
#endif

    switch(state) {
    case 5:
        _lv_demo_music_album_next(true);
        break;

    case 6:
        _lv_demo_music_album_next(true);
        break;
    case 7:
        _lv_demo_music_album_next(true);
        break;
    case 8:
        _lv_demo_music_play(0);
        break;
#if LV_DEMO_RTT_MUSIC_SQUARE || LV_DEMO_RTT_MUSIC_ROUND
    case 11:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
    case 13:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
#else
    case 12:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
#endif
    case 15:
        lv_obj_scroll_by(list, 0, -300, LV_ANIM_ON);
        break;
    case 16:
        lv_obj_scroll_by(list, 0, 300, LV_ANIM_ON);
        break;
    case 18:
        _lv_demo_music_play(1);
        break;
    case 19:
        lv_obj_scroll_by(ctrl, 0, LV_VER_RES, LV_ANIM_ON);
        break;
#if LV_DEMO_RTT_MUSIC_SQUARE || LV_DEMO_RTT_MUSIC_ROUND
    case 20:
        lv_obj_scroll_by(ctrl, 0, LV_VER_RES, LV_ANIM_ON);
        break;
#endif
    case 30:
        _lv_demo_music_play(2);
        break;
#ifndef LV_DEMO_RTT_MUSIC_AUTO_PLAY_FOREVER
    case 40: {
          lv_obj_t * bg = lv_layer_top();
          lv_obj_set_style_bg_color(bg, lv_color_hex(0x28B8BA), 0);
          lv_obj_set_style_text_color(bg, lv_color_white(), 0);
          lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, 0);
          lv_obj_fade_in(bg, 400, 0);
          lv_obj_t * dsc = lv_label_create(bg);
          lv_obj_set_style_text_font(dsc, font_large, 0);
          lv_label_set_text(dsc, "The average FPS is");
          lv_obj_align(dsc, LV_ALIGN_TOP_MID, 0, 200);

          lv_obj_t * num = lv_label_create(bg);
          lv_obj_set_style_text_font(num, font_large, 0);
#if LV_USE_PERF_MONITOR
          lv_label_set_text_fmt(num, "%"LV_PRId32, lv_refr_get_fps_avg());
#endif
          lv_obj_align(num, LV_ALIGN_TOP_MID, 0, 230);

          /* show LVGL and RT-Thread version */
          lv_obj_t* version_attr = lv_label_create(bg);
          lv_obj_set_style_text_align(version_attr, LV_TEXT_ALIGN_CENTER, 0);
          lv_obj_set_style_text_font(version_attr, font_large, 0);
          lv_label_set_text_fmt(version_attr, "LVGL %"LV_PRIu32".%"LV_PRIu32".%"LV_PRIu32" & RT-Thread %"LV_PRIu32".%"LV_PRIu32".%"LV_PRIu32,
              (uint32_t)LVGL_VERSION_MAJOR, (uint32_t)LVGL_VERSION_MINOR, (uint32_t)LVGL_VERSION_PATCH,
#if RT_VER_NUM < 0x50000
              (uint32_t)RT_VERSION, (uint32_t)RT_SUBVERSION, (uint32_t)RT_REVISION);
#else
              (uint32_t)RT_VERSION_MAJOR, (uint32_t)RT_VERSION_MINOR, (uint32_t)RT_VERSION_PATCH);
#endif
          lv_obj_align(version_attr, LV_ALIGN_BOTTOM_MID, 0, -70);

          /* show LVGL copyright */
          lv_obj_t * lvgl_copyright_attr = lv_label_create(bg);
          lv_obj_set_style_text_align(lvgl_copyright_attr, LV_TEXT_ALIGN_CENTER, 0);
          lv_obj_set_style_text_font(lvgl_copyright_attr, font_large, 0);
#if LV_DEMO_RTT_MUSIC_SQUARE || LV_DEMO_RTT_MUSIC_ROUND
          lv_label_set_text(lvgl_copyright_attr, "Copyright 2022 LVGL Kft.\nwww.lvgl.io | lvgl@lvgl.io");
#else
          lv_label_set_text(lvgl_copyright_attr, "Copyright 2022 LVGL Kft. | www.lvgl.io | lvgl@lvgl.io");
#endif
          lv_obj_align(lvgl_copyright_attr, LV_ALIGN_BOTTOM_MID, 0, -40);

          /* show RT-Thread copyright */
          lv_obj_t * rtt_copyright_attr = lv_label_create(bg);
          lv_obj_set_style_text_align(rtt_copyright_attr, LV_TEXT_ALIGN_CENTER, 0);
          lv_obj_set_style_text_font(rtt_copyright_attr, font_large, 0);
          lv_label_set_text(rtt_copyright_attr, "RT-Thread IoT RTOS | www.rt-thread.io");
          lv_obj_align(rtt_copyright_attr, LV_ALIGN_BOTTOM_MID, 0, -10);
          break;
    }
#endif /* LV_DEMO_RTT_MUSIC_AUTO_PLAY_FOREVER */
    case 41:
#ifndef LV_DEMO_RTT_MUSIC_AUTO_PLAY_FOREVER
        lv_scr_load(lv_obj_create(NULL));
        _lv_demo_music_pause();
#else
        state = 0;
#endif
        break;
    }
    state++;
}

#endif /*LV_DEMO_RTT_MUSIC_AUTO_PLAY*/

#endif /*LV_USE_DEMO_RTT_MUSIC*/

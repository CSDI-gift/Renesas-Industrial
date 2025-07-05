SET(SOURCES screens/ui_home.c
    screens/ui_home1.c
    screens/ui_home2.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    fonts/ui_font_Fontcn16.c
    fonts/ui_font_Fontcn20.c
    fonts/ui_font_Fontcn32.c)

add_library(ui ${SOURCES})

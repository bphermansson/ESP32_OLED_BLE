#include "ui.h"
#include <string.h>

LV_FONT_DECLARE(ui_font_text);

static lv_obj_t *screen;
static lv_obj_t *clock_view;
static lv_obj_t *clock_time;
static lv_obj_t *clock_date;
static lv_obj_t *notification_view;
static lv_obj_t *notification_text;
static lv_obj_t *connection_animation;
static lv_obj_t *connection_dot;

LV_DRAW_BUF_DEFINE_STATIC(animation_buffer, 128, 64, LV_COLOR_FORMAT_I1);

static lv_obj_t *make_label(lv_obj_t *parent, const lv_font_t *font,
                            lv_color_t color) {
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, lv_pct(100));
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, color, 0);
    return label;
}

void ui_init(void) {
    const int32_t width = lv_display_get_horizontal_resolution(NULL);
    const int32_t height = lv_display_get_vertical_resolution(NULL);
    const bool large = width >= 240 && height >= 160;
    connection_animation = NULL;

    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    clock_view = lv_obj_create(screen);
    lv_obj_remove_style_all(clock_view);
    lv_obj_set_size(clock_view, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(clock_view, lv_color_hex(0x070E1B), 0);
    lv_obj_set_style_bg_opa(clock_view, LV_OPA_COVER, 0);
    lv_obj_set_scrollbar_mode(clock_view, LV_SCROLLBAR_MODE_OFF);

    clock_time = make_label(clock_view, large ? &lv_font_montserrat_48 : &ui_font_text,
                            lv_color_hex(0xF1F5F9));
    lv_obj_set_style_text_align(clock_time, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(clock_time, LV_ALIGN_CENTER, 0, large ? -20 : -12);

    clock_date = make_label(clock_view, &ui_font_text, lv_color_hex(0x8E9FB8));
    lv_obj_set_style_text_align(clock_date, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(clock_date, LV_ALIGN_CENTER, 0, large ? 30 : 12);

    if (large) {
        LV_DRAW_BUF_INIT_STATIC(animation_buffer);
        connection_animation = lv_canvas_create(clock_view);
        lv_canvas_set_draw_buf(connection_animation, &animation_buffer);
        lv_canvas_set_palette(connection_animation, 0, lv_color32_make(7, 14, 27, 255));
        lv_canvas_set_palette(connection_animation, 1, lv_color32_make(255, 255, 255, 255));
        lv_obj_align(connection_animation, LV_ALIGN_TOP_RIGHT, 0, 0);
    }

    connection_dot = lv_obj_create(clock_view);
    lv_obj_remove_style_all(connection_dot);
    lv_obj_set_size(connection_dot, large ? 16 : 6, large ? 16 : 6);
    lv_obj_set_style_radius(connection_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(connection_dot, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_bg_opa(connection_dot, LV_OPA_COVER, 0);
    lv_obj_align(connection_dot, LV_ALIGN_TOP_RIGHT, -4, 4);

    notification_view = lv_obj_create(screen);
    lv_obj_remove_style_all(notification_view);
    lv_obj_set_size(notification_view, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(notification_view, lv_color_hex(0x070E1B), 0);
    lv_obj_set_style_bg_opa(notification_view, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(notification_view, 4, 0);
    lv_obj_set_scrollbar_mode(notification_view, LV_SCROLLBAR_MODE_OFF);
    notification_text = make_label(notification_view, &ui_font_text, lv_color_white());
    lv_label_set_long_mode(notification_text, LV_LABEL_LONG_MODE_WRAP);
    lv_obj_set_hidden(notification_view, true);
    lv_screen_load(screen);
}

void ui_show_clock(const char *time, const char *date, bool connected) {
    lv_label_set_text(clock_time, time);
    lv_label_set_text(clock_date, date);
    lv_obj_set_hidden(clock_view, false);
    lv_obj_set_hidden(notification_view, true);
    if (connected) {
        lv_obj_set_hidden(connection_dot, false);
        if (connection_animation) lv_obj_set_hidden(connection_animation, true);
    } else {
        lv_obj_set_hidden(connection_dot, true);
        if (connection_animation) lv_obj_set_hidden(connection_animation, false);
    }
}

void ui_show_notification(const char *title, const char *message) {
    lv_label_set_text_fmt(notification_text, "%s\n%s", title, message);
    lv_obj_set_hidden(clock_view, true);
    lv_obj_set_hidden(notification_view, false);
}

void ui_show_off(void) {
    lv_obj_set_hidden(clock_view, true);
    lv_obj_set_hidden(notification_view, true);
}

void ui_set_animation_frame(const uint8_t *frame) {
    if (!connection_animation) return;
    for (uint32_t y = 0; y < 64; ++y) {
        memcpy(lv_draw_buf_goto_xy(&animation_buffer, 0, y), frame + y * 16, 16);
    }
    lv_draw_buf_flush_cache(&animation_buffer, NULL);
    lv_obj_invalidate(connection_animation);
}

#include "ui.h"

LV_FONT_DECLARE(ui_font_text);
LV_FONT_DECLARE(my_font_133);
LV_FONT_DECLARE(ui_font_swedish_38);
LV_FONT_DECLARE(ui_font_date_48);

static lv_obj_t *screen;
static lv_obj_t *clock_view;
static lv_obj_t *clock_time;
static lv_obj_t *clock_date;
static lv_obj_t *notification_view;
static lv_obj_t *notification_text;
static lv_font_t notification_font;
static lv_obj_t *connection_line;
static lv_point_precise_t connection_line_points[2];

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

    clock_time = make_label(clock_view, large ? &my_font_133 : &ui_font_text,
                            lv_color_hex(0xF1F5F9));
    lv_obj_set_style_text_align(clock_time, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(clock_time, LV_ALIGN_CENTER, 0, large ? -20 : -12);

    clock_date = make_label(clock_view, &ui_font_text, lv_color_hex(0x8E9FB8));
    lv_obj_set_style_text_font(clock_date, &ui_font_date_48, 0);

    lv_obj_set_style_text_align(clock_date, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(clock_date, LV_ALIGN_CENTER, 0, large ? 50 : 62);

    connection_line_points[0].x = width * 15 / 320;
    connection_line_points[0].y = large ? 12 : 4;
    connection_line_points[1].x = width * 295 / 320;
    connection_line_points[1].y = connection_line_points[0].y;
    connection_line = lv_line_create(clock_view);
    lv_line_set_points(connection_line, connection_line_points, 2);
    lv_obj_set_style_line_width(connection_line, 2, 0);
    lv_obj_set_style_line_color(connection_line, lv_color_hex(0xFF0000), 0);

    notification_view = lv_obj_create(screen);
    lv_obj_remove_style_all(notification_view);
    lv_obj_set_size(notification_view, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(notification_view, lv_color_hex(0x070E1B), 0);
    lv_obj_set_style_bg_opa(notification_view, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(notification_view, 4, 0);
    lv_obj_set_scrollbar_mode(notification_view, LV_SCROLLBAR_MODE_OFF);
    notification_font = lv_font_montserrat_38;
    notification_font.fallback = &ui_font_swedish_38;
    notification_text = make_label(notification_view, &notification_font, lv_color_white());
    lv_label_set_long_mode(notification_text, LV_LABEL_LONG_MODE_WRAP);
    lv_obj_set_hidden(notification_view, true);
    lv_screen_load(screen);
}

void ui_show_clock(const char *time, const char *date, bool connected) {
    lv_label_set_text(clock_time, time);
    lv_label_set_text(clock_date, date);
    lv_obj_set_hidden(clock_view, false);
    lv_obj_set_hidden(notification_view, true);
    lv_obj_set_style_line_color(connection_line,
                                lv_color_hex(connected ? 0x00FF00 : 0xFF0000), 0);
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

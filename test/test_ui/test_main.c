#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

LV_FONT_DECLARE(ui_font_text);
static uint16_t draw_buffer[320 * 20];
static uint16_t pixels[320 * 240];
static int width;

static void flush(lv_display_t *display, const lv_area_t *area, uint8_t *map) {
    const uint16_t *src = (const uint16_t *)map;
    for (int y = area->y1; y <= area->y2; ++y)
        for (int x = area->x1; x <= area->x2; ++x)
            pixels[y * width + x] = *src++;
    lv_display_flush_ready(display);
}

static int any_pixels(void) {
    for (int i = 0; i < width * lv_display_get_vertical_resolution(NULL); ++i)
        if (pixels[i]) return 1;
    return 0;
}

static int any_bright_pixels(void) {
    for (int i = 0; i < width * lv_display_get_vertical_resolution(NULL); ++i)
        if ((pixels[i] & 0xf800) >= 0x8000) return 1;
    return 0;
}

static void test_size(int w, int h) {
    width = w;
    memset(pixels, 0, sizeof(pixels));
    lv_display_t *display = lv_display_create(w, h);
    assert(display);
    lv_display_set_default(display);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(display, draw_buffer, NULL, sizeof(draw_buffer),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, flush);
    ui_init();

    ui_show_clock("14:32", "Mon 28 Mar", false);
    lv_obj_t *screen = lv_screen_active();
    lv_obj_t *clock = lv_obj_get_child(screen, 0);
    lv_obj_t *notification = lv_obj_get_child(screen, 1);
    assert(strcmp(lv_label_get_text(lv_obj_get_child(clock, 0)), "14:32") == 0);
    assert(strcmp(lv_label_get_text(lv_obj_get_child(clock, 1)), "Mon 28 Mar") == 0);
    if (w == 320) {
        uint8_t frame[128 * 64 / 8];
        memset(frame, 0xff, sizeof(frame));
        ui_set_animation_frame(frame);
    }
    lv_refr_now(display);
    assert(any_bright_pixels());
    if (w == 320) assert(pixels[8 * width + 200] == 0xffff);

    ui_show_clock("14:33", "Mon 28 Mar", true);
    lv_refr_now(display);
    assert(strcmp(lv_label_get_text(lv_obj_get_child(clock, 0)), "14:33") == 0);
    assert(any_bright_pixels());
    if (w == 320) assert(pixels[8 * width + 200] != 0xffff);

    ui_show_notification("Meddelande", "\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96");
    lv_refr_now(display);
    assert(strcmp(lv_label_get_text(lv_obj_get_child(notification, 0)),
                  "Meddelande\n\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96") == 0);
    assert(any_bright_pixels());

    ui_show_off();
    lv_refr_now(display);
    assert(!any_pixels());
    ui_show_clock("14:34", "Mon 28 Mar", true);
    lv_refr_now(display);
    assert(any_bright_pixels());
    lv_display_delete(display);
}

int main(void) {
    lv_init();
    const uint32_t letters[] = {0xc4, 0xc5, 0xd6, 0xe4, 0xe5, 0xf6};
    for (unsigned i = 0; i < sizeof(letters) / sizeof(letters[0]); ++i) {
        lv_font_glyph_dsc_t glyph;
        assert(lv_font_get_glyph_dsc(&ui_font_text, &glyph, letters[i], 0));
    }
    test_size(320, 240);
    test_size(128, 64);
    lv_deinit();
    puts("LVGL screen, animation and Swedish glyph checks passed");
    return 0;
}

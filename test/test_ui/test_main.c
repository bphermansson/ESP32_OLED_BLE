#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

LV_FONT_DECLARE(ui_font_text);
LV_FONT_DECLARE(my_font_133);
LV_FONT_DECLARE(full_font_133);
LV_FONT_DECLARE(ui_font_swedish_38);
LV_FONT_DECLARE(ui_font_date_48);
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
    lv_obj_t *line = lv_obj_get_child(clock, 2);
    lv_color_t color = lv_obj_get_style_line_color(line, LV_PART_MAIN);
    assert(color.red == 255 && color.green == 0 && color.blue == 0);
    lv_refr_now(display);
    assert(any_bright_pixels());
    assert(pixels[(w == 320 ? 12 : 4) * width + w / 2] == 0xf800);

    ui_show_clock("14:33", "Mon 28 Mar", true);
    lv_refr_now(display);
    assert(strcmp(lv_label_get_text(lv_obj_get_child(clock, 0)), "14:33") == 0);
    assert(any_bright_pixels());
    color = lv_obj_get_style_line_color(line, LV_PART_MAIN);
    assert(color.red == 0 && color.green == 255 && color.blue == 0);
    assert(pixels[(w == 320 ? 12 : 4) * width + w / 2] == 0x07e0);

    ui_show_clock("14:33", "Mon 28 Mar", false);
    lv_refr_now(display);
    assert(pixels[(w == 320 ? 12 : 4) * width + w / 2] == 0xf800);

    ui_show_notification("Meddelande", "\xc3\xa5\xc3\xa4\xc3\xb6 \xc3\x85\xc3\x84\xc3\x96");
    lv_refr_now(display);
    const lv_font_t *notification_font =
        lv_obj_get_style_text_font(lv_obj_get_child(notification, 0), LV_PART_MAIN);
    assert(notification_font->dsc == lv_font_montserrat_38.dsc);
    assert(notification_font->fallback == &ui_font_swedish_38);
    const uint32_t swedish_letters[] = {0xc4, 0xc5, 0xd6, 0xe4, 0xe5, 0xf6};
    for (unsigned i = 0; i < sizeof(swedish_letters) / sizeof(swedish_letters[0]); ++i) {
        lv_font_glyph_dsc_t glyph;
        assert(lv_font_get_glyph_dsc(notification_font, &glyph, swedish_letters[i], 0));
        assert(glyph.resolved_font == &ui_font_swedish_38);
        assert(glyph.box_h >= 20);
    }
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
    const lv_font_fmt_txt_dsc_t *date_original = lv_font_montserrat_48.dsc;
    const lv_font_fmt_txt_dsc_t *date_subset = ui_font_date_48.dsc;
    for (uint32_t character = ' '; character <= '~'; ++character) {
        for (uint32_t next = ' '; next <= '~'; ++next) {
            lv_font_glyph_dsc_t expected, actual;
            assert(lv_font_get_glyph_dsc(&lv_font_montserrat_48, &expected, character, next));
            assert(lv_font_get_glyph_dsc(&ui_font_date_48, &actual, character, next));
            assert(expected.adv_w == actual.adv_w);
            assert(expected.box_w == actual.box_w && expected.box_h == actual.box_h);
            assert(expected.ofs_x == actual.ofs_x && expected.ofs_y == actual.ofs_y);
            const lv_font_fmt_txt_glyph_dsc_t *old_glyph = &date_original->glyph_dsc[expected.gid.index];
            const lv_font_fmt_txt_glyph_dsc_t *new_glyph = &date_subset->glyph_dsc[actual.gid.index];
            size_t bytes = (expected.box_w * expected.box_h * 4 + 7) / 8;
            assert(memcmp(date_original->glyph_bitmap + old_glyph->bitmap_index,
                          date_subset->glyph_bitmap + new_glyph->bitmap_index, bytes) == 0);
        }
    }
    const lv_font_fmt_txt_dsc_t *original = full_font_133.dsc;
    const lv_font_fmt_txt_dsc_t *subset = my_font_133.dsc;
    for (uint32_t character = '0'; character <= ':'; ++character) {
        for (uint32_t next = '0'; next <= ':'; ++next) {
            lv_font_glyph_dsc_t expected, actual;
            assert(lv_font_get_glyph_dsc(&full_font_133, &expected, character, next));
            assert(lv_font_get_glyph_dsc(&my_font_133, &actual, character, next));
            assert(expected.adv_w == actual.adv_w);
            assert(expected.box_w == actual.box_w && expected.box_h == actual.box_h);
            assert(expected.ofs_x == actual.ofs_x && expected.ofs_y == actual.ofs_y);
            const lv_font_fmt_txt_glyph_dsc_t *old_glyph = &original->glyph_dsc[expected.gid.index];
            const lv_font_fmt_txt_glyph_dsc_t *new_glyph = &subset->glyph_dsc[actual.gid.index];
            size_t bytes = (expected.box_w * expected.box_h * 4 + 7) / 8;
            assert(memcmp(original->glyph_bitmap + old_glyph->bitmap_index,
                          subset->glyph_bitmap + new_glyph->bitmap_index, bytes) == 0);
        }
    }
    const uint32_t letters[] = {0xc4, 0xc5, 0xd6, 0xe4, 0xe5, 0xf6};
    for (unsigned i = 0; i < sizeof(letters) / sizeof(letters[0]); ++i) {
        lv_font_glyph_dsc_t glyph;
        assert(lv_font_get_glyph_dsc(&ui_font_text, &glyph, letters[i], 0));
    }
    test_size(320, 240);
    test_size(128, 64);
    lv_deinit();
    puts("LVGL screen, connection line and Swedish glyph checks passed");
    return 0;
}

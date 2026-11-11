#ifndef UI_H
#define UI_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);
void ui_show_clock(const char *time, const char *date, bool connected);
void ui_show_notification(const char *title, const char *message);
void ui_show_off(void);
void ui_set_animation_frame(const uint8_t *frame);

#ifdef __cplusplus
}
#endif

#endif

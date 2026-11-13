cmake_minimum_required(VERSION 3.16)
project(esp32_oled_ble_ui_test C)

if(NOT LVGL_DIR)
    message(FATAL_ERROR "Set LVGL_DIR to the LVGL library directory")
endif()

set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_LIST_DIR}/../../include/lv_conf.h" CACHE PATH "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)
add_subdirectory("${LVGL_DIR}" lvgl)

add_executable(ui_test test_main.c ../../src/ui.c ../../src/ui_font_text.c
    ../../src/clock_font_133.c ../../src/my_font_133.c ../../src/ui_font_swedish_38.c
    ../../src/ui_font_date_48.c)
set_source_files_properties(../../src/my_font_133.c PROPERTIES COMPILE_DEFINITIONS "my_font_133=full_font_133")
target_include_directories(ui_test PRIVATE ../../include)
target_compile_options(ui_test PRIVATE -UNDEBUG)
target_link_libraries(ui_test PRIVATE lvgl m)

const fs = require("node:fs");
const path = require("node:path");

const root = path.resolve(__dirname, "..");
const input = path.join(root, ".pio/libdeps/nologo_esp32c3_super_mini/lvgl/src/font/lv_font_montserrat_48.c");
let font = fs.readFileSync(input, "utf8");
const bitmap = font.match(/(static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap\[\] = \{)([\s\S]*?)(\n\};)/);
if (!bitmap) throw new Error("Cannot find Montserrat 48 glyph bitmap");
const end = bitmap[2].indexOf('/* U+00B0 "°" */');
if (end < 0 || !bitmap[2].includes('/* U+007E "~" */')) {
  throw new Error("Unexpected Montserrat 48 glyph order");
}
font = font.replace(bitmap[0], `${bitmap[1]}${bitmap[2].slice(0, end).trimEnd()}\n};`);
if (!font.includes(".cmap_num = 2,")) throw new Error("Unexpected Montserrat 48 character maps");
font = font.replace(".cmap_num = 2,", ".cmap_num = 1,");
font = font.replace(/\blv_font_montserrat_48\b/g, "ui_font_date_48");
font = font.replace('#include "../../lvgl.h"', '#include "lvgl.h"');
font = "/* ASCII subset of LVGL's Montserrat 48; see include/LVGL-MIT.txt. */\n" + font;
fs.writeFileSync(path.join(root, "src/ui_font_date_48.c"), font);
console.log("Generated src/ui_font_date_48.c");

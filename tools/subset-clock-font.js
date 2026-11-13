const fs = require("node:fs");
const path = require("node:path");

const root = path.resolve(__dirname, "..");
const source = fs.readFileSync(path.join(root, "src/my_font_133.c"), "utf8");
const output = path.join(root, "src/clock_font_133.c");

function replaceOnce(text, pattern, replacement, description) {
  const updated = text.replace(pattern, replacement);
  if (updated === text) throw new Error(`Could not find ${description} in source font`);
  return updated;
}

const bitmapMatch = source.match(/(static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap\[\] = \{)([\s\S]*?)(\n\};)/);
if (!bitmapMatch) throw new Error("Could not find glyph bitmap");
const first = bitmapMatch[2].indexOf('/* U+0030 "0" */');
const last = bitmapMatch[2].indexOf('/* U+003B ";" */');
if (first < 0 || last <= first) throw new Error("Could not find clock glyph bitmap range");

const glyphMatch = source.match(/(static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc\[\] = \{)([\s\S]*?)(\n\};)/);
if (!glyphMatch) throw new Error("Could not find glyph descriptors");
const descriptors = [...glyphMatch[2].matchAll(/\{\.bitmap_index = (\d+),[^}]+\}/g)];
if (descriptors.length < 28) throw new Error("Clock glyph descriptors are missing");
const baseOffset = Number(descriptors[17][1]);
if (baseOffset !== 23189) throw new Error("Unexpected clock glyph bitmap offset");
const selected = descriptors.slice(17, 28).map((entry) =>
  entry[0].replace(/bitmap_index = \d+/, `bitmap_index = ${Number(entry[1]) - baseOffset}`));
const placeholder = "{.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0}";
const glyphs = Array(17).fill(placeholder).concat(selected).map((entry) => `    ${entry},`).join("\n");

let result = replaceOnce(source, bitmapMatch[0],
  `${bitmapMatch[1]}\n    ${bitmapMatch[2].slice(first, last).trimEnd()}\n};`, "glyph bitmap");
result = replaceOnce(result, glyphMatch[0], `${glyphMatch[1]}\n${glyphs}\n};`, "glyph descriptors");
result = replaceOnce(result, ".range_start = 32, .range_length = 95, .glyph_id_start = 1",
  ".range_start = 48, .range_length = 11, .glyph_id_start = 17", "character map");
result = result.replace(/\/\*![\s\S]*?\*\//g, "");
result = `/* Generated from my_font_133.c by tools/subset-clock-font.js; digits and colon only. */\n${result}`;
fs.writeFileSync(output, result);
console.log(`Generated ${path.relative(root, output)}`);

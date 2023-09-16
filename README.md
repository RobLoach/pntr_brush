# pntr_brush

Brush API for [pntr](https://github.com/RobLoach/pntr), inspired by [Cairo](https://en.wikipedia.org/wiki/Cairo_(graphics)), [Skia](https://skia.org), or [Canvas](https://www.w3schools.com/html/html5_canvas.asp).

## Example

``` c
#define PNTR_BRUSH_IMPLEMENTATION
#include "pntr_brush.h"

// ..

pntr_image* image = pntr_gen_image_color(320, 160, PNTR_WHITE);
pntr_brush* brush = pntr_load_brush(image);
pntr_brush_font(brush, font);

pntr_brush_begin_path(brush);
brush->strokeStyle = PNTR_GREEN;  // This path is green.
pntr_brush_move_to(brush, 0, 0);
pntr_brush_line_to(brush, 130, 130);
pntr_brush_line_to(brush, 200, 90);
pntr_brush_stroke(brush);

pntr_brush_begin_path(brush);
brush->strokeStyle = PNTR_BLUE;  // This path is blue.
pntr_brush_move_to(brush, 200, 50);
pntr_brush_line_to(brush, 50, 120);
pntr_brush_stroke(brush);

pntr_brush_fill_style(brush, PNTR_RED);
pntr_brush_fill_rect(brush, 30, 10, 30, 30);

pntr_brush_begin_path(brush);
brush->fillStyle = PNTR_BROWN;
pntr_brush_move_to(brush, 230, 100);
pntr_brush_arc(brush, 230, 100, 30, 70, 320, 500);
pntr_brush_fill(brush);

brush->fillStyle = PNTR_BLACK;
pntr_brush_fill_text(brush, "Hello World!", 10, 50);

pntr_unload_brush(brush);
```

![pntr_brush screenshot](docs/screenshot.png)

# License

[MIT](LICENSE)

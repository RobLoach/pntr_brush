#include <assert.h>

#define PNTR_DISABLE_MATH
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_IMPLEMENTATION
#include "pntr.h"

#define PNTR_BRUSH_IMPLEMENTATION
#include "../pntr_brush.h"

int main() {
    pntr_image* image = pntr_gen_image_color(320, 240, PNTR_WHITE);
    pntr_brush* brush = pntr_load_brush(image);
    pntr_font* font = pntr_load_font_default();
    pntr_image* gradient = pntr_gen_image_gradient(60, 60, PNTR_RED, PNTR_BLUE, PNTR_GREEN, PNTR_BLACK);

    brush->font = font;
    pntr_brush_font(brush, font);

    // Variables
    brush->fillStyle = PNTR_RED;
    pntr_brush_fill_style(brush, PNTR_BLUE);
    assert(brush->fillStyle.value == PNTR_BLUE.value);

    brush->strokeStyle = PNTR_PURPLE;
    pntr_brush_stroke_style(brush, PNTR_BLUE);
    assert(brush->strokeStyle.value == PNTR_BLUE.value);

    brush->lineWidth = 4;
    pntr_brush_stroke_style(brush, PNTR_BLUE);
    assert(brush->strokeStyle.value == PNTR_BLUE.value);

    pntr_brush_reset(brush);

    // pntr_brush_arc
    {
        brush->lineWidth = 4;
        pntr_brush_begin_path(brush);
        pntr_brush_arc(brush, 100, 100, 60, 0, 260, 360);
        pntr_brush_stroke_style(brush, PNTR_BROWN);
        pntr_brush_stroke(brush);
    }

    // pntr_brush_fill
    {
        pntr_brush_begin_path(brush);
        pntr_brush_move_to(brush, 50, 20);
        pntr_brush_line_to(brush, 20, 80);
        pntr_brush_line_to(brush, 80, 40);
        brush->fillStyle = PNTR_BLUE;
        pntr_brush_fill(brush);
    }

    brush->lineWidth = 8;
    pntr_brush_begin_path(brush);
    pntr_brush_move_to(brush, 100, 20);
    pntr_brush_line_to(brush, 150, 40);
    pntr_brush_line_to(brush, 100, 80);
    brush->strokeStyle = PNTR_RED;
    pntr_brush_stroke(brush);

    // pntr_brush_save(), pntr_brush_restore()
    {
        pntr_brush_reset(brush);
        pntr_brush_stroke_style(brush, PNTR_RED);
        pntr_brush_save(brush);
        pntr_brush_stroke_style(brush, PNTR_BLUE);
        pntr_brush_restore(brush);
        assert(brush->strokeStyle.value == (PNTR_RED).value);
    }

    // pntr_brush_draw_fill_text()
    {
        pntr_brush_fill_text(brush, "pntr_brush", 10, 10);
    }

    // pntr_brush_draw_image
    {
        pntr_brush_draw_image(brush, gradient, image->width - gradient->width - 10, 10);
    }

    pntr_save_image(image, "pntr_brush_test.png");

    pntr_unload_image(gradient);
    pntr_unload_image(image);
    pntr_unload_brush(brush);

    // Demo screenshot
    {
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

        pntr_save_image(image, "screenshot.png");

        pntr_unload_brush(brush);
        pntr_unload_image(image);
    }

    pntr_unload_font(font);

    return 0;
}

/**********************************************************************************************
*
*   pntr_brush - Brush-like API for pntr, similar to something like Cairo or Skia.
*
*   Copyright 2023 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       pntr https://github.com/robloach/pntr
*
*   CONFIGURATION:
*
*   LICENSE: zlib/libpng
*
*       Copyright (c) 2023 Rob Loach (@RobLoach, https://robloach.net)
*
*       This software is provided "as-is", without any express or implied warranty. In no event
*       will the authors be held liable for any damages arising from the use of this software.
*
*       Permission is granted to anyone to use this software for any purpose, including commercial
*       applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*         1. The origin of this software must not be misrepresented; you must not claim that you
*         wrote the original software. If you use this software in a product, an acknowledgment
*         in the product documentation would be appreciated but is not required.
*
*         2. Altered source versions must be plainly marked as such, and must not be misrepresented
*         as being the original software.
*
*         3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef PNTR_BRUSH_H__
#define PNTR_BRUSH_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PNTR_BRUSH_API
    #define PNTR_BRUSH_API PNTR_API
#endif

#ifndef PNTR_BRUSH_MAX_PATHS
#define PNTR_BRUSH_MAX_PATHS 64
#endif

// TODO: REmove this
#ifndef PNTR_BRUSH_PNTR_H
#define PNTR_BRUSH_PNTR_H "pntr.h"
#endif
#include PNTR_BRUSH_PNTR_H

typedef enum pntr_brush_action_type {
    PNTR_BRUSH_ACTION_TYPE_MOVE_TO = 0,
    PNTR_BRUSH_ACTION_TYPE_LINE_TO,
    PNTR_BRUSH_ACTION_TYPE_ARC
} pntr_brush_action_type;

typedef struct pntr_brush_action {
    pntr_vector position;
    pntr_brush_action_type type;

    float radius;
    float startAngle;
    float endAngle;
    int segments;
} pntr_brush_action;

typedef struct pntr_brush {
    pntr_image* dst;
    pntr_color fillStyle;
    pntr_color strokeStyle;
    int lineWidth;
    pntr_brush_action path[PNTR_BRUSH_MAX_PATHS];
    int pathSize;
    void* saved;
    pntr_font* font;
} pntr_brush;

// Brush methods
PNTR_BRUSH_API pntr_brush* pntr_load_brush(pntr_image* dst);
PNTR_BRUSH_API void pntr_unload_brush(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_reset(pntr_brush* brush);

// Drawing methods
PNTR_BRUSH_API void pntr_brush_fill_rect(pntr_brush* brush, int x, int y, int width, int height);
PNTR_BRUSH_API void pntr_brush_stroke_rect(pntr_brush* brush, int x, int y, int width, int height);
PNTR_BRUSH_API void pntr_brush_clear_rect(pntr_brush* brush, int x, int y, int width, int height);
PNTR_BRUSH_API void pntr_brush_stroke_text(pntr_brush* brush, const char* text, int posX, int posY);
PNTR_BRUSH_API void pntr_brush_fill_text(pntr_brush* brush, const char* text, int posX, int posY);
PNTR_BRUSH_API void pntr_brush_draw_image(pntr_brush* brush, pntr_image* image, int posX, int posY);
PNTR_BRUSH_API void pntr_brush_fill(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_stroke(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_arc(pntr_brush* brush, int centerX, int centerY, float radius, float startAngle, float endAngle, int segments);

// Path methods
PNTR_BRUSH_API void pntr_brush_begin_path(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_move_to(pntr_brush* brush, int posX, int posY);
PNTR_BRUSH_API void pntr_brush_line_to(pntr_brush* brush, int posX, int posY);
PNTR_BRUSH_API void pntr_brush_rect(pntr_brush* brush, int x, int y, int width, int height);
PNTR_BRUSH_API void pntr_brush_close_path(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_save(pntr_brush* brush);
PNTR_BRUSH_API void pntr_brush_restore(pntr_brush* brush);

// Property methods
PNTR_BRUSH_API void pntr_brush_line_width(pntr_brush* brush, int lineWidth);
PNTR_BRUSH_API void pntr_brush_stroke_style(pntr_brush* brush, pntr_color color);
PNTR_BRUSH_API void pntr_brush_fill_style(pntr_brush* brush, pntr_color color);
PNTR_BRUSH_API void pntr_brush_font(pntr_brush* brush, pntr_font* font);
PNTR_BRUSH_API int pntr_brush_measure_text(pntr_brush* brush, const char* text);

#ifdef __cplusplus
}
#endif

#endif  // PNTR_BRUSH_H__

#ifdef PNTR_BRUSH_IMPLEMENTATION
#ifndef PNTR_BRUSH_IMPLEMENTATION_ONCE
#define PNTR_BRUSH_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

#define cvector_clib_free pntr_unload_memory
#define cvector_clib_calloc(nitems, size) pntr_load_memory(nitems * size)
void* _pntr_brush_realloc(void *ptr, size_t size) {
    pntr_unload_memory(ptr);
    return pntr_load_memory(size);
}
#define cvector_clib_realloc _pntr_brush_realloc
//#define cvector_clib_assert assert
#define cvector_clib_memcpy pntr_memory_copy
//#define cvector_clib_memmove

PNTR_BRUSH_API pntr_brush* pntr_load_brush(pntr_image* dst) {
    if (dst == NULL) {
        return NULL;
    }

    pntr_brush* brush = pntr_load_memory(sizeof(pntr_brush));
    if (brush == NULL) {
        return NULL;
    }

    brush->dst = dst;
    pntr_brush_reset(brush);

    return brush;
}

PNTR_BRUSH_API void pntr_unload_brush(pntr_brush* brush) {
    if (brush == NULL) {
        return;
    }

    if (brush->saved != NULL) {
        pntr_unload_memory(brush->saved);
    }

    pntr_unload_memory(brush);
}

PNTR_BRUSH_API void pntr_brush_reset(pntr_brush* brush) {
    brush->fillStyle = PNTR_BLACK;
    brush->strokeStyle = PNTR_BLACK;
    brush->lineWidth = 1;
    brush->pathSize = 0;
}

PNTR_BRUSH_API void pntr_brush_fill_rect(pntr_brush* brush, int x, int y, int width, int height) {
    pntr_draw_rectangle_fill(brush->dst, x, y, width, height, brush->fillStyle);
}

PNTR_BRUSH_API void pntr_brush_stroke_rect(pntr_brush* brush, int x, int y, int width, int height) {
    pntr_draw_rectangle_thick(brush->dst, x, y, width, height, brush->lineWidth, brush->strokeStyle);
}

#ifndef PNTR_BRUSH_PIXEL
#define PNTR_BRUSH_PIXEL(image, x, y) image->data[(y) * (image->pitch >> 2) + (x)]
#endif

PNTR_BRUSH_API void pntr_brush_clear_rect(pntr_brush* brush, int posX, int posY, int width, int height) {
    if (width == 0 && height == 0) {
        pntr_clear_background(brush->dst, PNTR_WHITE);
        return;
    }

    if (posX < 0) {
        width += posX;
        posX = 0;
    }
    if (posY < 0) {
        width += posX;
        posX = 0;
    }
    if (posX + width >= brush->dst->width) {
        width = posX - brush->dst->width;
    }
    if (posY + height >= brush->dst->height) {
        height = posY - brush->dst->height;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pntr_color* col = &PNTR_BRUSH_PIXEL(brush->dst, x, y);
            pntr_color_set_a(col, 0);
        }
    }
}

PNTR_BRUSH_API void pntr_brush_begin_path(pntr_brush* brush) {
    brush->pathSize = 0;
}

PNTR_BRUSH_API inline void pntr_brush_move_to(pntr_brush* brush, int posX, int posY) {
    brush->path[brush->pathSize++] = PNTR_CLITERAL(pntr_brush_action) {
        .type = PNTR_BRUSH_ACTION_TYPE_MOVE_TO,
        .position = PNTR_CLITERAL(pntr_vector) {
            .x = posX,
            .y = posY
        }
    };
}

PNTR_BRUSH_API inline void pntr_brush_line_to(pntr_brush* brush, int posX, int posY) {
    brush->path[brush->pathSize++] = PNTR_CLITERAL(pntr_brush_action) {
        .type = PNTR_BRUSH_ACTION_TYPE_LINE_TO,
        .position = PNTR_CLITERAL(pntr_vector) {
            .x = posX,
            .y = posY
        }
    };
}

PNTR_BRUSH_API void pntr_brush_arc(pntr_brush* brush, int centerX, int centerY, float radius, float startAngle, float endAngle, int segments) {
    brush->path[brush->pathSize++] = PNTR_CLITERAL(pntr_brush_action) {
        .type = PNTR_BRUSH_ACTION_TYPE_ARC,
        .position = PNTR_CLITERAL(pntr_vector) {
            .x = centerX,
            .y = centerY
        },
        .radius = radius,
        .startAngle = startAngle,
        .endAngle = endAngle,
        .segments = segments
    };
}

PNTR_BRUSH_API void pntr_brush_rect(pntr_brush* brush, int x, int y, int width, int height) {
    pntr_brush_move_to(brush, x, y);
    pntr_brush_line_to(brush, x + width, y);
    pntr_brush_line_to(brush, x + width, y + height);
    pntr_brush_line_to(brush, x, y + height);
    pntr_brush_line_to(brush, x, y);
}

PNTR_BRUSH_API void pntr_brush_stroke(pntr_brush* brush) {
    pntr_vector brushPosition = { 0 };
    int i;
    for (i = 0; i < brush->pathSize; i++) {
        pntr_brush_action* action = &brush->path[i];
        switch (action->type) {
            case PNTR_BRUSH_ACTION_TYPE_MOVE_TO: {
                brushPosition = action->position;
            }
            break;
            case PNTR_BRUSH_ACTION_TYPE_LINE_TO: {
                pntr_draw_line_vec(brush->dst, brushPosition, action->position, brush->strokeStyle);
                brushPosition = action->position;
            }
            break;
            case PNTR_BRUSH_ACTION_TYPE_ARC: {
                pntr_draw_arc(brush->dst, action->position.x, action->position.y, action->radius, action->startAngle, action->endAngle, action->segments, brush->strokeStyle);
            }
            break;
        }
    }
}

PNTR_BRUSH_API void pntr_brush_fill(pntr_brush* brush) {
    if (brush == NULL || brush->dst == NULL) {
        return;
    }

    pntr_vector brushPosition = { 0 };

    pntr_vector points[PNTR_BRUSH_MAX_PATHS];
    int pointsSize = 0;

    for (int i = 0; i < brush->pathSize; i++) {
        pntr_brush_action* action = &brush->path[i];
        switch (action->type) {
            case PNTR_BRUSH_ACTION_TYPE_MOVE_TO: {
                brushPosition = action->position;
            }
            break;
            case PNTR_BRUSH_ACTION_TYPE_LINE_TO: {
                points[pointsSize++] = brushPosition;
                points[pointsSize++] = action->position;
                brushPosition = action->position;
            }
            break;
            case PNTR_BRUSH_ACTION_TYPE_ARC: {
                pntr_draw_arc_fill(brush->dst, action->position.x, action->position.y, action->radius, action->startAngle, action->endAngle, action->segments, brush->fillStyle);
            }
            break;
        }
    }

    pntr_draw_polygon_fill(brush->dst, points, pointsSize, brush->fillStyle);
}

PNTR_BRUSH_API inline void pntr_brush_stroke_style(pntr_brush* brush, pntr_color color) {
    if (brush != NULL) {
        brush->strokeStyle = color;
    }
}

PNTR_BRUSH_API inline void pntr_brush_line_width(pntr_brush* brush, int lineWidth) {
    if (brush != NULL) {
        brush->lineWidth = lineWidth;
    }
}

PNTR_BRUSH_API void pntr_brush_close_path(pntr_brush* brush) {
    if (brush == NULL) {
        return;
    }

    // Find the most recent move to command
    for (int i = brush->pathSize - 1; i >= 0; i--) {
        if (brush->path[i].type == PNTR_BRUSH_ACTION_TYPE_MOVE_TO) {
            pntr_brush_line_to(brush, brush->path[i].position.x, brush->path[i].position.y);
            return;
        }
    }
}

PNTR_BRUSH_API inline void pntr_brush_draw_image(pntr_brush* brush, pntr_image* image, int posX, int posY) {
    pntr_draw_image(brush->dst, image, posX, posY);
}

PNTR_BRUSH_API void pntr_brush_save(pntr_brush* brush) {
    if (brush == NULL) {
        return;
    }

    if (brush->saved == NULL) {
        brush->saved = pntr_load_memory(sizeof(pntr_brush));
    }

    // TODO: Make an actual stack of saves.
    pntr_memory_copy(brush->saved, brush, sizeof(pntr_brush));
}

PNTR_BRUSH_API void pntr_brush_restore(pntr_brush* brush) {
    if (brush == NULL || brush->saved == NULL) {
        return;
    }

    // TODO: Restore stack -- pop off the top
    pntr_memory_copy(brush, brush->saved, sizeof(pntr_brush));
    pntr_unload_memory(brush->saved);
    brush->saved = NULL;
}

PNTR_BRUSH_API void pntr_brush_font(pntr_brush* brush, pntr_font* font) {
    if (brush != NULL && font != NULL) {
        brush->font = font;
    }
}

PNTR_BRUSH_API void pntr_brush_fill_text(pntr_brush* brush, const char* text, int posX, int posY) {
    if (brush == NULL) {
        return;
    }

    pntr_draw_text(brush->dst, brush->font, text, posX, posY, brush->fillStyle);
}

PNTR_BRUSH_API void pntr_brush_stroke_text(pntr_brush* brush, const char* text, int posX, int posY) {
    if (brush == NULL || brush->font == NULL) {
        return;
    }

    pntr_draw_text(brush->dst, brush->font, text, posX, posY, brush->strokeStyle);
}

PNTR_BRUSH_API int pntr_brush_measure_text(pntr_brush* brush, const char* text) {
    if (brush == NULL) {
        return 0;
    }

    return pntr_measure_text(brush->font, text);
}

PNTR_BRUSH_API inline void pntr_brush_fill_style(pntr_brush* brush, pntr_color color) {
    if (brush != NULL) {
        brush->fillStyle = color;
    }
}

#ifdef __cplusplus
}
#endif

#endif  // PNTR_BRUSH_IMPLEMENTATION_ONCE
#endif  // PNTR_BRUSH_IMPLEMENTATION

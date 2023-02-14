#if !defined(DK_FONT_H)
#define DK_FONT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if defined(DK_FONT_STATIC)
#define DK_API static
#else
#define DK_API extern
#endif

typedef struct dk_font_t
{
    int width;
    int height;
    int size;
    int ascent;
    int descent;
    int line_gap;
    int advance;
    int bearing_x;
    int bearing_y;
    int glyph_count;
    int kerning_count;
    int *kerning;
    int *glyphs;
} dk_font_t;

DK_API dk_font_t *dk_font_load(const char *filename);
DK_API void dk_font_free(dk_font_t *font);
DK_API int dk_font_get_glyph(dk_font_t *font, int codepoint);
DK_API int dk_font_get_kerning(dk_font_t *font, int left, int right);
DK_API int dk_font_get_width(dk_font_t *font, const char *text);
DK_API int dk_font_get_height(dk_font_t *font, const char *text);
DK_API int dk_font_get_size(dk_font_t *font, const char *text);
DK_API int dk_font_get_ascent(dk_font_t *font, const char *text);
DK_API int dk_font_get_descent(dk_font_t *font, const char *text);
DK_API int dk_font_get_line_gap(dk_font_t *font, const char *text);
DK_API int dk_font_get_advance(dk_font_t *font, const char *text);
DK_API int dk_font_get_bearing_x(dk_font_t *font, const char *text);
DK_API int dk_font_get_bearing_y(dk_font_t *font, const char *text);
DK_API int dk_font_get_glyph_count(dk_font_t *font, const char *text);
DK_API int dk_font_get_kerning_count(dk_font_t *font, const char *text);
DK_API void dk_font_draw(dk_font_t *font, const char *text, int *buffer, int width, int height, int x, int y, int color);

#if defined(DK_FONT_IMPLEMENTATION)


#define DK_FONT_MAX_GLYPHS 65536
#define DK_FONT_MAX_KERNING 65536

typedef struct dk_font_header_t
{
    int width;
    int height;
    int size;
    int ascent;
    int descent;
    int line_gap;
    int advance;
    int bearing_x;
    int bearing_y;
    int glyph_count;
    int kerning_count;
} dk_font_header_t;

typedef struct dk_font_glyph_t
{
    int codepoint;
    int advance;
    int bearing_x;
    int bearing_y;
    int width;
    int height;
    int x;
    int y;
} dk_font_glyph_t;

typedef struct dk_font_kerning_t
{
    int left;
    int right;
    int amount;
} dk_font_kerning_t;

typedef struct dk_font_bitmap_t
{
    int width;
    int height;
    int *pixels;
} dk_font_bitmap_t;

typedef struct dk_font_stbtt_t
{
    unsigned char *data;
    int size;
    int offset;
    int index;
} dk_font_stbtt_t;

static int dk_font_stbtt_read(void *user, char *data, int size)
{
    dk_font_stbtt_t *stbtt = (dk_font_stbtt_t *)user;
    memcpy(data, stbtt->data + stbtt->offset, size);
    stbtt->offset += size;
    return size;
}

static void dk_font_stbtt_skip(void *user, int size)
{
    dk_font_stbtt_t *stbtt = (dk_font_stbtt_t *)user;
    stbtt->offset += size;
}

static int dk_font_stbtt_eof(void *user)
{
    dk_font_stbtt_t *stbtt = (dk_font_stbtt_t *)user;
    return stbtt->offset >= stbtt->size;
}

static dk_font_bitmap_t *dk_font_stbtt_render(dk_font_stbtt_t *stbtt, int size, int codepoint)
{
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
    unsigned char *pixels = NULL;
    dk_font_bitmap_t *bitmap = NULL;
    stbtt_fontinfo font = {0};
    stbtt_InitFont(&font, stbtt->data, stbtt->offset);
    stbtt_GetCodepointBitmapBox(&font, codepoint, stbtt_ScaleForPixelHeight(&font, size), stbtt_ScaleForPixelHeight(&font, size), &x, &y, &width, &height);
    pixels = (unsigned char *)malloc(width * height);
    stbtt_MakeCodepointBitmap(&font, pixels, width, height, width, stbtt_ScaleForPixelHeight(&font, size), stbtt_ScaleForPixelHeight(&font, size), codepoint);
    bitmap = (dk_font_bitmap_t *)malloc(sizeof(dk_font_bitmap_t));
    bitmap->width = width;
    bitmap->height = height;
    bitmap->pixels = (int *)malloc(width * height * sizeof(int));
    for (int i = 0; i < width * height; i++)
    {
        bitmap->pixels[i] = pixels[i] << 24;
    }
    free(pixels);
    return bitmap;
}

static dk_font_bitmap_t *dk_font_stbtt_render_range(dk_font_stbtt_t *stbtt, int size, int start, int end)
{
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
    int offset = 0;
    int codepoint = 0;
    int advance = 0;
    int bearing_x = 0;
    int bearing_y = 0;
    int glyph_count = 0;
    int kerning_count = 0;
    int *kerning = NULL;
    int *glyphs = NULL;
    dk_font_bitmap_t *bitmap = NULL;
    dk_font_bitmap_t *glyph = NULL;
    stbtt_fontinfo font = {0};
    stbtt_InitFont(&font, stbtt->data, stbtt->offset);
    for (int i = start; i <= end; i++)
    {
        stbtt_GetCodepointBitmapBox(&font, i, stbtt_ScaleForPixelHeight(&font, size), stbtt_ScaleForPixelHeight(&font, size), &x, &y, &width, &height);
        if (width > bitmap->width)
        {
            bitmap->width = width;
        }
        if (height > bitmap->height)
        {
            bitmap->height = height;
        }
    }
    bitmap = (dk_font_bitmap_t *)malloc(sizeof(dk_font_bitmap_t));
    bitmap->width = 0;
    bitmap->height = 0;
    bitmap->pixels = (int *)malloc(bitmap->width * bitmap->height * sizeof(int));
    for (int i = start; i <= end; i++)
    {
        glyph = dk_font_stbtt_render(stbtt, size, i);
        if (glyph->width > 0 && glyph->height > 0)
        {
            if (glyph->width > bitmap->width)
            {
                bitmap->width = glyph->width;
            }
            if (glyph->height > bitmap->height)
            {
                bitmap->height = glyph->height;
            }
        }
        free(glyph);
    }
    bitmap->pixels = (int *)realloc(bitmap->pixels, bitmap->width * bitmap->height * sizeof(int));
    for (int i = start; i <= end; i++)
    {
        glyph = dk_font_stbtt_render(stbtt, size, i);
        if (glyph->width > 0 && glyph->height > 0)
        {
            for (int j = 0; j < glyph->height; j++)
            {
                for (int k = 0; k < glyph->width; k++)
                {
                    bitmap->pixels[(j * bitmap->width) + k] = glyph->pixels[(j * glyph->width) + k];
                }
            }
        }
        free(glyph);
    }
    return bitmap;
}

static void dk_font_stbtt_free(dk_font_bitmap_t *bitmap)
{
    free(bitmap->pixels);
    free(bitmap);
}

typedef struct dk_font_t
{
    int size;
    int ascent;
    int descent;
    int line_gap;
    int glyph_count;
    int kerning_count;
    dk_font_glyph_t *glyphs;
    dk_font_kerning_t *kernings;
} dk_font_t;

static dk_font_t *dk_font_stbtt_load(dk_font_stbtt_t *stbtt, int size)
{
    int ascent = 0;
    int descent = 0;
    int line_gap = 0;
    int glyph_count = 0;
    int kerning_count = 0;
    dk_font_t *font = NULL;
    stbtt_fontinfo info = {0};
    stbtt_InitFont(&info, stbtt->data, stbtt->offset);
    ascent = stbtt_ScaleForPixelHeight(&info, size) * info.ascent;
    descent = stbtt_ScaleForPixelHeight(&info, size) * info.descent;
    line_gap = stbtt_ScaleForPixelHeight(&info, size) * info.lineGap;
    glyph_count = stbtt_GetNumberOfGlyphs(&info);
    kerning_count = stbtt_GetNumberOfCodepoints(&info);
    font = (dk_font_t *)malloc(sizeof(dk_font_t));
    font->size = size;
    font->ascent = ascent;
    font->descent = descent;
    font->line_gap = line_gap;
    font->glyph_count = glyph_count;
    font->kerning_count = kerning_count;
    font->glyphs = (dk_font_glyph_t *)malloc(glyph_count * sizeof(dk_font_glyph_t));
    font->kernings = (dk_font_kerning_t *)malloc(kerning_count * kerning_count * sizeof(dk_font_kerning_t));
    for (int i = 0; i < glyph_count; i++)
    {
        font->glyphs[i].codepoint = i;
        font->glyphs[i].advance = stbtt_ScaleForPixelHeight(&info, size) * stbtt_GetCodepointHMetrics(&info, i, &font->glyphs[i].bearing_x);
        font->glyphs[i].bearing_y = stbtt_ScaleForPixelHeight(&info, size) * stbtt_GetCodepointVMetrics(&info, i, &font->glyphs[i].bearing_x, &font->glyphs[i].bearing_y);
        stbtt_GetCodepointBitmapBox(&info, i, stbtt_ScaleForPixelHeight(&info, size), stbtt_ScaleForPixelHeight(&info, size),
                                    &font->glyphs[i].x, &font->glyphs[i].y, &font->glyphs[i].width, &font->glyphs[i].height);
    }
    for (int i = 0; i < kerning_count; i++)
    {
        for (int j = 0; j < kerning_count; j++)
        {
            font->kernings[(i * kerning_count) + j].codepoint_a = i;
            font->kernings[(i * kerning_count) + j].codepoint_b = j;
            font->kernings[(i * kerning_count) + j].kerning = stbtt_ScaleForPixelHeight(&info, size) * stbtt_GetCodepointKernAdvance(&info, i, j);
        }
    }
    return font;
}

static void dk_font_stbtt_free(dk_font_t *font)
{
    free(font->glyphs);
    free(font->kernings);
    free(font);
}

#endif // DK_FONT_IMPLEMENTATION
#endif // DK_FONT_H

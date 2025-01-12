#include "../Include/stdint.h"
#include "../Timer/timer.h"

#include "graphics.h"

extern BYTE bootscr[];

void SetPixel(int x, int y, BYTE color)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    framebuffer[y * WSCREEN + x] = color;
}

BYTE GetPixel(int x, int y)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    return framebuffer[y * WSCREEN + x];
}

void ClearScreen(void)
{
    for (int y = 0; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, 0x00);
        }
    }
}

void DrawRect(int x, int y, int w, int h, BYTE color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            SetPixel(j, i, color);
        }
    }
}

void DrawBootScr(void)
{
    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            //Calculate Center
            int fx = WSCREEN / 2 - 64;
            int fy = HSCREEN / 2 - 96;

            //Draw Bitmap
            SetPixel(x + fx, y + fy, bootscr[y * 128 + (x + 8)]);
        }
    }

    DrawLoadBar();
    ClearScreen();
}

void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    float sx = (float)w1 / w2;
    float sy = (float)h1 / h2;

    for (int dy = 0; dy < h2; dy++)
    {
        for (int dx = 0; dx < w2; dx++)
        {
            int sx_pos = (int)(x1 + dx * sx);
            int sy_pos = (int)(y1 + dy * sy);

            DWORD color = framebuffer[sy_pos * WSCREEN + sx_pos];

            framebuffer[(y2 + dy) * WSCREEN + (x2 + dx)] = color;
        }
    }
}
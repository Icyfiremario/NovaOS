#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Graphics/graphics.h"
#include "../Interrupts/idt.h"
#include "../Font/text.h"
#include "../Timer/timer.h"
#include "../Userspace/GUI/gui.h"

#include "cursor.h"
#include "mouse.h"

extern WINDOW* window;

BYTE MouseArea[WCURSOR * HCURSOR];
BYTE MouseCursorAfter[WCURSOR * HCURSOR];
BYTE MousePrevious[WCURSOR * HCURSOR];

BYTE mouseData;
BYTE mouseCycle;
BYTE mousePacket[4];

int mousePressed = 0;

int prevMouseX = 0;
int prevMouseY = 0;

int mouseX = 0;
int mouseY = 0;

void GetMouseState(int* x, int* y, int* pressed) 
{
    *x = mouseX;
    *y = mouseY;
    *pressed = mousePressed;
}

void DrawMouse(BYTE color)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        int x = 0;

        for (int i = 0; i < 2; i++)
        {
            BYTE byte = cursor[y * 2 + i];

            for (int j = 7; j >= 0; j--)
            {
                if (byte & (1 << j))
                {
                    SetPixel(mouseX + x, mouseY + y, color);
                }
                x++;
            }
        }
    }
}

void SaveMouseArea()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            BYTE color = GetPixel(mouseX + x, mouseY + y);

            MouseArea[y * WCURSOR + x] = color;
        }
    }
}

void SavePreviousArea(int prevMouseX, int prevMouseY)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            MousePrevious[y * WCURSOR + x] = GetPixel(prevMouseX + x, prevMouseY + y);
        }
    }
}

void RestorePreviousCursor()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            SetPixel(mouseX + x, mouseY + y, MouseArea[y * WCURSOR + x]);
        }
    }
}

void MouseWait()
{
    while (inb(0x64) & 0x02);
}

void ClearTrails(int prevMouseX, int prevMouseY)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            SetPixel(prevMouseX + x, prevMouseY + y, MouseArea[y * WCURSOR + x]);
        }
    }
}

void MouseHandler()
{
    mouseData = inb(0x60);

    if (mouseCycle == 0 && !(mouseData & 0x08))
    {
        return;
    }
    
    mousePacket[mouseCycle++] = mouseData;

    if (mouseCycle == 3)
    {
        mouseCycle = 0;

        mousePressed = (mousePacket[0] & 0x1);

        int deltaX = (int)((char)mousePacket[1]);
        int deltaY = (int)((char)mousePacket[2]);

        int prevMouseX = mouseX;
        int prevMouseY = mouseY;

        mouseX += deltaX;
        mouseY -= deltaY;

        mouseX = (mouseX < 0) ? 0 : ((mouseX > WSCREEN - 16) ? WSCREEN - 16 : mouseX);
        mouseY = (mouseY < 0) ? 0 : ((mouseY > HSCREEN - 16) ? HSCREEN - 16 : mouseY);

        //HandleWindowDragging(window, mouseX, mouseY, mousePressed);

        ClearTrails(prevMouseX, prevMouseY);
        SaveMouseArea();
        RestorePreviousCursor(prevMouseX, prevMouseY);
        DrawMouse(0x1E);
    }
}

void InitMouse()
{
    outb(0x64, 0xA7);
    outb(0x64, 0xA8);

    while (inb(0x64) & 1)
    {
        inb(0x60);
    }

    IRQInstallHandler(12, &MouseHandler);

    outb(0x64, 0x20);
    BYTE status = (inb(0x60) | 2);
    outb(0x64, 0x60);
    outb(0x60, status);

    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
}
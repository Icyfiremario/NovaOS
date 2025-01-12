#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Hardware/cmos.h"
#include "../Drivers/keyboard.h"
#include "../Drivers/mouse.h"
#include "../Memory/mem.h"
#include "../Font/text.h"
#include "../FileSystem/memfs.h"
#include "GUI/gui.h"

#include "run.h"
#include "userspace.h"

extern BYTE backgrd[];

WINDOW startMenu = { "", 0x12, 0, HSCREEN - 285, 150, 250, 1, TRUE, 0, 0, 0 };
WINDOW* winmgr;

int inUserSpace = 0x00;

int isStartMenuOpen = 0;
int isMaximizedOpen = 0;   
int wasMousePressed = 0;

int currentWindowID = 0;
int totalWindows = 0;

BYTE startBuffer[150 * 200];

void DrawBackground()
{
    for (int y = 0; y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, backgrd[y * WSCREEN + (x + 8)]);
        }
    }
}

void DrawTaskbar()
{
    for (int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, 0x13);
        }
    }

    SetCursorX(30);
    SetCursorY(HSCREEN - 25);

    Print("START", 0x0F);

    SetCursorX(WSCREEN - 65);
    SetCursorY(HSCREEN - 25);

    GetCMOSDate();
}

void SaveStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            startBuffer[y * 150 + x] = GetPixel(x, y);
        }
    }
}

void RestoreStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            SetPixel(x, y, startBuffer[y * 150 + x]);
        }
    }
}

void StartButtonHandler(int x, int y, int pressed)
{
    int startX = 0;
    int endX = 100;

    int startY = HSCREEN - 35;
    int endY = HSCREEN;

    if (pressed && !wasMousePressed && x >= startX && x < endX && y >= startY && y < endY)
    {
        if (isStartMenuOpen)
        {
            RestoreStartArea();
            isStartMenuOpen = 0;
        }
        else
        {
            SaveStartArea();
            DrawStartMenu(startMenu);
            isStartMenuOpen = 1;
        }
    }
    
    wasMousePressed = pressed;
}

void GetWindow(WINDOW window)
{
    winmgr[currentWindowID] = window;

    winmgr[currentWindowID].orgX = window.x;
    winmgr[currentWindowID].orgY = window.y;
    winmgr[currentWindowID].orgW = window.w;
    winmgr[currentWindowID].orgH = window.h;

    currentWindowID++;
    totalWindows++;
}

void MaximizeWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - (2 * buttonSize) - 20;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            if (win->isMaximized % 2 == 0)
            {
                //SaveTotalArea();

                win->x = 0;
                win->y = 0;
                win->w = WSCREEN;
                win->h = HSCREEN - 32;

                win->isMaximized++;

                DrawWindow(*win, 0x00);
            }
            else
            {
                win->x = GetStartWindowXY();
                win->y = GetStartWindowXY();
                win->w = 320;
                win->h = 200;

                win->isMaximized++;

                DrawBackground();

                DrawWindow(*win, 0x00);
            }

            if (strcmp(win->title, "NovaOS - Run") == 0x00)
            {
                //StartShell(*win);
            }
        }
    }
}

void CloseWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - buttonSize - 10;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            UpdateExplorer();
        }
    }
}

void MinimizeWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - (3 * buttonSize) - 30;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            UpdateExplorer();
        }
    }
}

void UpdateExplorer()
{
    DrawBackground();
    DrawTaskbar();
    DesktopIcons();
}

void UserspaceState(int state)
{
    if (state == FALSE)
    {
        inUserSpace = 0x00;
    }
    else
    {
        inUserSpace = 0x01;
    }
}

void DesktopIcons()
{
    FileSystem* fs = (FileSystem*) FSADDRESS;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (fs->files[i].filename[0] != '\0')
        {
            DrawDesktopIcon(ICON_TEXT, fs->files[i].filename, 10, 10 + (i * 76));
        }
    }
}

void UserSpace()
{
    while (!inUserSpace)
    {

    }

    UpdateExplorer();

    KeyboardState(TRUE);

    while (true)
    {
        int x, y, pressed;
        GetMouseState(&x, &y, &pressed);

        StartButtonHandler(x, y, pressed);
    
        //MaximizeWindow(x, y, pressed);
        MinimizeWindow(x, y, pressed);
        CloseWindow(x, y, pressed);
    }
}
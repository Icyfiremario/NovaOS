typedef enum
{
    ICON_NONE,
    ICON_ERROR,
    ICON_WARNING,
    ICON_INFO,
    ICON_QUESTION,
    ICON_USER,
    ICON_TERMINAL
} ICON;

typedef enum
{
    ICON_TEXT
} EXTICON;

typedef struct
{
    const BYTE* title;
    BYTE color;

    int x, y;
    int w, h;

    int state; //0 = invisible, 1 = minimized, 2 = fullscreen
    int focus;

    int isDragging;

    int dragOffsetX;
    int dragOffsetY;

    int isMaximized;

    int orgX, orgY;
    int orgW, orgH;
} WINDOW;

typedef struct GUIEvent
{
    int type;
    int mouseX, mouseY;
    int keyCode;
    int windowID; 
} GUIEvent;

typedef struct GraphicsBuffer
{
    int width, heights;
    BYTE* data;
} GraphicsBuffer;

void DrawWindow(WINDOW window, int nextWindow);
void DrawRoundedRect(int x, int y, int w, int h, int radius, BYTE color);
void DrawButton(int x, int y, int size, const char* icon, BYTE color);
void DrawMessageBox(WINDOW msgbox, ICON icon, char* text, int nextWindow);
void DrawIcon(int startX, int startY, ICON icon);
void DrawWindowButtons(WINDOW window);
void WindowClear(WINDOW window);
void DrawStartMenu(WINDOW window);
void DrawDesktopIcon(EXTICON exticon, char* filename, int x, int y);

int GetStartWindowXY();
#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "../Userspace/GUI/gui.h"
#include "../Graphics/graphics.h"

#include "run.h"

void ProcessRun(char* command)
{
    char cmd[16] = {0};
    int cmdIndex = 0;
    int i = 0;

    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[cmdIndex++] = command[i++];
    }

    cmd[cmdIndex] = '\0';

    if (strcmp(cmd, "NULL") == 0x00)
    {
        //Nothing YET...
    }
    else
    {
        int msgW = 400;
        int msgH = 110;

        int x = 320 - 200;
        int y = 240 - 55;

        WINDOW msgbox = { "NovaOS Run - Error", 0x13, x, y, msgW, msgH  };

        DrawMessageBox(msgbox, ICON_ERROR, "This program doesn't exist or try again.", 0x00);
    }
}
#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Interrupts/idt.h"
#include "../Font/text.h"
#include "../Shell/shell.h"
#include "../Graphics/graphics.h"

#include "../Userspace/GUI/gui.h"
#include "../Userspace/run.h"
#include "../Userspace/userspace.h"

#include "keyboard.h"

int allowInput = 1;
int enableText = 0;
int shift = 0;
int caps = 0;
int winPressed = 0;
int rPressed = 0;

char commandBuffer[50];
int commandLength = 0;

const char* lowercase[] = {
    " ", " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "\b", " ", "q", "w", "e", "r", "t", "y", "u", "i",
    "o", "p", "[", "]", "\n", " ", "a", "s", "d", "f", "g", "h",
    "j", "k", "l", ";", "'", "`", " ", "\\", "z", "x", "c", "v",
    "b", "n", "m", ",", ".", "/", " ", "*", " ", " "
};

const char* uppercase[] = {
    " ", " ", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
    "_", "+", "\b", " ", "Q", "W", "E", "R", "T", "Y", "U", "I",
    "O", "P", "{", "}", "\n", " ", "A", "S", "D", "F", "G", "H",
    "J", "K", "L", ":", "\"", "~", " ", "|", "Z", "X", "C", "V",
    "B", "N", "M", "<", ">", "?", " ", " ", " ", " "
};

void KeyboardState(int state)
{
    if (state == TRUE)
    {
        enableText = 1;
    }
    else if (state == 2)
    {
        enableText = 2;
    }
    else
    {
        enableText = 0;
    }
}

void KeyboardHandler()
{
    uint8_t scan = inb(0x60);
    uint8_t isPress = !(scan & 0x80); // Verifica se a tecla foi pressionada
    scan &= 0x7F; // Remove o bit de liberação

    if (allowInput)
    {
        switch (scan)
        {
            // Shift
            case 42:
            case 54:
                shift = isPress;
                break;

            // Caps lock
            case 58:
                if (isPress)
                {
                    caps = !caps;
                }
                break;

            case 0x1C: // Enter
                if (isPress && enableText != 2) 
                {
                    commandBuffer[commandLength] = '\0';
                    ProcessRun(commandBuffer);
                    commandLength = 0;
                }
                if (isPress && enableText == 2)
                {
                    commandBuffer[commandLength] = '\0';
                    ProcessShellCMD(commandBuffer);
                    commandLength = 0;
                }

                break;

            // Tecla Win (código especial 0xE0 seguido de 0x5B ou 0x5C)
            case 0x5B: // Win esquerda
            case 0x5C: // Win direita
                winPressed = isPress;
                break;

            // Tecla R
            case 0x13: // Scancode para 'R'
                rPressed = isPress;

                if (isPress && enableText && commandLength < 49) 
                {
                    if (shift || caps) 
                    {
                        commandBuffer[commandLength] = uppercase[scan][0];
                    } 
                    else 
                    {
                        commandBuffer[commandLength] = lowercase[scan][0];
                    }

                    PrintOut(commandBuffer[commandLength], 0x0F);
                    commandLength++;
                }
                break;

            default:
                if (isPress && enableText && commandLength < 49)
                {
                    if (shift || caps)
                    {
                        commandBuffer[commandLength] = uppercase[scan][0];
                        PrintOut(commandBuffer[commandLength], 0x0F);
                        commandLength++;
                    }
                    else
                    {
                        commandBuffer[commandLength] = lowercase[scan][0];
                        PrintOut(commandBuffer[commandLength], 0x0F);
                        commandLength++;
                    }
                }
                break;
        }

        // Verifica se Win e R estão pressionados simultaneamente
        if (winPressed && enableText != 2)
        {
            int StartWindowXY = GetStartWindowXY();
            WINDOW run = { "NovaOS - Run", 0x13, 10, HSCREEN - 210, 340, 150, 1, TRUE };

            DrawWindow(run, 0x01);

            DrawIcon(run.x + 15,  run.y + 40, ICON_TERMINAL);
            
            SetCursorX(run.x + 60);
            SetCursorY(run.y + 40);

            Print("Type a name of a program to run.", 0x0F);

            SetCursorX(run.x + 32);
            SetCursorY(run.y + 100);

            DrawRoundedRect(run.x + 16, run.y + 100, 300, 20, 10, 0x12);

            GetWindow(run);
        }
    }
}

void InitKeyboard()
{
    IRQInstallHandler(0x01, &KeyboardHandler);
}

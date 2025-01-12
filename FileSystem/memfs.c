#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "memfs.h"

FileSystem* fs = (FileSystem*) FSADDRESS;

void InitFileSystem()
{
    fs->nextFreeBlock = FSADDRESS + sizeof(FileSystem);

    for (int i = 0; i < MAXFILES; i++)
    {
        memset(fs->files[i].filename, 0x00, MAXFILENAME);
        fs->files[i].size = 0x00000000;
        fs->files[i].dataOffset = 0x00000000;
    }
}

int CreateFile(const char* filename, const LPBYTE data, DWORD size)
{
    //Checks for invalid file name
    if (size == 0 || strlen(filename) >= MAXFILENAME)
    {
        return -1;
    }

    int fileIndex = -1;

    //Checks for enough space
    for (int i = 0; i < MAXFILES; i++)
    {
        if (fs->files[i].filename[0] == '\0')
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1)
    {
        //No more space for more files
        return -2;
    }

    DWORD dataOffset = fs->nextFreeBlock;
    fs->nextFreeBlock += size;

    strncpy(fs->files[fileIndex].filename, filename, MAXFILENAME);
    fs->files[fileIndex].size = size;
    fs->files[fileIndex].dataOffset = dataOffset;

    LPBYTE fileData = (LPBYTE) dataOffset;
    memcpy(fileData , data, size);

    return 0x00;
}

int ReadFile(const char* filename, LPBYTE buffer, LPDWORD size)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(fs->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            *size = fs->files[i].size;

            LPBYTE file_data = (LPBYTE) fs->files[i].dataOffset;
            memcpy(buffer, file_data, *size);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

    return -1;
}

int DeleteFile(const char* filename)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(fs->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            memset(fs->files[i].filename, 0x00, MAXFILENAME);

            fs->files[i].size = 0x00000000;
            fs->files[i].dataOffset = 0x00000000;

            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

    return -1; 
}

int FileInfo(const char* filename) 
{
    Print("\n", 0x00);

    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(fs->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            Print("\nFilename: ", 0x0A);
            Print(fs->files[i].filename, 0x0F);

            Print("\nSize: ", 0x0A);
            PrintInt(fs->files[i].size, 0x0F);

            Print("\nData Offset: ", 0x0A);
            PrintInt(fs->files[i].dataOffset, 0x0F);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

    return -1;
}

int FindFile(const char* filename) 
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(fs->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            return i;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

    return -1;
}

int RenameFile(const char* oldFilename, const char* newFilename) 
{
    int fileIndex = FindFile(oldFilename);

    if (fileIndex == -1) 
    {
        return -1;
    }

    if (strlen(newFilename) >= MAXFILENAME) 
    {
        return -2; 
    }

    strncpy(fs->files[fileIndex].filename, newFilename, MAXFILENAME);

    return 0x00;
}

void ListFiles()
{
    Print("\n\n", 0x00);

    for (int i = 0; i < MAXFILES; i++)
    {
        if (fs->files[i].filename[0] != '\0')
        {
            Print(fs->files[i].filename, 0x0B);
            Print(" ", 0x00);
        }
    }
}
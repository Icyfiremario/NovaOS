#define MAXFILENAME 0x0F
#define MAXFILES    0x0A
#define MAXSUBDIR   0x05
#define FSADDRESS   0x2000

typedef struct
{
    char filename[MAXFILENAME];
    DWORD size;
    DWORD dataOffset;
} FileHeader;

typedef struct
{
    FileHeader files[MAXFILES];
    DWORD nextFreeBlock;
} FileSystem;

void InitFileSystem();
int CreateFile(const char* filename, const LPBYTE data, DWORD size);
int ReadFile(const char* filename, LPBYTE buffer, LPDWORD size);
int DeleteFile(const char* filename);
int FindFile(const char* filename);
int FileInfo(const char* filename);
int RenameFile(const char* oldFilename, const char* newFilename);
void ListFiles();
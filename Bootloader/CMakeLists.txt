
set(CMAKE_ASM_FLAGS "-felf -c")
set(CMAKE_LINKER_LANGUAGE "ASM_NASM")
add_library(Bootloader gdt.asm pmode.asm)
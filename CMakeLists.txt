cmake_minimum_required(VERSION 3.10)

project(NovaOS VERSION 1.0)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_ASM_COMPILER "nasm")

set(CMAKE_C_FLAGS "-ffreestanding -m32 -g -c -target i686-none-elf")


add_subdirectory(Bootloader)
@ECHO OFF

cls

echo "Compiling ASM Files"
nasm -fbin Bootloader/boot.asm   -o Binaries/boot.bin
nasm -felf Kernel/entry.asm      -o Binaries/entry.o
nasm -felf Interrupts/idt.asm    -o Binaries/asmidt.o 

echo "Compiling C Files"
i686-elf-gcc -ffreestanding -m32 -g -c Kernel/kernel.c       -o Binaries/kernel.o
i686-elf-gcc -ffreestanding -m32 -g -c Graphics/graphics.c   -o Binaries/graphics.o
i686-elf-gcc -ffreestanding -m32 -g -c Include/ports.c       -o Binaries/ports.o
i686-elf-gcc -ffreestanding -m32 -g -c Font/text.c           -o Binaries/text.o
i686-elf-gcc -ffreestanding -m32 -g -c Memory/mem.c          -o Binaries/mem.o
i686-elf-gcc -ffreestanding -m32 -g -c Interrupts/idt.c      -o Binaries/idt.o
i686-elf-gcc -ffreestanding -m32 -g -c Timer/timer.c         -o Binaries/timer.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/keyboard.c    -o Binaries/keyboard.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/mouse.c       -o Binaries/mouse.o
i686-elf-gcc -ffreestanding -m32 -g -c Drivers/qemu.c        -o Binaries/qemu.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cpu.c        -o Binaries/cpu.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/pci.c        -o Binaries/pci.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/disk.c       -o Binaries/disk.o
i686-elf-gcc -ffreestanding -m32 -g -c Hardware/cmos.c       -o Binaries/cmos.o
i686-elf-gcc -ffreestanding -m32 -g -c FileSystem/memfs.c    -o Binaries/memfs.o
i686-elf-gcc -ffreestanding -m32 -g -c Shell/shell.c         -o Binaries/shell.o

i686-elf-gcc -ffreestanding -m32 -g -c Userspace/userspace.c        -o Binaries/userspace.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/GUI/gui.c          -o Binaries/gui.o
i686-elf-gcc -ffreestanding -m32 -g -c Userspace/run.c              -o Binaries/run.o

echo "Linking ELF Files"
i686-elf-ld -o Binaries/fullkernel.bin -Ttext 0x7E00 Binaries/entry.o Binaries/kernel.o Binaries/graphics.o Binaries/ports.o Binaries/text.o Binaries/mem.o Binaries/asmidt.o Binaries/idt.o Binaries/timer.o Binaries/keyboard.o Binaries/cpu.o Binaries/pci.o Binaries/disk.o Binaries/cmos.o Binaries/mouse.o Binaries/qemu.o Binaries/memfs.o Binaries/shell.o Binaries/gui.o Binaries/userspace.o Binaries/run.o --oformat binary

echo "Mounting IMG"
cat Binaries/boot.bin Binaries/fullkernel.bin > "NovaOS.img"

echo "Running NovaOS"
qemu-system-i386 -net nic,model=rtl8139 -net user -drive format=raw,file="NovaOS.img" 

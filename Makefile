all: run

run: deploy_all
	bochs -f bochsrc

deploy_all: deploy_kernel
	

deploy_kernel: compile_kernel deploy_bootloader
	strip ./KERNEL/kernel.bin
	cat ./KERNEL/kernel.bin >> floppy.bin

compile_kernel:
	gcc -nostdlib -std=c99 ./KERNEL/kernel_main.c -o ./KERNEL/kernel.bin

deploy_bootloader: compile_bootloader
	cat ./BOOTLOADER/stage1.bin ./BOOTLOADER/stage2.bin > floppy.bin

compile_bootloader: ./BOOTLOADER/stage1.asm ./BOOTLOADER/stage2.asm
	fasm ./BOOTLOADER/stage1.asm
	fasm ./BOOTLOADER/stage2.asm

clean_temp:
	rm ./BOOTLOADER/stage1.bin 
	rm ./BOOTLOADER/stage2.bin
	rm ./KERNEL/kernel.bin
	rm floppy.bin
	rm bochsout.txt

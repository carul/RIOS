run: deploy
	bochs -f bochsrc

deploy: compile
	cat stage1.bin stage2.bin > floppy.bin

compile: stage1.asm stage2.asm
	fasm stage1.asm
	fasm stage2.asm

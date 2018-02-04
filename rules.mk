CROSS_COMPILE := /home/BlueFox/opensrc/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf-

CC := ${CROSS_COMPILE}gcc
AS := ${CROSS_COMPILE}as
LD := ${CROSS_COMPILE}ld
OBJCOPY := ${CROSS_COMPILE}objcopy
OBJDUMP := ${CROSS_COMPILE}objdump

#OPTIMIZATION_FLAGS = -O2
CFLAGS := -mcpu=cortex-a8
CFLAGS += -mfpu=neon
CFLAGS += ${OPTIMIZATION_FLAGS}
ASFLAGS := -mcpu=cortex-a8
#ASFLAGS += -mfpu=neon
#LDFLAGS = -T first.lds
LDFLAGS = -T linker.lds
LDFLAGS = -L /home/BlueFox/opensrc/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/lib/gcc/arm-linux-gnueabihf/4.8.3/libgcc.a -lgcc
LDFLAGS += -Map=$*.map

TARGET ?= code

default: ${TARGET}.bin

%.bin: %.elf
	${OBJCOPY} --gap-fill=0xFF -O binary $< $@

#%.elf: %.o
#%.elf: %.o first_startup.o
#%.elf: %.o common.o first_startup.o
%.elf: %.o common.o startup.o interrupt.o
	${LD} ${LDFLAGS} $^ -o $@

#%.o: %.c
#	${CC} ${CFLAGS} -c $< -o $@
#
#%.o: %.s
#	${AS} ${ASFLAGS} -c $< -o $@
#
view: ${TARGET}_disasm

%_disasm: %.elf
	${OBJDUMP} -d $<

clean:
	${RM} *.bin *.map *.elf *.o

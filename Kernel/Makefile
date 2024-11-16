include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_MM=$(wildcard memory_manager/*.c)
SOURCES_STRUCTS=$(wildcard structs/*.c)
SOURCES_SCHEDULER=$(wildcard scheduler/*.c)
SOURCES_IPC=$(wildcard ipc/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_MM=$(SOURCES_MM:.c=.o)
OBJECTS_STRUCTS=$(SOURCES_STRUCTS:.c=.o)
OBJECTS_SCHEDULER=$(SOURCES_SCHEDULER:.c=.o)
OBJECTS_IPC=$(SOURCES_IPC:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=
MM_MANAGER=LIST

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_MM) $(OBJECTS_STRUCTS) $(OBJECTS_SCHEDULER) $(OBJECTS_IPC)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_MM) $(OBJECTS_STRUCTS) $(OBJECTS_SCHEDULER) $(OBJECTS_IPC)
 
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_MM) $(OBJECTS_STRUCTS) $(OBJECTS_SCHEDULER) $(OBJECTS_IPC)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -D $(MM_MANAGER) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin *.elf obj memory_manager/*.o memory_manager/obj scheduler/*.o scheduler/obj ipc/*.o ipc/obj structs/*.o structs/obj

.PHONY: all clean 

include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_MM=$(wildcard memory_manager/*.c)
SOURCES_TEST=$(wildcard tests/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_MM=$(SOURCES_MM:.c=.o)
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)
LOADERSRC=loader.asm
TARGET := MemoryManagerTest

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_MM) $(OBJECTS_TEST) $(TARGET)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_MM) $(OBJECTS_TEST)
 
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_MM) $(OBJECTS_TEST)

    $(TARGET): $(OBJECTS)
		$(LINKER) $(LINKER_FLAGS) $(OBJECTS) -o ../$(TARGET).out

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin *.elf memory_manager/*.o tests/*.o

.PHONY: all clean $(TARGET)

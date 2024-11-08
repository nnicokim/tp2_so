include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_TEST=$(wildcard tests/*.c)
OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=obj/%.asm.o)
OBJECT_TEST=$(SOURCES_TEST:.c=.o)

all: $(MODULE)

# Linking the final module with object files from tests and assembly
$(MODULE): $(SOURCES) $(OBJECTS_ASM) $(OBJECT_TEST)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(OBJECTS_ASM) $(OBJECT_TEST) $(SOURCES) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS_ASM) $(OBJECT_TEST) $(SOURCES) -o ../0000-sampleCodeModule.elf

# Rule to compile .asm files to object files
obj/%.asm.o : asm/%.asm
	mkdir -p obj
	$(ASM) $(ASMFLAGS) $< -o $@

# Rule to compile .c files in tests/ directory to .o files
tests/%.o: tests/%.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

# Clean all object files and generated binaries
clean:
	rm -rf *.o
	rm -rf obj
	rm -f tests/*.o

.PHONY: all clean print

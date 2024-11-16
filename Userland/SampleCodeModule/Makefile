include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=obj/%.asm.o)

SOURCES_TEST=$(wildcard tests/*.c)
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM) $(OBJECTS_TEST) 
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(OBJECTS_ASM) $(OBJECTS_TEST) $(SOURCES) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS_ASM) $(OBJECTS_TEST) $(SOURCES) -o ../0000-sampleCodeModule.elf
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(OBJECTS_ASM) $(OBJECTS_TEST)  -o ../0000-sampleCodeModule.elf
	
obj/%.asm.o : asm/%.asm
	mkdir -p obj
	$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

clean:
	rm -rf *.o asm/*.o obj/*.o ../0000-sampleCodeModule.bin ../0000-sampleCodeModule.elf tests/*.o	
	rm -rf obj

.PHONY: all clean print

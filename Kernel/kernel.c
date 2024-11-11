#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>

#include <keyboardDriver.h>
#include "include/idtLoader.h"
#include <interrupts.h>
#include <sound.h>
#include <time.h>
#include "./memory_manager/include/mm_manager.h"
// #include "./include/test_mm.h"
// #include "./memory_manager/include/buddyAllocator.h"
#include "./structs/include/stack.h"
#include "./structs/include/pcb.h"
#include "./scheduler/include/scheduler.h"
#include "./ipc/include/semaphore.h"
#include "./ipc/include/pipes.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

CircularList round_robin;	   // Definición de la lista de procesos en round-robin
PCB *PCB_array[MAX_PROCESSES]; // Definición del Array de PCBs

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
					- sizeof(uint64_t)					  // Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{
	_cli(); // Deshabilitar interrupciones

	load_idt(); // Cargar la tabla de descriptores de interrupciones (IDT)

	my_mm_init(HEAP_START, BLOCK_COUNT * BLOCK_SIZE); // Inicializar el gestor de memoria (tenemos memoria de sobra)

	// size_t total_memory = 1024; // Memoria total disponible para el buddy allocator
	// init_buddy_allocator(HEAP_START, total_memory);

	initScheduler();

	initSems();

	_sti(); // Habilitar interrupciones

	printArray("You shouldn't be here chief...");

	return 0;
}

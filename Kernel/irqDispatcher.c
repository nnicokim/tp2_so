#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include "./scheduler/include/scheduler.h"

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq)
{
	switch (irq)
	{
	case 0:
		int_20();
		break;
	case 1:
		int_21();
		break;
	}
	return;
}

void int_20()
{
	timer_handler();
	// if (isSchedulerActive)
	// 	schedule();
}

void int_21()
{
	keyboard_handler();
	// if (isSchedulerActive)
	// 	schedule();
}
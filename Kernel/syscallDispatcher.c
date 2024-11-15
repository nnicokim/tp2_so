#include <stdint.h>
#include "./ipc/include/pipes.h"
#include <videoDriver.h>
#include <registers.h>
#include <time.h>
#include <keyboardDriver.h>
#include <clock.h>
#include <sound.h>
#include <naiveConsole.h>
#include "./structs/include/pcb.h"
#include "./scheduler/include/scheduler.h"
#include "./structs/include/stack.h"
#include "./structs/include/circularList.h"
#include "./memory_manager/include/mm_manager.h"
#include "./structs/include/circularList.h"

#include <rick.h>
#include "./ipc/include/semaphore.h"
#include "./ipc/include/pipes.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LASTIN 3
#define TIME_STR 10

#define PAGE 0x1000

extern char getScanCode();
extern int getTime(int timeUnit);
extern void _hlt();
extern void forceTimerTick();

uint64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count);
uint64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count);
uint64_t ksys_change_draw_size(uint64_t size);
uint64_t ksys_print_array_of_draw_size(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf);
uint64_t ksys_write_color(uint64_t fontColor, uint64_t buffer, uint64_t count);
uint64_t ksys_getRegisters(uint64_t rdi);
uint64_t ksys_wait(uint64_t seconds);
uint64_t ksys_clear_screen();
uint64_t ksys_change_font_size(uint64_t font);
uint64_t ksys_get_font_size();
uint64_t ksys_beep(uint64_t frec, uint64_t time_ms);
uint64_t ksys_getTime();
uint64_t ksys_draw_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);
uint64_t ksys_draw_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);
uint64_t ksys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);
// SO
uint64_t ksys_createOneProcess();
uint64_t ksys_createProcess(char *pr_name, void *process, int argc, char **argv, int *fds);
uint64_t ksys_blockProcess(int pid);
uint64_t ksys_unblockProcess(int pid);
uint64_t ksys_getCurrentpid();
uint64_t ksys_getCurrentPpid();
uint64_t ksys_killProcess(int pid);
uint64_t ksys_leaveCPU();
uint64_t ksys_waitPid(int pid);
uint64_t ksys_myExit();
uint64_t ksys_my_nice(uint64_t pid, uint64_t newPrio);
uint64_t ksys_increase_priority(int pid);
uint64_t ksys_decrease_priority(int pid);
uint64_t ksys_print_processes();
uint64_t ksys_print_memory();
uint64_t ksys_loop_print();
uint64_t ksys_testsync(uint64_t argc, char **argv);
uint64_t ksys_memset(uint64_t address, uint64_t value, uint64_t size);

uint64_t ksys_pollPipe(uint64_t id, uint64_t event);
uint64_t ksys_readPipe(uint64_t id, char *dest, uint64_t count);
uint64_t ksys_writePipe(uint64_t id, char *src, uint64_t count);

uint64_t ksys_mymalloc(uint64_t size);
uint64_t ksys_myfree(uint64_t ptr);

uint64_t ksys_semOpen(char *name, int initValue);
uint64_t ksys_semClose(char *name);
uint64_t ksys_semWait(int semIndex);
uint64_t ksys_semPost(int semIndex);
uint64_t ksys_findSem(char *name);
uint64_t ksys_yield();

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax)
{
    switch (rax)
    {
    case 0:
        return ksys_read(rdi, rsi, rdx);
    case 1:
        return ksys_write(rdi, rsi, rdx);
    case 2:
        return ksys_getTime();
    case 3:
        return ksys_change_draw_size(rdi);
    case 4:
        return ksys_print_array_of_draw_size(rdi, rsi, rdx, rcx, r8);
    case 5:
        return ksys_draw_array(rdi, rsi, rdx, rcx, r8);
    case 6:
        return ksys_write_color(rdi, rsi, rdx);
    case 7:
        return ksys_clear_screen();
    case 8:
        return ksys_getRegisters(rdi);
    case 9:
        return ksys_wait(rdi);
    case 10:
        return ksys_change_font_size(rdi);
    case 11:
        return ksys_get_font_size();
    case 12:
        return ksys_beep(rdi, rsi);
    case 13:
        return ksys_draw_square(rdi, rsi, rdx, rcx);
    case 14:
        return ksys_draw_rect(rdi, rsi, rdx, rcx, r8);
    case 15:
        flushBuffer();
        return 0;

    // Syscalls nuevas (SO)
    case 16:
        return ksys_createOneProcess();
    case 17:
        return ksys_blockProcess(rdi);
    case 18:
        return ksys_unblockProcess(rdi);
    case 19:
        return ksys_getCurrentpid();
    case 20:
        return ksys_getCurrentPpid();
    case 21:
        return ksys_killProcess(rdi);
    case 22:
        return ksys_leaveCPU();
    case 23:
        return ksys_waitPid(rdi);
    // case 24:
    //     return test_mm(rdi, (char **)rsi);
    // case 25:
    //     return test_processes(rdi, (char **)rsi);
    case 26:
        return ksys_myExit();
    // case 27:
    //     sys_test_prio();
    //     return 0;
    case 28:
        return ksys_my_nice(rdi, rsi);
    case 29:
        return ksys_increase_priority(rdi);
    case 30:
        return ksys_decrease_priority(rdi);
    case 31:
        return ksys_print_processes();
    case 32:
        return ksys_print_memory();
    case 33:
        return ksys_loop_print();
    // case 34:
    //     return ksys_testsync(rdi, (char **)rsi);
    case 35:
        return ksys_createProcess((char *)rdi, (void *)rsi, rdx, (char **)rcx, r8);
    case 36:
        return ksys_pollPipe(rdi, rsi);
    case 37:
        return ksys_readPipe(rdi, (char *)rsi, rdx);
    case 38:
        return ksys_writePipe(rdi, (char *)rsi, rdx);
    case 39:
        return rick();
    case 40:
        return ksys_mymalloc(rdi);
    case 41:
        return ksys_myfree(rdi);
    case 42:
        return ksys_semOpen((char *)rdi, rsi);
    case 43:
        return ksys_semClose((char *)rdi);
    case 44:
        return ksys_semWait(rdi);
    case 45:
        return ksys_semPost(rdi);
    case 46:
        return ksys_findSem((char *)rdi);
    case 47:
        return ksys_yield();
    case 48:
        return ksys_memset(rdi, rsi, rdx);
    }

    return 0;
}

uint64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count)
{
    int i = 0;
    char c;
    char *buff = (char *)buffer;
    if (fd == STDIN)
    {
        while (i < count && (c = getChar()) != 0)
        {
            buff[i++] = c;
        }
        return i;
    }
    else if (fd == LASTIN)
    {
        while (i < count && (c = getLastChar()) != 0)
        {
            buff[i++] = c;
        }
        return i;
    }
    return 0;
}

uint64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count)
{
    if (fd == STDOUT)
    {
        printArrayOfDimWithColor(WHITE, BLACK, (char *)buffer, count);
    }
    else if (fd == STDERR)
    {
        printArrayOfDimWithColor(RED, BLACK, (char *)buffer, count);
    }
    return count;
}

uint64_t ksys_getTime()
{
    char reserve[TIME_STR]; // en reserve queda guardado el time en formato hh:mm:ss
    timeToStr(reserve);
    printArray(reserve);
    return 0;
}

uint64_t ksys_change_draw_size(uint64_t size)
{
    changeDrawSize(size);
    return 0;
}

uint64_t ksys_print_array_of_draw_size(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf)
{
    drawArrayOfSize(fontColor, backgroundColor, x, y, (char *)buf);
    return 0;
}

uint64_t ksys_write_color(uint64_t fontColor, uint64_t buffer, uint64_t count)
{
    printArrayOfDimWithColor(fontColor, BLACK, (char *)buffer, count);
    return count;
}

uint64_t ksys_getRegisters(uint64_t rdi)
{
    uint64_t toRet = getRegisters((uint64_t *)rdi);
    return toRet;
}

uint64_t ksys_wait(uint64_t ms)
{
    timer_wait_ms(ms);
    return 0;
}

uint64_t ksys_clear_screen()
{
    clearScreen();
    return 0;
}

uint64_t ksys_change_font_size(uint64_t font)
{
    changeFontSize(font);
    return 0;
}

uint64_t ksys_get_font_size()
{
    return getFontSize();
}

uint64_t ksys_beep(uint64_t frec, uint64_t time_ms)
{
    beep((uint32_t)frec, (int)time_ms);
    return 0;
}

uint64_t ksys_draw_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size)
{
    drawSquare(color, x, y, size);
    return 0;
}

uint64_t ksys_draw_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y)
{
    drawRect(color, x, y, size_x, size_y);
    return 0;
}

uint64_t ksys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr)
{
    drawArray(fontColor, backgroundColor, x, y, (char *)arr);
    return 0;
}

uint64_t ksys_createOneProcess()
{
    return createOneProcess();
}

uint64_t ksys_createProcess(char *pr_name, void *process, int argc, char **argv, int *fds)
{
    return createProcess((char *)pr_name, (void *)process, argc, argv, fds);
}

uint64_t ksys_blockProcess(int pid)
{
    return blockProcess(pid);
}

uint64_t ksys_unblockProcess(int pid)
{
    return unblockProcess(pid);
}

uint64_t ksys_getCurrentpid()
{
    return getCurrentPid();
}

uint64_t ksys_getCurrentPpid()
{
    return getCurrentPPid();
}

uint64_t ksys_killProcess(int pid)
{
    return killProcess(pid);
}

uint64_t ksys_leaveCPU()
{
    forceTimerTick();
    return 0;
}

// uint64_t ksys_waitPid(int pid)
// {
//     // printArray("Waiting for pid: ");
//     // printDec(pid);
//     // printArray("\n");

//     PCB *childProcess = PCB_array[pid];

//     if (!childProcess)
//     {
//         printArray("Process not found\n");
//         return -1;
//     }

//     blockProcess(childProcess->ppid);
//     printArray("Parent blocked (wait pid)\n");

//     while (!childProcess || childProcess->state != FINISHED)
//     {
//         childProcess = PCB_array[pid];
//         // printArray(childProcess->name);
//         // printArray(" is still running\n");
//         forceTimerTick();
//     }

//     unblockProcess(childProcess->ppid);
//     printArray("Parent UNblocked (wait pid)\n");

//     return 0;
// }

uint64_t ksys_waitPid(int pid)
{
    PCB *childProcess = PCB_array[pid];
    if (childProcess->state == FINISHED)
    {
        return -1;
    }
    int PPid = getCurrentPid();
    blockProcess(PPid);
    forceTimerTick();
}

uint64_t ksys_myExit()
{
    my_exit();
    return 0;
}

uint64_t ksys_my_nice(uint64_t pid, uint64_t newPrio)
{
    my_nice(pid, newPrio);
    return 0;
}

uint64_t ksys_increase_priority(int pid)
{
    return increase_priority(pid);
}

uint64_t ksys_decrease_priority(int pid)
{
    return decrease_priority(pid);
}

uint64_t ksys_print_processes()
{
    print_processes();
    return 0;
}

uint64_t ksys_print_memory()
{
    mem();
    return 0;
}

uint64_t ksys_loop_print()
{
    loop_print();
    return 0;
}

// uint64_t ksys_testsync(uint64_t argc, char *argv[])
// {
//     int fds[] = {0, 1};
//     int sync_pid = createProcess("Sync_Test", (void *)test_sync, argc, argv, fds);
//     return sync_pid;
// }

uint64_t ksys_pollPipe(uint64_t id, uint64_t event)
{
    return pollPipe(id, event);
}

uint64_t ksys_readPipe(uint64_t id, char *dest, uint64_t count)
{
    return readPipe(id, dest, count);
}

uint64_t ksys_writePipe(uint64_t id, char *src, uint64_t count)
{
    return writePipe(id, src, count);
}
uint64_t ksys_mymalloc(uint64_t size)
{
    return (uint64_t)mymalloc(size);
}

uint64_t ksys_myfree(uint64_t ptr)
{
    myfree((void *)ptr);
    return 0;
}

uint64_t ksys_semOpen(char *name, int initValue)
{
    return semOpen(name, initValue);
}

uint64_t ksys_semClose(char *name)
{
    return semClose(name);
}

uint64_t ksys_semWait(int semIndex)
{
    return semWait(semIndex);
}

uint64_t ksys_semPost(int semIndex)
{
    return semPost(semIndex);
}

uint64_t ksys_findSem(char *name)
{
    return findSem(name);
}

uint64_t ksys_yield()
{
    yield();
    return 0;
}

uint64_t ksys_memset(uint64_t address, uint64_t value, uint64_t size)
{
    memset((void *)address, (int64_t)value, size);
    return 0;
}
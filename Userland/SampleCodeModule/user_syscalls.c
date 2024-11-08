#include <user_syscalls.h>
#include <interrupts.h>
#include <stdint.h>

#define INPUT_SIZE 100

int usys_read(int fd, char *buf, int size)
{
    return _read((uint64_t)fd, (uint64_t)buf, (uint64_t)size);
}

int usys_write(int fd, char *buf, int size)
{
    return _write((uint64_t)fd, (uint64_t)buf, (uint64_t)size);
}

int usys_write_color(uint32_t fontColor, char *buf, int size)
{
    return _write_color((uint64_t)fontColor, (uint64_t)buf, (uint64_t)size);
}

void usys_change_draw_size(int size)
{
    _change_draw_size((uint64_t)size);
}

void usys_print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char *buf)
{
    _print_array_of_draw_size((uint64_t)fontColor, (uint64_t)backgroundColor, (uint64_t)x, (uint64_t)y, (uint64_t)buf);
}

void usys_print_square(uint32_t fontColor, int x, int y, int size)
{
    _print_square((uint32_t)fontColor, (uint64_t)x, (uint64_t)y, (uint64_t)size);
}

void usys_print_rect(uint32_t color, int x, int y, int size_x, int size_y)
{
    _print_rect((uint32_t)color, (uint64_t)x, (uint64_t)y, (uint64_t)size_x, (uint64_t)size_y);
}

int usys_getRegisters(uint64_t *registers)
{
    return _getRegisters(registers);
}

void usys_wait(int ms)
{
    _wait((uint64_t)ms);
}

void usys_clear_screen()
{
    _clear_screen();
}

void usys_change_font_size(int new_font_size)
{
    _change_font_size(new_font_size);
}

int usys_get_font_size()
{
    return _get_font_size();
}

void usys_beep(int frec, int time_ms)
{
    _beep((uint32_t)frec, (uint64_t)time_ms);
}

int usys_get_time()
{
    return _get_time();
}

void flush_buffer()
{
    _flush_buffer();
}

int usys_createProcess(char *program, int argc, char **argv)
{
    return _createProcess(program, argc, argv);
}

int usys_blockProcess(int pid)
{
    return _blockProcess(pid);
}

int usys_unblockProcess(int pid)
{
    return _unblockProcess(pid);
}

int usys_getCurrentpid()
{
    return _getCurrentpid();
}

int usys_getCurrentPpid()
{
    return _getCurrentPpid();
}

int usys_killProcess(int pid)
{
    return _killProcess(pid);
}

int usys_leaveCPU()
{
    return _leaveCPU();
}

int usys_waitPid(int pid)
{
    return _waitPid(pid);
}

int usys_myExit()
{
    return _myExit();
}

int usys_my_nice(int pid, int new_prio)
{
    return _my_nice(pid, new_prio);
}

int usys_increase_priority(int pid)
{
    return _increase_priority(pid);
}

int usys_decrease_priority(int pid)
{
    return _decrease_priority(pid);
}

void usys_print_processes()
{
    _print_processes();
}

void usys_print_memory()
{
    _print_memory();
}

void *usys_mymalloc(size_t size)
{
    return (void *)_mymalloc(size);
}

void usys_myfree(void *ptr)
{
    _myfree(ptr);
}

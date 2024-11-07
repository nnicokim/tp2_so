#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

uint64_t _read(uint64_t fd, uint64_t buf, uint64_t size);

uint64_t _write(uint64_t fd, uint64_t buf, uint64_t size);

uint64_t _change_draw_size(uint64_t size);

uint64_t _print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf);

uint64_t _write_color(uint64_t fontCOlor, uint64_t buf, uint64_t size);

uint64_t _getRegisters(uint64_t *registers);

uint64_t _wait(uint64_t ms);

uint64_t _clear_screen();

uint64_t _change_font_size(uint64_t new_font_size);

uint64_t _get_font_size();

uint64_t _beep(uint64_t frec, uint64_t time);

uint64_t _print_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);

uint64_t _print_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);

uint64_t _get_time();

uint64_t _draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);

uint64_t _flush_buffer();

uint64_t _createProcess(void (*program)(int, char **), int argc, char **argv);

uint64_t _blockProcess(int pid);

uint64_t _unblockProcess(int pid);

uint64_t _getCurrentpid();

uint64_t _getCurrentPpid();

uint64_t _killProcess(int pid);

uint64_t _leaveCPU();

uint64_t _waitPid(int pid);

uint64_t _test_mm(uint64_t argc, char *argv[]);

uint64_t _test_processes(uint64_t argc, char *argv[]);

uint64_t _test_prio();

uint64_t _myExit();

uint64_t _my_nice(uint64_t pid, uint64_t newPrio);

uint64_t _increase_priority(int pid);

uint64_t _decrease_priority(int pid);

uint64_t _print_processes();

uint64_t _print_memory();
#endif

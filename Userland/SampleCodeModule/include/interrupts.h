#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

extern uint64_t _read(uint64_t fd, uint64_t buf, uint64_t size);

extern uint64_t _write(uint64_t fd, uint64_t buf, uint64_t size);

extern uint64_t _change_draw_size(uint64_t size);

extern uint64_t _print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf);

extern uint64_t _write_color(uint64_t fontCOlor, uint64_t buf, uint64_t size);

extern uint64_t _getRegisters(uint64_t *registers);

extern uint64_t _wait(uint64_t ms);

extern uint64_t _clear_screen();

extern uint64_t _change_font_size(uint64_t new_font_size);

extern uint64_t _get_font_size();

extern uint64_t _beep(uint64_t frec, uint64_t time);

extern uint64_t _print_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);

extern uint64_t _print_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);

extern uint64_t _get_time();

extern uint64_t _draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);

extern uint64_t _flush_buffer();

extern uint64_t _createOneProcess();

extern uint64_t _createProcess(void *process, int argc, char **argv);

extern uint64_t _blockProcess(int pid);

extern uint64_t _unblockProcess(int pid);

extern uint64_t _getCurrentpid();

extern uint64_t _getCurrentPpid();

extern uint64_t _killProcess(int pid);

extern uint64_t _leaveCPU();

extern uint64_t _waitPid(int pid);

extern uint64_t _test_mm(uint64_t argc, char *argv[]);

extern uint64_t _test_processes(uint64_t argc, char *argv[]);

extern uint64_t _test_prio();

extern uint64_t _myExit();

extern uint64_t _my_nice(uint64_t pid, uint64_t newPrio);

extern uint64_t _increase_priority(int pid);

extern uint64_t _decrease_priority(int pid);

extern uint64_t _print_processes();

extern uint64_t _print_memory();

extern uint64_t _loop_print();

extern uint64_t _test_sync(uint64_t argc, char *argv[]);
#endif

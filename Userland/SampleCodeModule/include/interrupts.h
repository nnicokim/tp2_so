#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

extern uint64_t usys_read(uint64_t fd, char* buff, uint64_t size);

extern uint64_t usys_write(uint64_t fd, char* buff, uint64_t size);

extern uint64_t usys_change_draw_size(uint64_t size);

extern uint64_t usys_print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char* buff);

extern uint64_t usys_write_color(uint64_t fontCOlor, char* buff, uint64_t size);

extern uint64_t usys_getRegisters(uint64_t *registers);

extern uint64_t usys_wait(uint64_t ms);

extern uint64_t usys_clear_screen();

extern uint64_t usys_change_font_size(uint64_t new_font_size);

extern uint64_t usys_get_font_size();

extern uint64_t usys_beep(uint64_t frec, uint64_t time);

extern uint64_t usys_print_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);

extern uint64_t usys_print_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);

extern uint64_t usys_get_time();

extern uint64_t usys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);

extern uint64_t usys_flush_buffer();

extern uint64_t usys_createOneProcess();

extern uint64_t usys_createProcess(char *pr_name, void *process, int argc, char **argv, int *fds);

extern uint64_t usys_blockProcess(int pid);

extern uint64_t usys_unblockProcess(int pid);

extern uint64_t usys_getCurrentpid();

extern uint64_t usys_getCurrentPpid();

extern uint64_t usys_killProcess(int pid);

extern uint64_t usys_leaveCPU();

extern uint64_t usys_waitPid(int pid);

// extern uint64_t usys_test_mm(uint64_t argc, char *argv[]);

// extern uint64_t usys_test_processes(uint64_t argc, char *argv[]);

// extern uint64_t usys_test_prio();

extern uint64_t usys_myExit();

extern uint64_t usys_my_nice(uint64_t pid, uint64_t newPrio);

extern uint64_t usys_increase_priority(int pid);

extern uint64_t usys_decrease_priority(int pid);

extern uint64_t usys_print_processes();

extern uint64_t usys_print_memory();

extern uint64_t usys_loop_print();

// extern uint64_t usys_test_sync(uint64_t argc, char *argv[]);

extern uint64_t usys_malloc(uint64_t size);

extern uint64_t usys_free(uint64_t ptr);

extern uint64_t usys_rick();

extern uint64_t usys_mymalloc(uint64_t size);

extern uint64_t usys_myfree(void* ptr);
#endif

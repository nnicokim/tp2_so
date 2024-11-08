#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H
#include <stdint.h>
#include <interrupts.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LASTIN 3

int usys_read(int fd, char *buf, int size);

int usys_write(int fd, char *buf, int size);

int usys_write_color(uint32_t fontColor, char *buf, int size);

void usys_change_draw_size(int size);

void usys_print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char *buf);

void usys_print_square(uint32_t fontColor, int x, int y, int size);

void usys_print_rect(uint32_t color, int x, int y, int size_x, int size_y);

int usys_getRegisters(uint64_t *registers);

void usys_wait(int ms);

void usys_clear_screen();

void usys_change_font_size(int new_font_size);

int usys_get_font_size();

void usys_beep(int frec, int time_ms);

int usys_get_time();

void flush_buffer();

int usys_createProcess(void (*program)(int, char **), int argc, char **argv);

int usys_blockProcess(int pid);

int usys_unblockProcess(int pid);

int usys_getCurrentpid();

int usys_getCurrentPpid();

int usys_killProcess(int pid);

int usys_leaveCPU();

int usys_waitPid(int pid);

int usys_myExit();

int usys_my_nice(int pid, int new_prio);

int usys_increase_priority(int pid);

int usys_decrease_priority(int pid);

void usys_print_processes();

void usys_print_memory();

#endif
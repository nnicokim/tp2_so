#ifndef USER_LIB_H
#define USER_LIB_H

#include <stdint.h>
#include <ucolors.h>

#define INPUT_SIZE 100

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LASTIN 3

void print(char *arr);

int strlen_u(char *arr);

void strcpy_u(char *destination, const char *source);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

void _invalid_opcode_exception(void);

int parseCommandArg(char *str);

int strcmp_u(const char *s1, const char *s2);

char getChar();

char getLastChar();

void putChar(char c);

char getLastChar();

void printColor(int fontColor, char *string);

void printError(char *string);

void intToString(int num, char *buf, int dim);

void intToStr(int num, char *str);

int stringToInt(const char *str);

void printScore(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, int num);

void udrawArray(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char *arr);

void udrawFrame(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y, uint64_t thickness);

int puts(const char *str);

int printf(const char *format, ...);
#endif
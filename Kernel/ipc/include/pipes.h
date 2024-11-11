#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <string.h>
#include <lib.h>

#include "../ipc/include/semaphore.h"
#include "../scheduler/include/scheduler.h" 

#define MAX_PIPES 100
#define EOF -1
#define READ_FD  0
#define WRITE_FD 1

typedef struct pipesType {
	char * name;
	unsigned int readIndex;
	unsigned int writeIndex;
	int unreadData;

	int readSemId;
	int writeSemId;
	
	char * pipeBuffer;
	uint8_t eof;
} pipesType;

typedef enum {
    READ_EVENT,
    WRITE_EVENT
} pipeEvent;

int createNamedPipe(char * name);
int createAnonPipe();
void destroyPipe(int id);

void putEOF(int id);

int getPipeId(char * name);
int readPipe(int id, char * dest, unsigned int count);
int writePipe(int id, const char * src, unsigned int count);

void EOFToCurrentProcess();

#endif
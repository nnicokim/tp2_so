// pipes
#include "./include/pipes.h"

static pipesType *pipes[MAX_PIPES] = {0};
static int nextPipeId = 1;

int createNamedPipe(char *name)
{
    pipesType *pipe = mymalloc(sizeof(pipesType));
    if (pipe == NULL)
    {
        return -1;
    }

    pipe->name = mymalloc(strlen_k(name) + 1);
    if (pipe->name == NULL)
    {
        myfree(pipe);
        return -1;
    }
    strcpy_k(pipe->name, name);

    /**
     * Initialize the pipe
     */
    pipe->readIndex = 0;
    pipe->writeIndex = 0;
    pipe->unreadData = 0;

    /**
     * Create semaphores for the pipe
     */
    pipe->readSemId = semCreate(name, 0);
    pipe->writeSemId = semCreate(name, 1); // Ver si conviene inicializarlo en PAGE en vez de 1

    pipe->pipeBuffer = mymalloc(PAGE);
    if (pipe->pipeBuffer == NULL)
    {
        myfree(pipe->name);
        myfree(pipe);
        return -1;
    }
    pipe->eof = 0;

    pipes[nextPipeId++] = pipe;
    return nextPipeId - 1;
}

int createAnonPipe()
{
    return createNamedPipe("");
}

void destroyPipe(int id)
{
    const pipesType *pipe = pipes[id];
    if (pipe == NULL)
        return;

    myfree(pipe->pipeBuffer);
    myfree(pipe);
    pipes[id] = NULL;
    return;
}

void putEOF(int id)
{
    pipes[id]->eof = 1;
    return;
}

int getPipeId(char *name)
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        if (pipes[i] != NULL && strcmp_k(pipes[i]->name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int pollPipe(int id, pipeEvent event)
{
    const pipesType *pipe = pipes[id];
    if (pipe == NULL)
        return -1;

    switch (event)
    {
    case READ_EVENT:
        return semWait(pipe->readSemId);
    case WRITE_EVENT:
        return semWait(pipe->writeSemId);
    default:
        return -1;
    }
}

int readPipe(int id, char *dest, unsigned int count)
{
    pipesType *pipe = pipes[id];
    if (pipe == NULL)
    {
        return -1;
    }

    if (pipe->eof || pipe->unreadData <= 0)
    {
        dest[0] = EOF;
        return 0;
    }

    uint32_t toRead = count > pipe->unreadData ? pipe->unreadData : count;
    memcpy(dest, pipe->pipeBuffer + pipe->readIndex, toRead);

    pipe->readIndex += toRead;
    pipe->unreadData -= toRead;

    semPost(pipe->readSemId);
    return toRead;
}

int writePipe(int id, const char *src, unsigned int count)
{
    pipesType *pipe = pipes[id];

    if (pipe == NULL || pipe->eof)
        return -1;

    unsigned int toWrite = count > PAGE - pipe->writeIndex ? PAGE - pipe->writeIndex : count;
    memcpy(pipe->pipeBuffer + pipe->writeIndex, src, toWrite);

    pipe->writeIndex += toWrite;
    pipe->unreadData += toWrite;

    semPost(pipe->writeSemId);
    return toWrite;
}

void EOFToCurrentProcess()
{
    int pid = getCurrentProcess();
    PCB *pcb = PCB_array[pid];
    if (pcb->FD[READ_FD] != -1)
    {
        putEOF(pcb->FD[READ_FD]);
    }
    return;
}
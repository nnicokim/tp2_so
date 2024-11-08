// #include "./include/test_util.h"

#include "../include/tests/test_mm.h"
#include <stdint.h>

#define MAX_BLOCKS 128

// #define BLOCK_SIZE 10000
// #define BLOCK_COUNT 10000

// void *start;
// int size, currentBlock;
// void *free_ptrs[BLOCK_COUNT];

// void *my_mm_init(void *ptrs, int s)
// {
//   start = ptrs;
//   size = s;
//   for (int i = 0; i < BLOCK_COUNT; i++)
//   {
//     free_ptrs[i] = start + i * BLOCK_SIZE;
//   }
//   currentBlock = 0;
//   return start;
// }

// void *mymalloc(size_t size)
// {
//   if (currentBlock < BLOCK_COUNT && size <= BLOCK_SIZE)
//   {
//     return free_ptrs[currentBlock++];
//   }
//   else
//     return NULL;
// }

// void myfree(void *ptr)
// {
//   if (ptr == NULL || ptr < start)
//     return;
//   free_ptrs[--currentBlock] = ptr;
// }

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[])
{

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1)
    return -1;

  if ((max_memory = satoi(argv[0])) <= 0)
    return -1;

  while (1)
  {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory)
    {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = usys_mymalloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address)
      {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
        {
          printColor(RED, "test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        usys_myfree(mm_rqs[i].address);
  }
}
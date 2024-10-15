#include "syscall.h"
#include "./include/test_util.h"
// #include "./include/test_mm_buddy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <buddyAllocator.h>

#define MAX_BLOCKS 10

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm_buddy(uint64_t argc, char *argv[])
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
    // while (rq < MAX_BLOCK && total < max_memory) {
    while (total < max_memory)
    {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      // mm_rqs[rq].address = mymalloc(mm_rqs[rq].size);
      mm_rqs[rq].address = buddy_allocate(mm_rqs[rq].size);

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
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        // myfree(mm_rqs[i].address);
        buddy_free(mm_rqs[i].address);
  }
}
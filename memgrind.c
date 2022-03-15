#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mymalloc.h"

// test case A: use malloc to allocate 1 byte and immediately free it using free, and repeating this 120 times. 
static int testCase_A()
{
  int i;
  for (i = 0; i < 120; i++)
  {
    void *pointer = malloc(1);
    if (pointer == NULL)
    {
      return -1;
    }
    free(pointer);
  }
  return 0;
}

// Test case B: use malloc() to allocate 1 byte and store the pointer in an array repeating this 120 times. 
static int testCase_B(void){
    // Initialize all member from the array not NULL with '= { NULL }'
    // (only the first one is initialized explicitly).
    // This is not necessary if you are 100% sure that it will
    // be initialized later, but still recommended

    char * ptr[120] = { NULL };

    for (int i = 0; i < 120; i++) {
        ptr[i] = malloc(1);
        // After storing the result of malloc in ptr[i], you should
        // check the same location 'ptr[i]' to see if it's NULL
        if (ptr[i] == NULL)
        {

            // If an allocation fails mid-loop (i.e. when i equals 60)
            // and we return immediately, the previous chunks (59 in this case)
            // are not deallocated.
            // Therefore, you should deallocate those chunks before
            // leaving the function
            for (int j = 0; j < i; j++) {
                free(ptr[i]);
            }
            return -1;
        }
    }

    for (int i = 0; i < 120; i++) {
        free(ptr[i]);
    }
    return 0;
}
// Test case C: choose randomly between 1 bye malloc and free, repeat this 240 times. 
static int testCase_C()
{
  int i, dealo = 0, alo = 0;
  char *pointer[120];
  int malo = 120, fre = 0; // malloc-ed and free-ed blocks count

  for (i = 0; i < 240; i++)
  {

    // decide to malloc or free
    const int v = rand() % (malo + fre);

    if (v < fre)
    {
      free(pointer[dealo]);
      dealo++;
      fre--;
    }
    else
    {
      fre++;
      malo--;
      pointer[alo++] = malloc(1);
      if (pointer == NULL)
      {
        return -1;
      }
    }
  }
  return 0;
}

int testCase_D()
{
  void *pointer1 = malloc(4096);
  if (pointer1 == NULL)
  {
    return -1;
  }

  void *pointer2 = malloc(1);
  if (pointer2 != NULL)
  {
    return -1;
  }

  free(pointer1);

  return 0;
}

int testCase_E()
{
  /// try to allocate 0 size block using memory
  void *pointer = malloc(0);
  if (pointer != NULL)
  {
    return -1;
  }

  // try to free null pointer from memory
  free(NULL);

  // Try to free pointer, which is set inside the allocated block
  pointer = malloc(20);
  if (pointer == NULL)
  {
    return -1;
  }
  free(pointer + 10);
  free(pointer);

  return 0;
}

int main(void)
{
  const int TOTAL_REPS = 50;
  struct timeval begain, end, test_time, temp, total[5];

  srand(getpid());

  memoryIntialize();

  int i;
  for (i = 0; i < TOTAL_REPS; i++)
  {

    gettimeofday(&begain, 0);
    if (testCase_A() < 0)
    {
      fprintf(stderr, "Test case A FAILED\n");
      return 1;
    }
    gettimeofday(&end, 0);
    timersub(&end, &begain, &test_time);
    timeradd(&total[0], &test_time, &temp);
    total[0] = temp;

    gettimeofday(&begain, 0);
    if (testCase_B() < 0)
    {
      fprintf(stderr, "Test case B FAILED\n");
      return 1;
    }
    gettimeofday(&end, 0);
    timersub(&end, &begain, &test_time);
    timeradd(&total[1], &test_time, &temp);
    total[1] = temp;

    gettimeofday(&begain, 0);
    if (testCase_C() < 0)
    {
      fprintf(stderr, "Test case C FAILED\n");
      return 1;
    }
    gettimeofday(&end, 0);
    timersub(&end, &begain, &test_time);
    timeradd(&total[2], &test_time, &temp);
    total[2] = temp;

    gettimeofday(&begain, 0);
    if (testCase_D() < 0)
    {
      fprintf(stderr, "Test case D FAILED\n");
      return 1;
    }
    gettimeofday(&end, 0);
    timersub(&end, &begain, &test_time);
    timeradd(&total[3], &test_time, &temp);
    total[3] = temp;

    gettimeofday(&begain, 0);
    if (testCase_E() < 0)
    {
      fprintf(stderr, "Test case E FAILED\n");
      return 1;
    }
    gettimeofday(&end, 0);
    timersub(&end, &begain, &test_time);
    timeradd(&total[4], &test_time, &temp);
    total[4] = temp;
  }

  // calculateing the mean time
  for (i = 0; i < 5; i++)
  {
    total[i].tv_sec /= TOTAL_REPS;
    total[i].tv_usec /= TOTAL_REPS;
  }

  // printing the finale times
  printf("Overall time\n");
  for (i = 0; i < 5; i++)
  {
    printf("Overall time to execute test %c %li.%li\n", 'A' + i, (long)total[i].tv_sec, (long)total[i].tv_usec);
  }

  return 0;
}

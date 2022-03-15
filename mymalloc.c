#include <stdio.h>
#include <strings.h>

#include "mymalloc.h"

// length of the memory
#define SIZE 4096

// metadata for each memory block
struct metaData
{
  unsigned char isUsed;
  unsigned short blocksize;
};

// intitalizing array potraying as memory
static char memory[sizeof(struct metaData) + SIZE + sizeof(struct metaData)];

// finding the first block to fit in given size
// returns the pointer to metadata block or it will give null, if faliture to find block
static char *firstBlock_fix(const size_t size)
{

  char *iter = memory;
  struct metaData *meta = (struct metaData *)iter;

  while (meta->blocksize != 0)
  {                             // iter till the last block is not reached
    if ((meta->isUsed == 0) && // if block is free
        (meta->blocksize >= size))
    {              // and it can fit the size required
      return iter; // then return the meradata
    }

    // this helps to move to the next block
    iter += sizeof(struct metaData) + meta->blocksize;
    meta = (struct metaData *)iter;
  }
  return NULL;   // returns null upon faliure to find the block of required size
}
// it finds the block by the data pointer
// it returns pointer to metadata or null if faliure to find block
static char *find_block(void *pointer)
{
  char *iter = memory;
  struct metaData *meta = (struct metaData *)iter;

  while (meta->blocksize != 0)
  { // iter till the last block 

    // if the poiner points at the data of the block 
    if ((iter + sizeof(struct metaData)) == pointer)
    {
      return iter; // return
    }

    // helps to move to the next block
    iter += sizeof(struct metaData) + meta->blocksize;
    meta = (struct metaData *)iter;
  }
  return NULL; //returns null upon faliure to find the block with the given poiner.
}

// inserts block into the memory 
void insert_block()
{
  char *iter = memory;
  struct metaData *meta = (struct metaData *)iter;

  int i = 0;
  while (meta->blocksize != 0)
  { // iter till the last block

    printf("%d| isUsed=%d, size=%d\n", i++, meta->isUsed, meta->blocksize);

    // helps to move to the next block
    iter += sizeof(struct metaData) + meta->blocksize;
    meta = (struct metaData *)iter;
  }
}

// helps to merge the contiguos free blocks of memory
static void mergeBlock()
{
  char *iter = memory;
  struct metaData *meta = (struct metaData *)iter;
  struct metaData *prev = NULL;

  while (meta->blocksize != 0)
  { // iter till the last block

    // if previous and current blocks are free
    if ((prev != NULL) && (prev->isUsed == 0) && (meta->isUsed == 0))
    {
      // merge them together
      prev->blocksize += meta->blocksize + sizeof(struct metaData);
      if (prev->blocksize > SIZE)
      {
        fprintf(stderr, "Error: block merge invalid\n");
      }
    }
    else
    {
      prev = meta;
    }

    // helps to move to the next block
    iter += sizeof(struct metaData) + meta->blocksize;
    meta = (struct metaData *)iter;
  }
}

// Initialize our memory allocator, by clearing memory and settings first and last blocks
void memoryIntialize()
{

  // this method clears the memory
  bzero(memory, sizeof(memory));

  // setting the first block as to occupie the whole memory
  struct metaData *meta = (struct metaData *)memory;
  meta->isUsed = 0;
  meta->blocksize = SIZE;

  // last block is of size -1
  struct metaData *endOfMeta = (struct metaData *)&memory[sizeof(struct metaData) + SIZE];
  endOfMeta->isUsed = 1;
  endOfMeta->blocksize = 0;
}

// Allocate a request for memory of specified size
//it returns the pointer to the adress or null upon the faliure
void *mymalloc(size_t size, const char *file, const int lineno)
{

  if (size <= 0)
  {
    fprintf(stderr, "%s:%d:malloc: Is invalid size\n", file, lineno);
    return NULL;
  }

  if (size > SIZE)
  {
    fprintf(stderr, "%s:%d:malloc: Is invalid size\n", file, lineno);
    return NULL;
  }

  // it finds the block that can fit in the request.
  char *iter = firstBlock_fix(size);
  if (iter == NULL)
  { // if the block of memory is not found the give this error
    fprintf(stderr, "%s:%d:malloc: Is out of memory\n", file, lineno);
    return NULL;
  }

  struct metaData *meta = (struct metaData *)iter;

  const int space_left = meta->blocksize - size - sizeof(struct metaData);
  if (space_left > 16)
  { // if space is more than 16 bytes
    // then split the block and set it to the next size
    struct metaData *next = (struct metaData *)&iter[sizeof(struct metaData) + size];
    next->isUsed = 0;
    next->blocksize = space_left;
  }

  meta->isUsed = 1;
  meta->blocksize = size;

  // it reterns the address of metadata to be useful later
  return (void *)&iter[sizeof(struct metaData)];
}

// Release a memory pointer
void myfree(void *vpointer, const char *file, const int lineNum)
{
  char *pointer = (char *)vpointer;

  if (pointer == NULL)
  {
    fprintf(stderr, "%s:%d:free: Can't free NULL\n", file, lineNum);
    return;
  }

  // check if pointer is inside memory[]
  if ((pointer < &memory[sizeof(struct metaData)]) || (pointer >= &memory[SIZE]))
  {
    fprintf(stderr, "%s:%d:free: Pointer out of memory[]\n", file, lineNum);
    return;
  }

  // check if pointer is at the front of the block
  char *iter = find_block(pointer);
  if (iter == NULL)
  {
    fprintf(stderr, "%s:%d:free: Pointer not found\n", file, lineNum);
    return;
  }

  struct metaData *meta = (struct metaData *)iter;
  if (meta->isUsed == 0)
  {
    fprintf(stderr, "%s:%d:free: Pointer not malloc-ed\n", file, lineNum);
    return;
  }

  meta->isUsed = 0; // mark block as free

  mergeBlock();
}

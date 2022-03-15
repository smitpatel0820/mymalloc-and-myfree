CS214 Systems Programming Project 1: My little malloc() by Smit Patel and Aditi Patel

⌘ Purpose 
=> The purpose of this assignment is to implementing a toy version of the memory management system provided by the C standard library, along with drop-in replacements for the malloc () and free() functions using marcos in our mymalloc.h file,

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
 
 The extra parameters are used to indicate the file name and line number where an error occurred.

=> Malloc(size_t size) will return a pointer to a block of memory whose size is at least the requested size. These blocks of memory will come from a 5000-byte char array defined in mymalloc.c,

static char memory[sizeof(struct metaData) + SIZE + sizeof(struct metaData)];
In this case SIZE is 4096

⌘ To run the program, simply call the make command-
make

⌘ This will produce not only an object file of mymalloc but also a memgrind executable. To excute the executable, simply call-
./memgrind

//Note that memgrind.c just tests the malloc and free macros defined in mymalloc.

⌘ #include "mymalloc.h"
Currently, our virtual memory is 4096 bytes but this can be modified by adjusting this line in mymalloc.c.
#define MEM_SIZE 4096

⌘ Memory organization
We have organized the memory into blocks. Each block is has two parts - header and data.

=> The header specifies if block is used and its size. It adds overhead of 4 bytes, but its needed to build a list of free/used blocks. 
The end of memory is marked with a used block, of size 0. This way we can iterate over all of the blocks, until we reach the last block.

=> The data part is returned to user, so he can use it.

⌘ User can call two memorey functions - malloc and free.

=> malloc searches for a free block, and if found, its marked as used, and returned to user.
If the block is too large, we split it in two. First block goes to user, and the second is marked, as free,with reduced size.

⌘ When user calls free(), we do some validation checks, and mark the block as free.

⌘ A defragmentation function is called to merge the adjacent free blocks into one large block.


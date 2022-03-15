#include <sys/types.h>

void memoryIntialize();

void *mymalloc(size_t size, const char *file, const int lineNum);
void myfree(void *vpointer, const char *file, const int lineNum);

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

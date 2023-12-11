#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include "buddySystemAllocator.h"

void push(BlockInfo**, BlockInfo*);
BlockInfo* pop(BlockInfo**);

#endif // __LINKED_LIST__
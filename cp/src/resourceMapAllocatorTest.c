

#include "../include/resourceMapAllocator.h"
#include "../include/testFunctionsRMAllocator.h"


int main() {
    Allocator* allocator = createMemoryAllocator(40000000);

    testWithLightDataSequential(allocator);
    // testLightDataRandom(allocator);
    // allocBlock(allocator, 40000000 - 100);

    // destroyMemoryAllocator(allocator);
}
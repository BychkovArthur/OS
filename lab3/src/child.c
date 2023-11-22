#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid argumetns\n");
        // -- передать в parent
        return 1;
    }
    FILE* outputFile = fopen(argv[1], "w");
    if (outputFile == NULL) {
        printf("Can't open file (child)\n");
        // -- передать в parent
        return 1;
    }
}
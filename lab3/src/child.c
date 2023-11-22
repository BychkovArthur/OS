#include <stdio.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Invalid argumetns\n");
        return 1;
    }
    
    FILE* outputFile = fopen(argv[1], "w");

    fprintf(outputFile, "124234" );
}
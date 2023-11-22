#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE* outputFile = fopen(argv[1], "w");

    fprintf(outputFile, "124234" );
}
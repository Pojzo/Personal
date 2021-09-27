#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME_LENGTH 9
#define BUFFER_LENGTH 50

bool FilesEqual(FILE*, FILE*);

int main(int argc, char** argv){
    const char* filename1 = argv[1];
    const char* filename2 = argv[2];
    printf("Comparing %s and %s\n", filename1, filename2);

    char buffer[BUFFER_LENGTH];

    FILE *fp1, *fp2;
    fp1 = fopen(filename1, "r");
    fp2 = fopen(filename2, "r");
    if (FilesEqual(fp1, fp2)) {
        printf("Files are equal\n");
    }
    else {
        printf("Files are not equal\n");
    }
}

bool FilesEqual(FILE* fp1, FILE* fp2) {
    char buffer1[BUFFER_LENGTH];
    char buffer2[BUFFER_LENGTH];

    while(fgets(buffer1, BUFFER_LENGTH, fp1)) {
        if (!fgets(buffer2, BUFFER_LENGTH, fp2)) {
            return false;
        }
        if (strncmp(buffer1, buffer2, BUFFER_LENGTH)) {
            printf("Nerovnaju sa tu\n");
            return false;
        }
    }
    return true;
}


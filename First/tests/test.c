#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME_LENGTH 9
#define BUFFER_LENGTH 50
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define BLUE  "\034[34m"
#define YELLOW "\033[33m"

void CompareFiles(FILE*, FILE*);
void RemoveNewLine(char*, unsigned);

int main(int argc, char** argv){
    printf(RESET);
    const char* filename1 = argv[1];
    const char* filename2 = argv[2];
    printf(YELLOW "*-------RUNNING TEST %s-------*\n" RESET, argv[3]);

    char buffer[BUFFER_LENGTH];

    FILE *fp1, *fp2;
    fp1 = fopen(filename1, "r");
    fp2 = fopen(filename2, "r");
    CompareFiles(fp1, fp2);
    printf("\n");
}

void RemoveNewLine(char* string, unsigned string_len) {
    string[string_len -1] = '\0';
}

void CompareFiles(FILE* fp1, FILE* fp2) {
    char buffer1[BUFFER_LENGTH];
    char buffer2[BUFFER_LENGTH];

    while(fgets(buffer1, BUFFER_LENGTH, fp1)) {
        RemoveNewLine(buffer1, strlen(buffer1));
        if (!fgets(buffer2, BUFFER_LENGTH, fp2)) {
            printf("%s | ", buffer1);
            printf(RED "EMPTY LINE\n" RESET);
            continue;
        }
        if (strncmp(buffer1, buffer2, strlen(buffer1))) {
            printf("%s | ", buffer1);
            printf(RED "%s" RESET, buffer2);
            continue;
        }
        printf("%s | ", buffer1);
        printf(GREEN "%s" RESET, buffer2);
    }
}


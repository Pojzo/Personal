#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 100

bool StringsEqual(const char* s1, const char* s2) {
    // ak sa dlzka jedneho stringu nerovna tomu druhemu, tak automaticky sa nemozu rovnat
    while (*(s1) && *(s2)) {
        if (*s1 != *s2) {
            return false;
        }
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

void StringCopy(char* string, char* dest, unsigned start, unsigned substring_len) {
    for (unsigned i = start; i < start + substring_len; i++) {
        dest[i - start] = string[i];
    }
}

bool ContainsDuplicateSubstring(char substrings [][MAX_LENGTH], unsigned num_substrings) {
    for(unsigned i = 0; i < num_substrings; i++) {
        for(unsigned j = 0; j < num_substrings; j++) {
            if (i == j) {
                continue;
            }
            if (StringsEqual(substrings[i], substrings[j])) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    char buffer[MAX_LENGTH];
    unsigned substring_len = 2;
    scanf("%s", &buffer);
    unsigned num_substrings = strlen(buffer) + 1 - substring_len;
    char substrings[num_substrings][MAX_LENGTH];
    char substring[substring_len];
    for (unsigned i = 0; i < strlen(buffer) - substring_len + 1; i++) {
        StringCopy(buffer, substrings[i], i, substring_len);
    }
    /*
    for (unsigned i = 0; i < num_substrings; i++) {
        printf("%s\n", substrings[i]);
    }
    */
    if (ContainsDuplicateSubstring(substrings, num_substrings)) {
        printf("Contains duplicate substring\n");
    }
    else {
        printf("Does not contain duplicate substring\n");
    }

    return 0;
}

#include <string.h>
#include <stdlib.h>

int strsplit(char *str, const char *delim, char *out[], int maxparts)
{
    int count = 0;
    char *token = strtok(str, delim);
    while (token != NULL && count < maxparts)
    {
        out[count++] = token;
        token = strtok(NULL, delim);
    }
    return count;
}

int randbetween(int min, int max) {
    return rand() % (max - min + 1) + min;
}

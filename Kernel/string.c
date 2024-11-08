#include <string.h>
#include <lib.h>

int strlen(char *str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

char *strcpy(char *dest, char *src)
{
    return memcpy(dest, src, strlen(src));
}

int strcmp(char *str1, char *str2)
{
    int i = 0;
    for (int i = 0; str1[i] != '\0' && str2[i] != '\0'; i++)
        if (str1[i] != str2[i])
            return str1[i] - str2[i];

    return str1[i] - str2[i];
}
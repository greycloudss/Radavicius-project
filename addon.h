#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define strupr _strupr
#define strlwr _strlwr
#define MAX_USERS 1000
#pragma once

//printing a somewhat centered thing

void print(char string[]) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    printf("\n\n%*s\n", (csbi.dwSize.X + strlen(string)) / 2, string, csbi.dwSize.X - strlen(string) / 2, "");
}

void dataWrite(const char file_path[], char x[], int num) {
    //no need for a null check since the flag "+" creates a file database.bin
    //int num is just a flag if the passed down var is a password its0 flag is 3
    FILE* cache = fopen(file_path, "ab+");
    char* mod = (char*)malloc(sizeof(x));
    if (num == 3) {
        free(mod);
        char* mod = (char*)malloc(strlen(x) + num);
        for (int i = 0; i < num; ++i)
            strcat(mod, "\n");
    }

    fwrite(mod, 1, strlen(mod), cache);
    free(mod);
    fclose(cache);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

bool occuranceChecker(const char* file_path, const char* target_string) {
    FILE* file = fopen(file_path, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return true;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* binary_data = (char*)malloc(file_size);
    if (binary_data == NULL) {
        perror("Memory allocation error");
        fclose(file);
        free(binary_data);
        return true;
    }

    fread(binary_data, 1, file_size, file);
    fclose(file);

    char* found_position = strstr(binary_data, target_string);

    if (found_position != NULL) {
        size_t index = found_position - binary_data;
        free(binary_data);
        return true;
    }
    else {
        free(binary_data);
        return false;
    }

}
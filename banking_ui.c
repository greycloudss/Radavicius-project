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

typedef struct {
    bool status;
    char name[18];
    char password[18];
    int userID;
    double balance;
} User;



//printing a somewhat centered thing
void print(char string[]) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    printf("\n\n%*s\n", (csbi.dwSize.X + strlen(string)) / 2, string, csbi.dwSize.X - strlen(string) / 2, "");
}

void dataWrite(const char file_path[], char x[], int num) {
    //no need for a null check since the flag "+" creates a file database.bin
    //int num is just a flag if the passed down var is a password its flag is 3
    FILE* cache = fopen(file_path, "ab+");
    char* mod = (char*) malloc(strlen(x));
    if (num == 3) {
        free(mod);
        char* mod = (char*)malloc(strlen(x) + num);
        for (int i = 0; i < num; ++i)
            strcat(mod, "\n");
    }

    fwrite(mod, 1, sizeof(mod), cache);
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

bool occuranceChecker(const char* file_path, const char* target_string, User* name) {
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
        return true;
    }

    fread(binary_data, 1, file_size, file);
    fclose(file);

    char* found_position = strstr(binary_data, target_string);

    if (found_position != NULL) {
        strcpy(&name->name, target_string);
        size_t index = found_position - binary_data;
        return true;
    }
    else {
        return false;
    }

    free(binary_data);
}

bool loginUser(User* creds) {
    char cache_name[] = "database.bin";
    bool state[] = { false, false, false };
    const char prompts[][30] = { "Enter your username: ", "Enter your id: ", "Enter your password: " };
    int count = 0;
    char tmp[18];
    print("Login");
    while (count < 3) {
        print(prompts[count]);
        if (scanf("%18s", &tmp)) {
            if (occuranceChecker(cache_name, tmp, &creds->name)) {
                
                state[count++] = true;
            }
            else return false;
        }
    }

    return true;
}

int id_gen() {
    char tmp[7] = { 0 };
    User A;
    while (1) {
        int id = rand(time(NULL));
        tostring(tmp, id);
        if (occuranceChecker("database.bin", tmp, &A) == false)
            return id;
    }
}



bool registUser() {
    char cache_name[] = "database.bin";
    bool state[] = { false, false, false };
    const char prompts[][30] = { "Enter your username: ", "Enter your password: ", "Reenter your password: " };
    int count = 0;
    char tmp[5][18] = { 0 };
    User A;
    const int sizes[] = { 18, 18, 18 };
    print("Registration");
    while (count < 3) {
        print(prompts[count]);
        if (scanf("%18s", &tmp[count]) == 1) {
            if (occuranceChecker(cache_name, tmp[count], &A) == true) {
                print("Registration failed");
                Sleep(5000);
                return false;
            }
            else {
                if (count < 2)
                    state[count++] = true;
                else {
                    if (strcmp(tmp[1], tmp[2]) != 0) {
                        print("Registration failed");
                        Sleep(5000);
                        return false;
                    }
                    else {
                        print("Registration complete");
                        state[count++] = true;
                    }
                }
            }
        }
    }


    //please forgive me for the upcoming 6 lines
    strcpy(tmp[3], "Your username: ");
    strcpy(tmp[4], "Your password: ");
    strcat(tmp[3], tmp[0]);
    strcat(tmp[4], tmp[2]);
    strcpy(tmp[0], tmp[3]);
    strcpy(tmp[2], tmp[4]);

    tostring(tmp[1], id_gen());

    system("cls");
    print(tmp[0]);
    print(tmp[1]);
    print(tmp[2]);

    for (int i = 0; i < 0; ++i) {
        if (state[i] != true)
            return false;
        dataWrite(cache_name, tmp[i], i + 1);
    }
    Sleep(5000);
    return true;
}

User showBalance(const char file[], unsigned mode, char name[], User *creds) {
    //mode = 0 is for transfer money; 1 is for general use
    //if 

}


int userInterface(bool *status, int instance) {
    //move of arrow
    bool state = true;
    int selCode = 0;
    User creds;
    const char options[][16] = {"Register", "Login", "Balance", "Transfer"};
    int selBlocks[][2] = { {0, 1}, {2, 3} };
    char printName[100];

    do {
        //if presses any of the movement keys then do modullus 2 between selCode and 1 in order to figure out which option is selected
        //W and S are also used since im an abosolute unit at video games
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) {
            selCode = selCode ^ 1;
            state = true;
        }

        //switch statements unable to make since GetAsyncKeyState(VK_RETURN) & 0x8000 is not liked by switch, same as selBlocks[instance][selCode] == n
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (selBlocks[instance][selCode] == 0) {
                // register
                system("cls");
                registUser();
                selBlocks[instance][selCode] = 1;
            }
            if (selBlocks[instance][selCode] == 1) {
                // login
                system("cls");
                status = loginUser(&creds);
                if (status == true)
                    return 1;

            }
            if (selBlocks[instance][selCode] == 2) {
                // balance

            }
            if (selBlocks[instance][selCode] == 3) {
                // transfer

            }
            
        }

        if (state) {
            system("cls");
            if (status != true) {
                print("Login status: false");
            }
            else {
                strcpy(printName, "\n\nCurrently logged in as: ");
                print(strcat(printName, creds.name));
            }
            print(strupr(options[selBlocks[instance][selCode]]));
            print(strlwr(options[selBlocks[instance][selCode ^ 1]]));
            state = false;
        }
    } while (1);
}

void main() {
    int instance = 0;
    bool status = false;
    while (1) {
        instance += userInterface(&status, instance);
        if (instance == 0)
            status = false;
    }
}
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
    char name[50];
    char password[50];
    bool isLoggedIn;
    int userID;
    double balance;
} User;

void registerUser(User* users, int* userCount);
void loginUser(User* users, int userCount);
void displayBalance(const User* user);
void transferBalance(User* users, int userCount);
int generateUserID(const User* users, int userCount);
void readDatabase(User* users, int* userCount);
void writeDatabase(const User* users, int userCount);

// Read user data from database.bin file
void readDatabase(User* users, int* userCount) {
    FILE* file = fopen("database.bin", "rb");
    if (file == NULL) {
        *userCount = 0;
        return;
    }
    fread(userCount, sizeof(int), 1, file);
    fread(users, sizeof(User), *userCount, file);
    fclose(file);
}

// Write user data to database.bin file
void writeDatabase(const User* users, int userCount) {
    FILE* file = fopen("database.bin", "wb");
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fclose(file);
}

// Rest of the functions (registerUser, loginUser, generateUserID, etc.) with modifications to interact with the database

// Generate a unique user ID
int generateUserID(const User* users, int userCount) {
    int uniqueID;
    bool isUnique;
    do {
        isUnique = true;
        uniqueID = rand() % 100000; // Random ID between 0 and 99999
        for (int i = 0; i < userCount; i++) {
            if (users[i].userID == uniqueID) {
                isUnique = false;
                break;
            }
        }
    } while (!isUnique);
    return uniqueID;
}

// Function to register a new user
void registerUser(User* users, int* userCount) {
    if (*userCount >= MAX_USERS) {
        printf("\nRegistration failed: User limit reached.\n");
        return;
    }

    printf("\nEnter your name: ");
    scanf("%49s", users[*userCount].name);
    printf("Enter your password: ");
    scanf("%49s", users[*userCount].password);
    users[*userCount].userID = generateUserID(users, *userCount);
    users[*userCount].isLoggedIn = false;
    users[*userCount].balance = 0.0;
    (*userCount)++;
    writeDatabase(users, *userCount);
    printf("\nRegistration successful!\n");
}

// Function to login a user
void loginUser(User* users, int userCount) {
    char name[50], password[50];
    printf("\nEnter your name: ");
    scanf("%49s", name);
    printf("Enter your password: ");
    scanf("%49s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, password) == 0) {
            users[i].isLoggedIn = true;
            printf("\nLogin successful!\n");
            return;
        }
    }
    printf("\nLogin failed!\n");
}


//printing a somewhat centered thing
void print(char string[]) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    printf("\n\n%*s\n", (csbi.dwSize.X + strlen(string)) / 2, string, csbi.dwSize.X - strlen(string) / 2, "");
}

int userInterface(bool *status, int instance, User *creds, int *userCount) {
    //move of arrow
    bool state = true;
    int selCode = 0;

 
    char options[][16] = { "Register", "Login", "Balance", "Transfer" };
    int selBlocks[][2] = { {0, 1}, {2, 3} };

    do {
        //if presses any of the movement keys then do modullus 2 between selCode and 1 in order to figure out which option is selected
        //W and S are also used since im an abosolute unit at video games
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) {
            selCode = selCode ^ 1;
            state = true;
        }

        if (true == state) {
            system("cls");
            if (status != true) {
                print("Login status: false");
            }
            else {
                print(strcat("\n\nCurrently logged in as:", creds->name, "\n\n"));
            }
            print(strupr(options[selBlocks[instance][selCode]]));
            print(strlwr(options[selBlocks[instance][selCode ^ 1]]));
            state = false;
        }

        if (GetAsyncKeyState(VK_RETURN)) {
            if (selBlocks[instance][selCode] == 0)
                registerUser(&creds, userCount);
        }


    } while (1);

    return state;
}

void main() {
    int instance = 0;
    //login status
    bool status = false;
    User creds;
    int userCount;
    while (1) {
        //addition of state to instance in order to increase the value thus leading to a different assortment of options
        instance += userInterface(&status, instance, &creds, &userCount);
    }
}
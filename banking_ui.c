#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "addon.h"
#define strupr _strupr
#define strlwr _strlwr
#define MAX_USERS 1000
/*
* Autorius: Dovydas Levinas
* versija 2.0
* Kuria failus ir i juos raso, banko sys simulatorius-ish; veikia kai uzsinori
*/

typedef struct {
    bool status;
    char name[18];
    char password[18];
    int userID;
    double balance;
} User;


void clearBuffer() {
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

bool loginUser(char *string) {
    char cache_name[] = "database.bin";
    bool state[] = { false, false, false };
    const char prompts[][30] = { "Enter your username: ", "Enter your id: ", "Enter your password: " };
    int count = 0;
    char tmp[18];
    print("Login");
    while (count < 3) {
        print(prompts[count]);
        if (scanf("%18s", &tmp) && getchar() == '\n') {
            if (count == 0) {
                strcpy(string, tmp);
            }
            if (occuranceChecker(cache_name, tmp)) {
                state[count++] = true;
                
            }
            else return false;
        }
    }

    return true;
}

int id_gen() {
    char tmp[7] = { 0 };
    while (1) {
        int id = rand(time(NULL));
        tostring(tmp, id);
        
        if (occuranceChecker("database.bin", tmp) == false)
            return id;
    }
}


/*
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
 
    strcpy(tmp[0], tmp[3]);
    strcpy(tmp[2], tmp[4]);
    char id[7];
    tostring(id, id_gen());
    strcpy(tmp[1], "Your id is: ");
    strcat(tmp[1], id);

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

redundant as it has been deemed too little time to fix this
*/


bool registerUser() {
    FILE* cache;
    cache = fopen("database.bin", "ab+");
    const char prompts[][30] = { "Enter your full name: ", "Enter your password: ", "Reenter your password: " };
    bool flags[3] = { false, false, false };
    int count = 0;
    int size[3] = { 30, 20, 20 };
    char tmp[3][30] = { 0 };
    print("Registration\n");
    do {
        print(prompts[count]);
        if (scanf("%29s", tmp[count]) == 1) {
            if (strlen(tmp[count]) <= size[count]) {
                if (count < 2) {
                    flags[count++] = true;
                }
                else if (count == 2) {
                    if (strcmp(tmp[1], tmp[2]) == 0) {
                        flags[count++] = true;
                    }
                    else {
                        print("Passwords do not match. Please re-enter.\n");
                        count = 0;
                    }
                }
            }
            else {
                print("Invalid input\n");
                count = 0;
            }
        }
        else {
            print("Invalid input\n");
            count = 0;
            scanf("%*s");
        }
    } while (count < 3);


    for (int i = 0; i < 3; ++i) {
        if (!flags[i]) {
            print("Registration failed. Please try again.\n");
            fclose(cache);
            return false;
        }
    }
    time_t t1;
    srand((unsigned)time(&t1));
    char bonkers[7] = { 0 };
    for (int i = 0; i < 6; ++i) {
        char frac_id = (rand() % 10) + '0';
        bonkers[i] = frac_id;
    }
    printf("\nName: %s, User id: %s, Password: %s", tmp[0], bonkers, tmp[1]);


    fprintf(cache, "--------------\n%s\n%s\n%s\n--------------", tmp[0], bonkers, tmp[1]);
    fclose(cache);
    Sleep(5000);
    return true;
}

static double showBalance(unsigned mode, char *name) {
    char* fp_name = malloc(sizeof(name) + sizeof(".bin"));
    strcpy(fp_name, name);
    strcat(fp_name, ".bin");
    double money;

    //dataWrite(fp_name, "300", 0);
    if (fp_name == NULL) {
        print("Insufficient funds....");
        Sleep(5000);
        money = 0;
    }
    
    FILE* cache = fopen(fp_name, "rb");
    
    if (cache == NULL) {
        print("Insufficient funds");
        Sleep(5000);
        money = 0;
    }
    rewind(cache);
    
    if (fscanf(cache, "%f", &money) != 1) {
        fprintf(stderr, "Error fetching money count\n");
        money = 0;
    }

    if (mode != 0) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        print("your current balance is:");
        printf(" %*.2f\n", (csbi.dwSize.X) / 2, money, csbi.dwSize.X / 2, "");
        Sleep(5000);
    }

    
    free(fp_name);
    fclose(cache);
    return money;
}

static void transferMoney(char* name) {
    clearBuffer();
    system("cls");

    char* fp_name = malloc(sizeof(name) + sizeof(".bin"));
    strcpy(fp_name, name);
    strcat(fp_name, ".bin");
    FILE* cache = fopen(fp_name, "wb+");
    if (cache == NULL) {
        print("Unable to access your account");
        perror("Insufficient funds");
        Sleep(3000);
        return;
    }
    char tmp[2][18];
    int count = 0;
    double moneyT;
    double mMoney;

    const char prompts[][40] = { "Enter the recipients name: ", "Enter the transfer amount: ", };
    print("Since this is a private bank, only domestic transactions are available");

    print(prompts[0]);
    scanf("%18s", &tmp[0]);
    print(prompts[1]);
    scanf("%lf", &mMoney);
    
    if (fscanf(cache, "%lf", &moneyT) == 1 && fseek(cache, 0, SEEK_END)) {
        moneyT -= mMoney;
        if (moneyT >= 0) {
            fprintf(cache, "%.2lf", moneyT);
            print("Transaction complete");
            printf("Your reimaining funds are: %.2lf", moneyT);
            Sleep(3000);
        }
        else {
            print("Insuficient funds");
            printf("Your reimaining funds are: %.2lf", moneyT);
            Sleep(3000);
        }
    }
    else {
        print("Insuficient funds to transfer");
        moneyT = 0;
        printf("Your reimaining funds are: %.2lf", moneyT);
        Sleep(3000);
    }

    free(fp_name);
    fclose(cache);
    return;
}

int userInterface(bool *status, int instance, char* NAME) {
    //move of arrow
    bool state = true;
    int selCode = 0;
    char name[18];
    const char options[][16] = {"Register", "Login", "Balance", "Transfer"};
    int selBlocks[][2] = { {0, 1}, {2, 3} };
    char printName[40];
    strcpy(printName, "Currently logged in as: ");
    if (instance == 1) {
        strcat(printName, NAME);
        strcpy(name, NAME);
    }

    do {
        //if presses any of the movement keys then do modullus 2 between selCode and 1 in order to figure out which option is selected
        //W and S are also used since im an abosolute unit at video games
        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) {
            selCode = selCode ^ 1;
            state = true;
        }

        //switch statements unable to make since (GetAsyncKeyState(VK_RETURN) & 0x8000) is not liked by switch, same as selBlocks[instance][selCode] == n
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (selBlocks[instance][selCode] == 0) {
                // register
                system("cls");
                registerUser();
                selBlocks[instance][selCode] = 1;
            }
            if (selBlocks[instance][selCode] == 1) {
                // login
                system("cls");

                status = loginUser(name);
                assert(status != NULL);
                system("cls");
                print("Welcome Mr/Mrs. ");
                print(name);
                
                strcpy(NAME, name);
                Sleep(3000);
                if (status == true) {
                    return 1;
                }
            }
            if (selBlocks[instance][selCode] == 2) {
                // balance
                fflush(stdin);
                system("cls");
                showBalance(1, name);
                state = true;
            }
            if (selBlocks[instance][selCode] == 3) {
                // transfer
                fflush(stdin);
                system("cls");
                transferMoney(name);
                state = true;
            }
            
        }

        if (state) {
            system("cls");
            if (instance != 1)
                print("Login status: false");
            else
                print(printName);

            print(strupr(options[selBlocks[instance][selCode]]));
            print(strlwr(options[selBlocks[instance][selCode^1]]));
            state = false;
        }
    } while (1);
}

void main() {
    int instance = 0;
    bool status = false;
    char name[18];
    print("|\\/\\/| Welcome to the Levi banking System |\\/\\/|\n");
    print("This banking system is only available to select few");
    print("Thus behave or measures will be taken\n\n");
    print("Enjoy your stay");
    Sleep(5000);
    while (1) {
        instance += userInterface(&status, instance, name);
        if (instance == 0)
            status = false;
    }
}
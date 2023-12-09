#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#define u_char unsigned char

bool regist() {
	/*
	May god himself forgive me for this noodle code...
	*/

	FILE* cache;
	cache = fopen("database.txt", "a+");
	char prompts[][30] = { "Enter your full name: ", "Enter your password: ", "Reenter your password: " };
	bool flags[3] = { false, false, false };
	int count = 0;
	int size[3] = { 30, 20, 20 };
	char tmp[3][30] = { 0 };

	do {
		system("cls");
		printf("< Registration >\n%s", prompts[count]);
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
						printf("Passwords do not match. Please re-enter.\n");
						count = 0;
					}
				}
			}
			else {
				printf("Invalid input\n");
				count = 0;
			}
		}
		else {
			printf("Invalid input\n");
			count = 0;
			scanf("%*s");
		}
	} while (count < 3);


	for (int i = 0; i < 3; ++i) {
		if (!flags[i]) {
			printf("Registration failed. Please try again.\n");
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


	fprintf(cache, "\n%s\n%s\n%s\n--------------", tmp[0], bonkers, tmp[1]);
	fclose(cache);
	Sleep(500);
	return true;
}

bool login() {
	FILE* cache;
	cache = fopen("database.txt", "r");
	char prompts[][30] = { "Enter your full name: ", "Enter your user id: ", "Enter your password: " };
	int count = 0;
	int size[3] = { 30, 6, 20 };
	char tmp[30];
	char line[30];
	bool flags[3] = { false, false, false };
	do {
		system("cls");
		printf("< login >\n%s", prompts[count]);
		if (scanf("%29s", &tmp) == 1) {
			if (strlen(tmp) <= size[count]) {
				while (fgets(line, 30, cache) != NULL) {
					if (strstr(line, tmp) != NULL) {
						flags[count++] = true;
						break;
					}
					else {
						count = 0;
					}
				}
			}
		}
	} while (count < 3);

	fclose(cache);
	for (int i = 0; i < 3; ++i)
		if (flags[i] == false)
			return false;

	return true;
}


void showTransfer() {
	printf("Transfer function is under construction.\n");
	Sleep(5000);
}

void showBalance() {
	printf("Balance function is under construction.\n");
	Sleep(5000);
}


char prompt(u_char instance, bool status) {
	char sel[][30] = { {"Login"}, {"Register"}, {"Transfer"}, {"Balance"}, {"Local transfer"}, {"Extern transfer"} };
	u_char sel_codes[][2] = { {0, 1}, {2, 3}, {4, 5} };

	int count = 0;
	bool state = true;

	do {
		if (status == false && instance >= 1)
			return 0;

		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
			count = count ^ 1;
			state = true;
		}

		if (state == true) {
			system("cls");
			printf("[=--> %s <--=]\n(-- %s --)\n", sel[sel_codes[instance][count ^ 1]], sel[sel_codes[instance][count]]);
			state = false;
		}

		if (GetAsyncKeyState(VK_RETURN)) {
			if (instance == 2) {
				showTransfer();
				return 0;
			}
			else if (instance == 3) {
				showBalance();
				return 0;
			}
			else if (instance == 1) {
				return instance;
			}
			else {
				return instance += count;
			}
		}

		if (GetAsyncKeyState(VK_TAB)) {
			if (instance == 1)
				status = false;
			if ((instance >= 1) && (status == true)) {
				return instance -= 1;
			}
			else {
				return -instance;
			}
		}
		Sleep(300);
	} while (1);

	return instance;
}

void main() {
	u_char bonk;
	u_char instance = 0;
	bool status = false;
	while (1) {
		if (1 == instance) {
			status = login();
			if (status == false)
				instance = 0;
		}
		bonk = prompt(instance, status);
		instance += bonk;
		if (0 == instance) {
			status = regist();
			if (status == false)
				instance = 0;
			else
				instance = 1;
		}
	}
}
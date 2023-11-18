#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#define u_char unsigned char

typedef struct {
	char fname[30];
	char user_id[10];
	char pword[20];
} info;
/*
bool regist(bool status) {
	while (1) {
		
	}


	return status;
}*/

bool login() {
	system("cls");
	FILE* cache = fopen("database.txt", "r");
	bool flags[3] = { false, false, false };
	char prompts[3][30] = { "enter your fname : ", "enter user id: ", "enter your password: " };
	char tmp[3][30] = { 0 };
	int sizers[3] = { 30, 10, 20 };
	u_char count = 0;
	while (count < 3) {
		printf("%s", prompts[count]);
		
		if (scanf("%29s", &tmp[count]) != 0) {
			// Remove the newline character from the input
			tmp[count][strcspn(tmp[count], "\n")] = 0;
			if (sizeof(tmp[count]) == sizers[count]) {
				char line[100];
				rewind(cache); // Reset the file pointer to the beginning of the file
				while (fgets(line, sizeof(line), cache) != NULL) {
					// Remove the newline character from the line
					line[strcspn(line, "\n")] = 0; 
					if (strcmp(line, tmp[count]) == 0) {
						flags[count++] = true;
						system("cls");
						break;
					}
				}
			}
			else
				printf("Input too long, must be %d characters long\n", sizers[count]);
		}
		else {
			printf("invalid input");
			while (getchar() != '\n')
				;
		}
	}
	fclose(cache);
	for (int i = 0; i < 3; i++) {
		if (flags[i] == false)
			return false;
	}
	return true;
}

char prompt(u_char instance, bool status) {
	char sel[][30] = { {"Login"}, {"Register"}, {"Transfer"}, {"Balance"}, {"Local transfer"}, {"Extern transfer" } }; // selection names
	u_char sel_codes[][2] = { {0, 1}, {3, 2}, {4, 5} }; /*{"Transfer"}, {"Balance"}, {""}};*/ // selection codes
	// if this was lua or py this would be easier to do by just saying do x in pairs y;

	u_int count = 0; // basically max selections available on screen so every time enter is pressed count goes up

	bool state = true; //state if the selection changes
	do {
		if (status == false && instance >= 1)
			return 0;
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
			count = count ^ 1;
			state = true;
		}
		if (state == true) {
			system("cls");
			printf("[=--> %s <--=]\n(-- %s --)", sel[sel_codes[instance][count^1]], sel[sel_codes[instance][count]]);

			state = false;
		}

		if (GetAsyncKeyState(VK_RETURN))
			return instance += count;

		if (GetAsyncKeyState(VK_TAB)) {
			if (instance == 1)
				status = false;
			if ((instance >= 1) && (state == true)) {
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
	info x;
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
	}
}
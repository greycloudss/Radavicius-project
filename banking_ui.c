#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#define u_char unsigned char

typedef struct {
	char fname[30];
	char email[30];
	char user_id[10];
	char phone[20];
	char pword[20];
} info;



bool login(info* x) {
	system("cls");
	FILE* cache;
	cache = fopen("database.bin", "rb+");
	bool flag = false;
	char username[30], password[30];
	printf("Enter username: ");
	fgets(username, 30, stdin);
	printf("Enter password: ");
	fgets(password, 30, stdin);
	username[strcspn(username, "\n")] = 0; // remove trailing newline
	password[strcspn(password, "\n")] = 0; // remove trailing newline
	info user;
	while (fread(&user, sizeof(info), 1, cache)) {

		if (strcmp(username, user.user_id) == 0 && strcmp(password, user.pword) == 0) {
			printf("Login successful\n");
			flag = true;
			break;

		}
	}
	printf("Incorrect credentials\n");
	fclose(cache);
	return flag;
}

char prompt(u_char instance, bool status) {
	char sel[][30] = { {"Login"}, {"Register"}, {"Transfer"}, {"Balance"}, {"Local transfer"}, {"Extern transfer" } }; // selection names
	u_char sel_codes[][2] = { {0, 1}, {2, 3}, {4, 5} }; /*{"Transfer"}, {"Balance"}, {""}};*/ // selection codes
	// if this was lua or py this would be easier to do by just saying do x in pairs y;

	u_int count = 0; // basically max selections available on screen so every time enter is pressed count goes up

	bool state = true; //state if the selection changes
	do {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
			count = count ^ 1;
			state = true;
		}
		if (state == true) {
			system("cls");
			printf("--%d--", instance);
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
			status = login(&x);

		}
		bonk = prompt(instance, status);
		instance += bonk;
	}

}
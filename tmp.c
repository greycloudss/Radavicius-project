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

char prompt(u_char instance) {
	char sel[][30] = { {"Login"}, {"Register"}, {"Transfer"}, {"Balance"}, "Local Trasnfer", "Extern Transfer"}; // selection names
	u_char sel_codes[][2] = { {2, 3}, {4, 5} }; /*{"Transfer"}, {"Balance"}, {""}};*/ // selection codes
	// if this was lua or py this would be easier to do by just saying do x in pairs y;
	
	u_int count = 0; // basically max selections available on screen so every time enter is pressed count goes up
	
	bool state = true; //state if the selection changes
	do {
		if (GetAsyncKeyState(VK_TAB)) {
			state = true;
			instance = 0; //fuck around and youll need to log in again
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN))
			count = count ^ 1;

		if (GetAsyncKeyState(VK_RETURN)) {
			state = true;
			instance = count;
		}
		
		if (true == state) {
			//if ()
		}

	} while (1);
	

	return 0;
}


void main() {
	info x;
	u_char instance = 0;
	while (1) {
		
	}

}

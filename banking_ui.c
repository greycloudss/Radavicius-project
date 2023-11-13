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



void login(info* x) {
	FILE* cache;
	cache = fopen("database.bin", "rb+");
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
			return;
		}
	}
	printf("Incorrect credentials\n");
	fclose(cache);
}

char prompt(u_char instance) {

	return 0;
}


void main() {
	info x;
	login_reg(&x);

}
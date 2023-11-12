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



void login_reg(u_char state, info* x) {
	FILE* cache;
	cache = fopen("database.bin", "rb+");
	char tmp[2][5][30];
	int count = 0, count1 = 0;
	bool flag = false;
	if (state == 1) { //login
		while (count < 4) {
			gets(tmp[count++]);
		}

		while (1) {
			if (strcmp(tmp[0][1], fgets(tmp[1][1], 30, cache)) == 0) {
				if (strcmp(tmp[0][count1], fgets(tmp[1][count1], 30, cache)) == 0) {
					++count1;
				}
				else {
					printf("Incorrect credentials");
					break;
				}
				if (count1 == 4) {
					flag = true;
					printf("Credentials: %b", flag);
				}
			}
		}
	}
	else { //register
		
		//fprintf("");
	}
	fclose(cache);
}

int prompt( instance) {
	
}


void main() {
	info x;
	login_reg(1, &x);

}
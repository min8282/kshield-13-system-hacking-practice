#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	char cmd[20] = "ls ";
	char input[10] = {0, };
	fgets(input, 10, stdin);
	strcat(cmd, input);
	system(cmd);
	return 0;
}

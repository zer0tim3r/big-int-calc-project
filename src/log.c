#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 30

// load library
void print_num_withoutLog(char[], int, _Bool); // function.h

// declaration
void logging(char log[100][108], int log_count, char put[100], _Bool type);
void load_screen(char log[100][108], int log_count, char vars[5][MAX_LENGTH], int vars_SF[5], _Bool vars_sign[5]);

// definition
void logging(char log[100][108], int log_count, char put[100], _Bool type)
{ // type => TRUE : input | FALSE : output
	char temp[108];
	if (type)
		strcpy(temp, "(input) ");
	else
		strcpy(temp, "======> ");
	strcat(temp, put);
	strcpy(log[log_count], temp);
}

void load_screen(char log[100][108], int log_count, char vars[5][MAX_LENGTH], int vars_SF[5], _Bool vars_sign[5])
{
	system("clear");
	printf("************************************************************************\n");
	printf("                          큰 정 수 계 산 기\n");
	printf("************************************************************************\n");
	printf("변 수\n");
	printf("************************************************************************\n");

	for (int i = 0; i < 5; i++)
	{
		printf("%c = ", 'a' + i);
		print_num_withoutLog(vars[i], vars_SF[i], vars_sign[i]);
	}
	printf("************************************************************************\n");
	printf("Help : H(istory) L(oad) R(efresh) (rese)T S(ave) Q(uit)\n");
	printf("************************************************************************\n");
	for (int i = 0; i < log_count; i++)
	{
		for (int j = 0; j < 109 && log[i][j] != 0; j++)
			putchar(log[i][j]);
		putchar('\n');
	}
}

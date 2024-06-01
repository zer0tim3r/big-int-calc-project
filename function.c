#include <stdio.h>
#include <stdbool.h>

#define MAX_LENGTH 30

// libraries
void logging(char[100][108], int, char[], bool);

// declaration
void numcpy(char[], char[]);
int get_SF(char[]);
void print_output(char[100][108], int, char[]);
void print_num(char[100][108], int, char[], int, bool);
void print_num_withoutLog(char[], int, bool);
void print_error(char[100][108], int);

// definition
void numcpy(char var1[], char var2[])
{
	int i;
	for (i = 0; i < MAX_LENGTH; i++)
		var1[i] = var2[i];
}

int get_SF(char var[])
{
	int i;
	for (i = 29; i > -1; i--)
	{
		if (var[i] != -48)
			break;
	}
	return i + 1;
}

void print_output(char log[100][108], int log_count, char output[])
{
	int i;
	printf("======> ");
	for (i = 0; i < 46 && output[i] != 0; i++)
	{
		// printf("%c", output[i]);
		putchar(output[i]);
	}
	putchar('\n');
	logging(log, log_count, output, false);
}

void print_num(char log[100][108], int log_count, char var[], int SF, bool sign)
{
	int i, j, index;
	int I = SF % 3, J = SF / 3 - 1;
	char temp[41] = {0};
	int temp_count = 0;

	if (!sign && var[0] != 0)
	{
		temp[temp_count] = '-';
		temp_count++;
	}
	for (i = 0, index = 0; i < I; i++, index++)
	{
		temp[temp_count] = var[index] + '0';
		temp_count++;
	}
	if (I != 0 && J != -1)
	{
		temp[temp_count] = ',';
		temp_count++;
	}
	for (i = 0; i < J; i++)
	{
		for (j = 0; j < 3; j++, index++)
		{
			temp[temp_count] = var[index] + '0';
			temp_count++;
		}
		temp[temp_count] = ',';
		temp_count++;
	}
	if (J != -1)
	{
		for (j = 0; j < 3; j++, index++)
		{
			temp[temp_count] = var[index] + '0';
			temp_count++;
		}
	}
	print_output(log, log_count, temp);
}

void print_num_withoutLog(char var[], int SF, bool sign)
{
	int i, j, index;
	int I = SF % 3, J = SF / 3 - 1;
	if (!sign && var[0] != 0)
		putchar('-');
	for (i = 0, index = 0; i < I; i++, index++)
	{
		putchar(var[index] + '0');
	}
	if (I != 0 && J != -1)
	{
		putchar(',');
	}
	for (i = 0; i < J; i++)
	{
		for (j = 0; j < 3; j++, index++)
		{
			putchar(var[index] + '0');
		}
		putchar(',');
	}
	if (J != -1)
	{
		for (j = 0; j < 3; j++, index++)
		{
			putchar(var[index] + '0');
		}
	}
	putchar('\n');
}

void print_error(char log[100][108], int log_count)
{
	char error[6] = "error";
	print_output(log, log_count, error);
}

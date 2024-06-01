#include <stdio.h>
#include <stdbool.h>

#define MAX_LENGTH 30

// declaration
void write_file(char[5][MAX_LENGTH], int[], bool[], char[3][MAX_LENGTH], int[], bool[]);
void read_file(char[5][MAX_LENGTH], int[], bool[], char[3][MAX_LENGTH], int[], bool[]);

// definition
void write_file(char vars[5][MAX_LENGTH], int vars_SF[], bool vars_sign[], char histories[3][MAX_LENGTH], int histories_SF[], bool histories_sign[])
{
	int i, j;
	FILE *out = fopen("cal.txt", "w");
	for (i = 0; i < 5; i++)
	{
		putc(vars_sign[i] ? '+' : '-', out);
		for (j = 0; j < vars_SF[i]; j++)
			putc(vars[i][j] + '0', out);
		putc('\n', out);
	}
	for (i = 0; i < 3; i++)
	{
		putc(histories_sign[i] ? '+' : '-', out);
		for (j = 0; j < histories_SF[i]; j++)
			putc(histories[i][j] + '0', out);
		putc('\n', out);
	}
	fclose(out);
}

void read_file(char vars[5][MAX_LENGTH], int vars_SF[], bool vars_sign[], char histories[3][MAX_LENGTH], int histories_SF[], bool histories_sign[])
{
	int i, j;
	char c;
	FILE *in = fopen("cal.txt", "r");
	for (i = 0; i < 5; i++)
	{
		vars_sign[i] = (getc(in) == '+') ? true : false;
		for (j = 0; (c = getc(in)) != '\n' && j < 30; j++)
			vars[i][j] = c - '0';
		vars_SF[i] = j;
	}
	for (i = 0; i < 3; i++)
	{
		histories_sign[i] = (getc(in) == '+') ? true : false;
		for (j = 0; (c = getc(in)) != '\n' && j < 30; j++)
			histories[i][j] = c - '0';
		histories_SF[i] = j;
	}
	fclose(in);
}

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 30

// declaration
void write_file(char[5][MAX_LENGTH], int[], _Bool[], char[3][MAX_LENGTH], int[], _Bool[]);
void read_file(char[5][MAX_LENGTH], int[], _Bool[], char[3][MAX_LENGTH], int[], _Bool[]);

// definition
void write_file(char vars[5][MAX_LENGTH], int vars_SF[], _Bool vars_sign[], char histories[3][MAX_LENGTH], int histories_SF[], _Bool histories_sign[])
{
	FILE *out = fopen("cal.txt", "w");

	for (int i = 0; i < 5; i++)
	{
		putc(vars_sign[i] ? '+' : '-', out);
		for (int j = 0; j < vars_SF[i]; j++)
			putc(vars[i][j] + '0', out);
		putc('\n', out);
	}

	for (int i = 0; i < 3; i++)
	{
		putc(histories_sign[i] ? '+' : '-', out);
		for (int j = 0; j < histories_SF[i]; j++)
			putc(histories[i][j] + '0', out);
		putc('\n', out);
	}

	fclose(out);
}

void read_file(char vars[5][MAX_LENGTH], int vars_SF[], _Bool vars_sign[], char histories[3][MAX_LENGTH], int histories_SF[], _Bool histories_sign[])
{
	int i, j;
	char c;
	FILE *in = fopen("cal.txt", "r");

	for (i = 0; i < 5; i++)
	{
		vars_sign[i] = (getc(in) == '+') ? TRUE : FALSE;
		for (j = 0; (c = getc(in)) != '\n' && j < 30; j++)
			vars[i][j] = c - '0';
		vars_SF[i] = j;
	}

	for (i = 0; i < 3; i++)
	{
		histories_sign[i] = (getc(in) == '+') ? TRUE : FALSE;
		for (j = 0; (c = getc(in)) != '\n' && j < 30; j++)
			histories[i][j] = c - '0';
		histories_SF[i] = j;
	}

	fclose(in);
}

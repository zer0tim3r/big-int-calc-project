#include <stdbool.h>
#define MAX_LENGTH 30

// libraries
void print_output(char[100][108], int, char[]);
void numcpy(char[], char[]);

// declaration
void print_histories(char[100][108], int, char[3][MAX_LENGTH], int[], bool[]);
void write_history(char[3][MAX_LENGTH], int[], bool[], char[], int, bool);

// definition
void print_histories(char log[100][108], int log_count, char histories[3][MAX_LENGTH], int histories_SF[], bool histories_sign[])
{
	int h, i, j, index;
	int I, J;
	char temp[46] = {'h', '0', ' ', '=', ' '};
	int temp_count;

	for (h = 0; h < 3; h++)
	{
		I = histories_SF[h] % 3;
		J = histories_SF[h] / 3 - 1;
		for (i = 5; i < 46; i++)
			temp[i] = 0;
		temp[1] = h + '1';
		temp_count = 5;
		if (!histories_sign[h] && histories[h][0] != 0)
		{
			temp[temp_count] = '-';
			temp_count++;
		}
		for (i = 0, index = 0; i < I; i++, index++)
		{
			temp[temp_count] = histories[h][index] + '0';
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
				temp[temp_count] = histories[h][index] + '0';
				temp_count++;
			}
			temp[temp_count] = ',';
			temp_count++;
		}
		if (J != -1)
		{
			for (j = 0; j < 3; j++, index++)
			{
				temp[temp_count] = histories[h][index] + '0';
				temp_count++;
			}
		}
		print_output(log, log_count + h, temp);
	}
}

void write_history(char histories[3][MAX_LENGTH], int histories_SF[], bool histories_sign[], char result[], int result_SF, bool result_sign)
{
	numcpy(histories[2], histories[1]);
	numcpy(histories[1], histories[0]);
	numcpy(histories[0], result);
	histories_SF[2] = histories_SF[1];
	histories_SF[1] = histories_SF[0];
	histories_SF[0] = result_SF;
	histories_sign[2] = histories_sign[1];
	histories_sign[1] = histories_sign[0];
	histories_sign[0] = result_sign;
}

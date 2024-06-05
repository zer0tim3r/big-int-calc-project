#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 30

// log.c
void logging(char[100][108], int, char[], _Bool);

// declaration
void numcpy(char[], char[]);
int get_SF(char[]);
void print_output(char[100][108], int, char[]);
void print_num(char[100][108], int, char[], int, _Bool);
void print_num_withoutLog(char[], int, _Bool);
void print_error(char[100][108], int);
int decimal2hex(char[], char[]);

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
	logging(log, log_count, output, FALSE);
}

void print_num(char log[100][108], int log_count, char var[], int SF, _Bool sign)
{
	int index = 0;
	int I = SF % 3, J = SF / 3 - 1;
	char temp[41] = {0};
	int temp_count = 0;

	if (!sign && var[0] != 0)
	{
		temp[temp_count] = '-';
		temp_count++;
	}
	for (int i = 0; i < I; i++, index++)
	{
		temp[temp_count] = var[index] + '0';
		temp_count++;
	}
	if (I != 0 && J != -1)
	{
		temp[temp_count] = ',';
		temp_count++;
	}
	for (int i = 0; i < J; i++)
	{
		for (int j = 0; j < 3; j++, index++)
		{
			temp[temp_count] = var[index] + '0';
			temp_count++;
		}
		temp[temp_count] = ',';
		temp_count++;
	}
	if (J != -1)
	{
		for (int j = 0; j < 3; j++, index++)
		{
			temp[temp_count] = var[index] + '0';
			temp_count++;
		}
	}
	print_output(log, log_count, temp);
}

void print_num_withoutLog(char var[], int SF, _Bool sign)
{
	int index = 0;
	int I = SF % 3, J = SF / 3 - 1;
	if (!sign && var[0] != 0)
		putchar('-');
	for (int i = 0; i < I; i++, index++)
	{
		putchar(var[index] + '0');
	}
	if (I != 0 && J != -1)
	{
		putchar(',');
	}
	for (int i = 0; i < J; i++)
	{
		for (int j = 0; j < 3; j++, index++)
		{
			putchar(var[index] + '0');
		}
		putchar(',');
	}
	if (J != -1)
	{
		for (int j = 0; j < 3; j++, index++)
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

int hexCharToDecimal(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else
	{
		return (c >= 'a' && c <= 'f' ? c - 'a' : c - 'A') + 10;
	}
}

int decimal2hex(char input[], char result[])
{
	input += 1; // remove #

	int input_len = 0;
	for (; input[input_len]; input_len++) 0; // strlen
    int digits[100] = {0};  // 10진수 각 자리를 저장하는 배열
    int digits_len = 1;

    // 각 16진수 문자를 처리
    for (int i = 0; i < input_len; i++) {
        int carry = hexCharToDecimal(input[i]);

        // 기존의 모든 10진수 자리에 대해 곱셈 및 더하기 연산
        for (int j = 0; j < digits_len; j++) {
            int value = digits[j] * 16 + carry;
            digits[j] = value % 10;
            carry = value / 10;
        }

        // 캐리 처리
        while (carry > 0) {
            digits[digits_len] = carry % 10;
            carry /= 10;
            digits_len++;
        }
    }

    // 10진수 숫자를 문자열로 변환
    for (int i = 0; i < digits_len; i++) {
        result[digits_len - i - 1] = digits[i] + '0';
    }

    result[digits_len] = 0; // END OF STR

	return 1;
}

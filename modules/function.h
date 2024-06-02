
#include "common.h"

// libraries
void logging(char[100][108], int, char[], bool);

// declaration
void numcpy(char[], char[]);
int get_SF(char[]);
void print_output(char[100][108], int, char[]);
void print_num(char[100][108], int, char[], int, bool);
void print_num_withoutLog(char[], int, bool);
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
	input += 1;
	int len = 0;
	for (; input[len]; len++) 0; // strlen
    int decimalDigits[100] = {0};  // 10진수 각 자리를 저장하는 배열
    int decimalLen = 1;

    // 각 16진수 문자를 처리
    for (int i = 0; i < len; i++) {
        int carry = hexCharToDecimal(input[i]);

        // 기존의 모든 10진수 자리에 대해 곱셈 및 더하기 연산
        for (int j = 0; j < decimalLen; j++) {
            int value = decimalDigits[j] * 16 + carry;
            decimalDigits[j] = value % 10;
            carry = value / 10;
        }

        // 캐리 처리
        while (carry > 0) {
            decimalDigits[decimalLen] = carry % 10;
            carry /= 10;
            decimalLen++;
        }
    }

    // 10진수 숫자를 문자열로 변환
    for (int i = 0; i < decimalLen; i++) {
        result[decimalLen - i - 1] = decimalDigits[i] + '0';
    }
    result[decimalLen] = '\0';

	return 1;
}

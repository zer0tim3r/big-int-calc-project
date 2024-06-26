#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 30

// calculate.c
int general_calculate(char input[], char vars[5][MAX_LENGTH], int vars_SF[], _Bool vars_sign[], char historys[3][MAX_LENGTH], int historys_SF[], _Bool historys_sign[], char result[]);
int sum(char[], int, _Bool, char[], int, _Bool, char[]);
int multiple(char[], int, _Bool, char[], int, _Bool, char[]);
int divide(char[], int, _Bool, char[], int, _Bool, char[]);
int get_remainder(char[], int, _Bool, char[], int, _Bool, char[]);
void sign_processing(char[], int, _Bool); // value of array changes by sign.
void convert_bit_order(char array[], int SF);

// file.c
void write_file(char[5][MAX_LENGTH], int[], _Bool[], char[3][MAX_LENGTH], int[], _Bool[]);
void read_file(char[5][MAX_LENGTH], int[], _Bool[], char[3][MAX_LENGTH], int[], _Bool[]);

// function.c
void numcpy(char[], char[]);
int get_SF(char[]);
void print_output(char[100][108], int, char[]);
void print_num(char[100][108], int, char[], int, _Bool);
void print_num_withoutLog(char[], int, _Bool);
void print_error(char[100][108], int);
int decimal2hex(char[], char[]);

// history.c
void print_histories(char[100][108], int, char[3][MAX_LENGTH], int[], _Bool[]);
void write_history(char[3][MAX_LENGTH], int[], _Bool[], char[], int, _Bool);

// log.c
void logging(char log[100][108], int log_count, char put[100], _Bool type);
void load_screen(char log[100][108], int log_count, char vars[5][MAX_LENGTH], int vars_SF[5], _Bool vars_sign[5]);


void get_input(char* serialized) {
	
}

int main()
{
	char log[100][108];
	int log_count = 0;

	char input[100];
	char output[45];
	int command; // it indicates is input command? => -1 : not command, else : refer to index in array-input

	char vars[5][MAX_LENGTH];
	char histories[3][MAX_LENGTH];
	char result[MAX_LENGTH];

	int vars_SF[5];
	int histories_SF[3];
	int result_SF;

	_Bool vars_sign[5];
	_Bool histories_sign[3];
	_Bool result_sign;

	int i, j;
	char c;
	char var_temp = -1;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 30; j++)
			vars[i][j] = -48;
		vars[i][0] = 0;
		vars_SF[i] = 1;
		vars_sign[i] = TRUE;
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 30; j++)
			histories[i][j] = -48;
		histories[i][0] = 0;
		histories_SF[i] = 1;
		histories_sign[i] = TRUE;
	}

	load_screen(log, log_count, vars, vars_SF, vars_sign);

	while (1)
	{
		command = -1;
		printf("(input) ");
		for (i = 0; i < 100; i++)
			input[i] = 0;
		for (i = 0; (c = getchar()) != '\n' && i < 100; i++)
			input[i] = c;
		logging(log, log_count, input, TRUE);
		log_count++;
		if (i == 100 && c != '\n')
		{
			while ((c = getchar()) != '\n')
				;
			print_error(log, log_count);
			log_count++;
		}
		else
		{

			// check if command typed
			for (i = 0; i < 100 && input[i] != 0; i++)
			{
				if (input[i] != ' ')
				{
					if (input[i] >= '0' && input[i] <= '9')
						break;
					else if (input[i] >= 'a' && input[i] <= 'e')
						break;
					else if (input[i] >= 'A' && input[i] <= 'E')
						break;
					command = i; // character presumed as command exists.
					break;
				}
			}
			if (command != -1)
			{
				for (i++; i < 100 && input[i] != 0; i++)
				{
					if (input[i] != ' ')
					{
						command = -1; // the guess above was wrong.
						break;
					}
				}
			}

			// process input
			if (command != -1)
			{
				switch (input[command])
				{
				case 'Q': // Quit
				case 'q':
					printf("======> bye\n");
					return 0;
				case 'H': // History
				case 'h':
					print_histories(log, log_count, histories, histories_SF, histories_sign);
					log_count += 3;
					break;
				case 'R': // Refresh
				case 'r':
					log_count = 0;
					load_screen(log, log_count, vars, vars_SF, vars_sign);
					break;
				case 'T': // reseT
				case 't':
					for (i = 0; i < 5; i++)
					{
						for (j = 1; j < 30; j++)
							vars[i][j] = -48;
						vars[i][0] = 0;
						vars_SF[i] = 1;
						vars_sign[i] = TRUE;
					}
					// write_history(a) can be worked.
					for (i = 0; i < 3; i++)
					{
						for (j = 1; j < 30; j++)
							histories[i][j] = -48;
						histories[i][0] = 0;
						histories_SF[i] = 1;
						histories_sign[i] = TRUE;
					}
					log_count = 0;
					load_screen(log, log_count, vars, vars_SF, vars_sign);
					break;
				case 'S': // Save
				case 's':
					write_file(vars, vars_SF, vars_sign, histories, histories_SF, histories_sign);
					print_output(log, log_count, "cal.txt에 저장");
					log_count++;
					break;
				case 'L': // Load
				case 'l':
					read_file(vars, vars_SF, vars_sign, histories, histories_SF, histories_sign);
					log_count = 0;
					load_screen(log, log_count, vars, vars_SF, vars_sign);
					print_output(log, log_count, "cal.txt로부터 복구");
					log_count++;
					break;
				default: // wrong command input
					print_error(log, log_count);
					log_count++;
				}
			}
			else
			{
				for (i = 0; i < 100 && input[i] == ' '; i++)
					;
				if ((i < 100) && (input[i] == '#'))
				{
					switch (decimal2hex(input, result))
					{
					case -1:
						print_error(log, log_count);
						log_count++;
						break;
					case 1:
						print_output(log, log_count, result);
						log_count++;
						break;
					}
					continue;
				}
				if ((i < 100) && ((input[i] >= 'a' && input[i] <= 'e') || (input[i] >= 'A' && input[i] <= 'E')))
				{ // probably definition of variable or assignment of variable using operator, '='.
					for (i++; i < 100 && input[i] == ' '; i++)
						;
					if (i < 100 && input[i] == '=')
					{ // nearest conjecture of condition is correct.
						input[i] = ' ';
						for (j = i; j > -1 && input[j] == ' '; j--)
							;
						var_temp = input[j] - 'A';
						if (var_temp > 4)		   // variable is upper-case letter
							var_temp += 'A' - 'a'; // var_temp = input[i] - 'A' + 'A' - 'a' = input[i] - 'a'.
						input[j] = ' ';
					}
				}
				switch (general_calculate(input, vars, vars_SF, vars_sign, histories, histories_SF, histories_sign, result))
				{
				case -1:
					print_error(log, log_count);
					log_count++;
					break;
				case 0:
					result_sign = FALSE;
					result_SF = get_SF(result);

					if (var_temp != -1)
					{
						numcpy(vars[var_temp], result);
						vars_SF[var_temp] = result_SF;
						vars_sign[var_temp] = result_sign;
						load_screen(log, log_count, vars, vars_SF, vars_sign);
					}

					print_num(log, log_count, result, result_SF, result_sign);
					log_count++;

					write_history(histories, histories_SF, histories_sign, result, result_SF, result_sign);
					break;
				case 1:
					result_sign = TRUE;
					result_SF = get_SF(result);

					if (var_temp != -1)
					{
						numcpy(vars[var_temp], result);
						vars_SF[var_temp] = result_SF;
						vars_sign[var_temp] = result_sign;
						load_screen(log, log_count, vars, vars_SF, vars_sign);
					}

					print_num(log, log_count, result, result_SF, result_sign);
					log_count++;

					write_history(histories, histories_SF, histories_sign, result, result_SF, result_sign);
					break;
				}
				var_temp = -1;
			}
		}
	}

	return 0; // unreachable code
}

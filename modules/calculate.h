#include "common.h"

// libraries
int get_SF(char[]);
void numcpy(char[], char[]);

// declaration
int general_calculate(char input[], char vars[5][MAX_LENGTH], int vars_SF[], bool vars_sign[], char historys[3][MAX_LENGTH], int historys_SF[], bool historys_sign[], char result[]);
int sum(char[], int, bool, char[], int, bool, char[]);
int multiple(char[], int, bool, char[], int, bool, char[]);
int divide(char[], int, bool, char[], int, bool, char[]);
int get_remainder(char[], int, bool, char[], int, bool, char[]);
void sign_processing(char[], int, bool); // value of array changes by sign.
void convert_bit_order(char array[], int SF);

// definition
int general_calculate(char input[], char vars[5][MAX_LENGTH], int vars_SF[], bool vars_sign[], char historys[3][MAX_LENGTH], int historys_SF[], bool historys_sign[], char result[])
{
	int i;

	char init[MAX_LENGTH];
	for (i = 0; i < MAX_LENGTH; i++)
		init[i] = -48;

	char buffer[MAX_LENGTH];
	numcpy(buffer, init); // we initialize buffer to use var:init.
	int buffer_SF = 0;

	int index_pointer; // It points index of input
	int operator_pointer = 0;
	int operand_pointer = 0;
	int operation_pointer = 0;
	int address_pointer;

	int previous_operation_element; // 0 : operator, 1 : variable(flushed number included), 2 : number(writing), 3 : number(wroten) - deleted
	char operators[50];
	char operands[50][MAX_LENGTH];
	int operands_SF[50];
	bool operands_sign[50];
	bool valid_operator_before_unary = true;
	int address[50]; // it initialize with same index. after calculate, address of following operand saved in index of precedented operand. for example, after calculate a[3] * 5[4], address[3] = 4;

	char temp[MAX_LENGTH];

	static char int_MAX[MAX_LENGTH] = {2, 1, 4, 7, 4, 8, 3, 6, 4, -1, -48}; // it will be used when processed # operator.

	for (i = 0; i < 50; i++)
		operands_sign[i] = true;																 // prepare for unary.
	for (index_pointer = 0; index_pointer < 100 && input[index_pointer] == ' '; index_pointer++) // short circuit evaluate prevents core dump : out of index
		;
	if (input[index_pointer] >= 'a' && input[index_pointer] <= 'e')
	{
		previous_operation_element = 1;
		numcpy(operands[operand_pointer], vars[input[index_pointer] - 'a']);
		operands_SF[operand_pointer] = vars_SF[input[index_pointer] - 'a'];
		operands_sign[operand_pointer] = vars_sign[input[index_pointer] - 'a'];
		operand_pointer++;
	}
	else if (input[index_pointer] >= 'A' && input[index_pointer] <= 'E')
	{
		previous_operation_element = 1;
		numcpy(operands[operand_pointer], vars[input[index_pointer] - 'A']);
		operands_SF[operand_pointer] = vars_SF[input[index_pointer] - 'A'];
		operands_sign[operand_pointer] = vars_sign[input[index_pointer] - 'A'];
		operand_pointer++;
	}
	else if (input[index_pointer] >= '1' && input[index_pointer] <= '9')
	{ // why doesn't '0' enter this condition? => the first 0 refers to octal numeral, that isn't professor's request.
		previous_operation_element = 2;
		buffer[buffer_SF] = input[index_pointer] - 48;
		buffer_SF++;
	}
	else
	{
		switch (input[index_pointer])
		{ // set of single characters
		case '0':
			previous_operation_element = 1;
			numcpy(operands[operand_pointer], buffer);
			operands[operand_pointer][0] = 0;
			operands_SF[operand_pointer] = 1;
			operand_pointer++;
			break;
		case 'H':
		case 'h':
			if (index_pointer != 99 && input[index_pointer + 1] >= '1' && input[index_pointer + 1] <= '3')
			{ // SCE prevents core dump.
				previous_operation_element = 1;
				index_pointer++;
				numcpy(operands[operand_pointer], historys[input[index_pointer] - '1']);
				operands_SF[operand_pointer] = historys_SF[input[index_pointer] - '1'];
				operands_sign[operand_pointer] = historys_sign[input[index_pointer] - '1'];
				operand_pointer++;
			}
			else
				return -1;
			break;
		// case '+':
		case '-': // it must be unary operator. unary operator does not enter this below array. just calculate this time.
			previous_operation_element = 0;
			operands_sign[0] = false;
			valid_operator_before_unary = false;
			break;
		default:
			return -1;
		}
	}
	for (index_pointer++; index_pointer < 100 && input[index_pointer] != 0; index_pointer++)
	{ // short circuit evaluate prevents core dump : out of index
		for (; index_pointer < 100; index_pointer++)
		{
			if (input[index_pointer] == ' ')
			{
				if (previous_operation_element == 2)
				{
					previous_operation_element = 1;
					// flush

					numcpy(operands[operand_pointer], buffer);
					operands_SF[operand_pointer] = buffer_SF;
					// the sign cannot be distinguished by only number not including others.

					// buffer initialize
					numcpy(buffer, init);
					buffer_SF = 0;

					operand_pointer++;
				}
			}
			else
			{
				break;
			}
		}
		if (index_pointer == 100 || input[index_pointer] == 0)
			break;
		if (input[index_pointer] >= 'a' && input[index_pointer] <= 'e')
		{
			if (previous_operation_element)
				return -1;
			previous_operation_element = 1;
			numcpy(operands[operand_pointer], vars[input[index_pointer] - 'a']);
			operands_SF[operand_pointer] = vars_SF[input[index_pointer] - 'a'];
			operands_sign[operand_pointer] = (vars_sign[input[index_pointer] - 'a'] == operands_sign[operand_pointer]) ? true : false;
			operand_pointer++;
		}
		else if (input[index_pointer] >= 'A' && input[index_pointer] <= 'E')
		{
			if (previous_operation_element)
				return -1;
			previous_operation_element = 1;
			numcpy(operands[operand_pointer], vars[input[index_pointer] - 'A']);
			operands_SF[operand_pointer] = vars_SF[input[index_pointer] - 'A'];
			operands_sign[operand_pointer] = (vars_sign[input[index_pointer] - 'A'] == operands_sign[operand_pointer]) ? true : false;
			operand_pointer++;
		}
		else if (input[index_pointer] >= '0' && input[index_pointer] <= '9')
		{
			if (previous_operation_element == 1 || previous_operation_element == 3 || buffer_SF > MAX_LENGTH) // if previous op element is variable or number : ex) 100 1 or SF exceeds 30
				return -1;
			if (previous_operation_element != 2 && input[index_pointer] == '0')
			{ // same as above. ( professor's request. )
				previous_operation_element = 1;
				numcpy(operands[operand_pointer], buffer);
				operands[operand_pointer][0] = 0;
				operands_SF[operand_pointer] = 1;
				operand_pointer++;
			}
			else
			{
				previous_operation_element = 2;
				buffer[buffer_SF] = input[index_pointer] - '0';
				buffer_SF++;
			}
		}
		else
		{
			switch (input[index_pointer])
			{
			case 'H':
			case 'h':
				if (index_pointer != 99 && input[index_pointer + 1] >= '1' && input[index_pointer + 1] <= '3')
				{ // SCE prevents core dump.
					previous_operation_element = 1;
					index_pointer++;
					numcpy(operands[operand_pointer], historys[input[index_pointer] - '1']);
					operands_SF[operand_pointer] = historys_SF[input[index_pointer] - '1'];
					operands_sign[operand_pointer] = (historys_sign[input[index_pointer] - '1'] == operands_sign[operand_pointer]) ? true : false;
					operand_pointer++;
				}
				else
					return -1;
				break;
			case '+':
			case '-':
				if (previous_operation_element == 2)
				{
					// flush
					numcpy(operands[operand_pointer], buffer);
					operands_SF[operand_pointer] = buffer_SF;
					// the sign cannot be distinguished by only number not including others.

					// buffer initialize
					numcpy(buffer, init);
					buffer_SF = 0;

					operand_pointer++;
				}
				else if (previous_operation_element == 0)
				{
					if (input[index_pointer] == '-' && valid_operator_before_unary)
					{
						operands_sign[operand_pointer] = false;
						valid_operator_before_unary = false;
						break;
					}
					else
						return -1;
				}
				if (index_pointer != 99 && input[index_pointer + 1] == '-') // -- operator
					return -1;
				previous_operation_element = 0;
				operators[operator_pointer] = input[index_pointer];

				operator_pointer++;
				valid_operator_before_unary = true;
				break;
			case '*':
			case '/':
			case '%':
				switch (previous_operation_element)
				{
				case 0:
					return -1;
				case 2:
					// flush
					numcpy(operands[operand_pointer], buffer);
					operands_SF[operand_pointer] = buffer_SF;
					// the sign cannot be distinguished by only number not including others.

					// buffer initialize
					numcpy(buffer, init);
					buffer_SF = 0;

					operand_pointer++;
				default:
					previous_operation_element = 0;
					operators[operator_pointer] = input[index_pointer];
					operator_pointer++;
					valid_operator_before_unary = true;
				}
				break;
			default:
				return -1;
			}
		}
	}
	if (previous_operation_element == 2)
	{
		// flush
		numcpy(operands[operand_pointer], buffer);
		operands_SF[operand_pointer] = buffer_SF;
		// the sign cannot be distinguished by only number not including others.

		operand_pointer++;
	}
	else if (previous_operation_element == 0) // expression cannot end with operator.
		return -1;

	for (i = 0; i < 50; i++) // address initialize
		address[i] = i;

	// calculate multiple, divide, get_remainder

	for (operation_pointer = 0; operation_pointer < operator_pointer; operation_pointer++)
	{
		switch (operators[operation_pointer])
		{
		case '*':
			address[operation_pointer] = operation_pointer + 1;
			switch (multiple(operands[operation_pointer], operands_SF[operation_pointer], operands_sign[operation_pointer], operands[address[operation_pointer]], operands_SF[address[operation_pointer]], operands_sign[address[operation_pointer]], temp))
			{
			case -1:
				return -1;
			case 0:
				operands_sign[operation_pointer + 1] = false;
				break;
			case 1:
				operands_sign[operation_pointer + 1] = true;
				break;
			}
			numcpy(operands[operation_pointer + 1], temp);
			operands_SF[operation_pointer + 1] = get_SF(temp);
			break;
		case '/':
			address[operation_pointer] = operation_pointer + 1;
			switch (divide(operands[operation_pointer], operands_SF[operation_pointer], operands_sign[operation_pointer], operands[address[operation_pointer]], operands_SF[address[operation_pointer]], operands_sign[address[operation_pointer]], temp))
			{
			case -1:
				return -1;
			case 0:
				operands_sign[operation_pointer + 1] = false;
				break;
			case 1:
				operands_sign[operation_pointer + 1] = true;
				break;
			}
			numcpy(operands[operation_pointer + 1], temp);
			operands_SF[operation_pointer + 1] = get_SF(temp);
			break;
		case '%':
			address[operation_pointer] = operation_pointer + 1;
			switch (get_remainder(operands[operation_pointer], operands_SF[operation_pointer], operands_sign[operation_pointer], operands[address[operation_pointer]], operands_SF[address[operation_pointer]], operands_sign[address[operation_pointer]], temp))
			{
			case -1:
				return -1;
			case 0:
				operands_sign[operation_pointer + 1] = false;
				break;
			case 1:
				operands_sign[operation_pointer + 1] = true;
				break;
			}
			numcpy(operands[operation_pointer + 1], temp);
			operands_SF[operation_pointer + 1] = get_SF(temp);
			break;
		}
	}

	// calculate plus, and minus. / if address[i] == i,

	for (operation_pointer = 0; operation_pointer < operator_pointer; operation_pointer++)
	{
		if (operators[operation_pointer] == '+' || operators[operation_pointer] == '-')
		{
			for (address_pointer = operation_pointer + 1; address_pointer < operator_pointer && address[address_pointer] != address_pointer; address_pointer++) // short circuit evaluate prevents core dump : out of index
				;
			if (operators[operation_pointer] == '-')
			{
				operands_sign[address_pointer] = operands_sign[address_pointer] ? false : true;
			}
			switch (sum(operands[operation_pointer], operands_SF[operation_pointer], operands_sign[operation_pointer], operands[address_pointer], operands_SF[address_pointer], operands_sign[address_pointer], temp))
			{
			case -1:
				return -1;
			case 0:
				operands_sign[address_pointer] = false;
				break;
			case 1:
				operands_sign[address_pointer] = true;
				break;
			}
			numcpy(operands[address_pointer], temp);
			operands_SF[address_pointer] = get_SF(temp);
		}
	}

	numcpy(result, operands[operand_pointer - 1]);
	return operands_sign[operand_pointer - 1];
}

void convert_bit_order(char array[], int SF)
{
	char temp;
	int i, N;
	N = SF / 2;
	for (i = 0; i < N; i++)
	{
		temp = array[i];
		array[i] = array[SF - i - 1];
		array[SF - i - 1] = temp;
	}
}

int sum(char var1[], int var1_SF, bool var1_sign, char var2[], int var2_SF, bool var2_sign, char result[])
{
	int i;
	bool result_sign;
	int result_SF;

	for (i = 0; i < MAX_LENGTH; i++)
	{ // initialize result as 0
		result[i] = 0;
	}
	for (i = var1_SF; i < MAX_LENGTH; i++)
	{
		var1[i] = 0;
	}
	for (i = var2_SF; i < MAX_LENGTH; i++)
	{
		var2[i] = 0;
	}

	if (var1_sign == var2_sign)
	{							 // plus same sign
		result_sign = var1_sign; // sign of the result follow both operands.
	}
	else
	{ // else
		if (var1_SF > var2_SF)
		{ // is var1 bigger than var2?
			result_sign = var1_sign;
		}
		else if (var1_SF < var2_SF)
		{ // is var2 bigger than var1?
			result_sign = var2_sign;
		}
		else
		{ // SF is same. so we have to compare more.
			for (i = 0; i < var1_SF; i++)
			{
				if (var1[i] > var2[i])
				{ // is var1 bigger than var2?
					result_sign = var1_sign;
					break;
				}
				else if (var1[i] < var2[i])
				{ // is var2 bigger than var1?
					result_sign = var2_sign;
					break;
				} // else : var1[i] == var[2] so repeat the block.
			}
			if (i == -1)
			{ // var1 + var2 == 0
				return 1;
			}
		}
	}
	convert_bit_order(var1, var1_SF);
	convert_bit_order(var2, var2_SF);
	sign_processing(var1, var1_SF, var1_sign);
	sign_processing(var2, var2_SF, var2_sign);

	if (result_sign)
	{
		for (i = 0; i < 29; i++)
		{ // i recycling
			result[i] += var1[i] + var2[i];
			result[i + 1] += result[i] / 10;
			result[i] %= 10;
			if (result[i] < 0)
			{
				result[i] += 10;
				result[i + 1]--;
			}
		}
		result[29] += var1[29] + var2[29];
		if (result[29] > 9)
			return -1;
	}
	else
	{
		for (i = 0; i < 29; i++)
		{ // i recycling
			result[i] += var1[i] + var2[i];
			result[i + 1] += result[i] / 10;
			result[i] %= 10;
			if (result[i] > 0)
			{
				result[i] -= 10;
				result[i + 1]++;
			}
		}
		result[29] += var1[29] + var2[29];
		if (result[29] > 9)
			return -1;
	}

	for (i = var1_SF; i < MAX_LENGTH; i++)
	{
		var1[i] = -48;
	}
	for (i = var2_SF; i < MAX_LENGTH; i++)
	{
		var2[i] = -48;
	}

	for (i = 29; i > -1; i--)
	{
		if (result[i] != 0)
			break;
		else
			result[i] = -48;
	}
	if (i == -1)
	{ // if result is 0
		i++;
		result[i] = 0;
	}
	result_SF = i + 1;

	convert_bit_order(var1, var1_SF);
	convert_bit_order(var2, var2_SF);
	convert_bit_order(result, result_SF);

	sign_processing(var1, var1_SF, var1_sign);
	sign_processing(var2, var2_SF, var2_sign);
	sign_processing(result, result_SF, result_sign);

	return result_sign;
}
int multiple(char var1[], int var1_SF, bool var1_sign, char var2[], int var2_SF, bool var2_sign, char result[])
{
	int i, j;
	int result_prevent_overflow[MAX_LENGTH];
	bool result_sign;
	int result_SF;
	if (var1_SF + var2_SF > 31)
		return -1;
	for (i = 0; i < MAX_LENGTH; i++)
	{ // initialize result as 0
		result_prevent_overflow[i] = 0;
	}
	for (i = var1_SF; i < MAX_LENGTH; i++)
	{
		var1[i] = 0;
	}
	for (i = var2_SF; i < MAX_LENGTH; i++)
	{
		var2[i] = 0;
	}

	convert_bit_order(var1, var1_SF);
	convert_bit_order(var2, var2_SF);
	if (var1_sign == var2_sign)
	{ // When things with the same sign are multiplied, the sign of the result is +.
		result_sign = true;
	}
	else
	{ // else
		result_sign = false;
	}

	for (i = 0; i < var1_SF; i++)
	{
		for (j = 0; j < var2_SF; j++)
		{
			result_prevent_overflow[i + j] += var1[i] * var2[j];
		}
	}
	i = 0;
	for (i = 0; i < MAX_LENGTH; i++)
	{
		result_prevent_overflow[i + 1] += result_prevent_overflow[i] / 10;
		result_prevent_overflow[i] %= 10;
	}
	if (result_prevent_overflow[29] > 9)
		return -1;
	for (i = var1_SF; i < MAX_LENGTH; i++)
	{
		var1[i] = -48;
	}
	for (i = var2_SF; i < MAX_LENGTH; i++)
	{
		var2[i] = -48;
	}

	for (i = 29; i > -1; i--)
	{
		if (result_prevent_overflow[i] != 0)
			break;
		else
			result_prevent_overflow[i] = -48;
	}
	if (i == -1)
	{
		i++;
		result_prevent_overflow[i] = 0;
	}
	result_SF = i + 1;
	for (i = 0; i < MAX_LENGTH; i++)
	{
		result[i] = result_prevent_overflow[i];
	}

	convert_bit_order(var1, var1_SF);
	convert_bit_order(var2, var2_SF);
	convert_bit_order(result, result_SF);
	return result_sign;
}

void sign_processing(char var[], int SF, bool sign)
{
	int i;
	if (!sign)
	{
		for (i = 0; i < SF; i++)
		{
			var[i] *= -1;
		}
	}
	return;
}

int divide(char var1[], int var1_SF, bool var1_sign, char var2[], int var2_SF, bool var2_sign, char result[])
{
	int i, j;

	char temp[30] = {2, 0};
	int temp_SF = 1;
	bool temp_sign = true;

	bool result_sign;

	char MOD[9][30];		// multiples of divisor
	int MOD_next_digit = 0; // the index of MOD exceeding digit of the smallest divisor.
	int place;
	int goal = var1_SF - var2_SF + 1;
	bool next_digit = false;
	bool big; // Is MOD bigger than the part of dividend?

	char dividend[30];

	result[0] = 0;
	for (i = 1; i < 30; i++)
	{
		result[i] = -48;
	}

	numcpy(dividend, var1);

	if (var1_sign == var2_sign)
	{ // When things with the same sign are multiplied, the sign of the result is +.
		result_sign = true;
	}
	else
	{ // else
		result_sign = false;
	}

	if (var1_SF < var2_SF)
		return 1;					  // It doesn't matter.
	if (var2_SF == 1 && var2[0] == 0) // Is it tried to divide by 0?
		return -1;					  // error

	// get divisors.

	numcpy(MOD[0], var2);

	for (i = 1; i < 9; i++)
	{
		if (multiple(var2, var2_SF, var2_sign, temp, temp_SF, temp_sign, MOD[i]) == -1)
		{
			MOD_next_digit = i;
			break;
		}
		else if (get_SF(MOD[i]) != var2_SF && (!MOD_next_digit))
		{
			MOD_next_digit = i;
		}
		temp[0]++;
	}
	if (!MOD_next_digit)
		MOD_next_digit = 9;
	for (place = 0; place != goal; place++)
	{
		big = false;
		if (next_digit)
		{
			for (i = MOD_next_digit; i < 9; i++)
			{
				for (j = 0; j < var2_SF + 1; j++)
				{
					if (MOD[i][j] > dividend[place + j - 1])
					{
						big = true;
						break;
					}
					else if (MOD[i][j] < dividend[place + j - 1])
						break;
				}
				if (j == var2_SF && MOD[i][j] == dividend[place + j - 1])
				{ // short circuit evaluate prevents segmentation fault.
					big = false;
					i++;
					break;
				}
				else if (big)
				{
					break;
				}
			}
			if (i == MOD_next_digit)
			{ // if dividend is bigger than number of next digit or dividend is bigger than the biggest divisor?
				i--;
				for (j = var2_SF - 1; j > -1; j--)
				{
					dividend[place + j] -= MOD[i][j];
					if (dividend[place + j] < 0)
					{
						dividend[place + j - 1]--;
						dividend[place + j] += 10;
					}
				}
				result[place] = i + 1;
			}
			else
			{
				i--;
				for (j = var2_SF; j > -1; j--)
				{
					dividend[place + j - 1] -= MOD[i][j];
					if (dividend[place + j - 1] < 0)
					{
						dividend[place + j - 2]--;
						dividend[place + j - 1] += 10;
					}
				}
				result[place] = i + 1;
			}

			if (dividend[place] != 0)
				next_digit = true;
			else
				next_digit = false;
		}
		else
		{
			for (i = 0; i < MOD_next_digit; i++)
			{
				for (j = 0; j < var2_SF; j++)
				{
					if (MOD[i][j] > dividend[place + j])
					{
						big = true;
						break;
					}
					else if (MOD[i][j] < dividend[place + j])
						break;
				}
				if (j == var2_SF && MOD[i][j - 1] == dividend[place + j - 1])
				{ // short circuit evaluate prevents segmentation fault.
					i++;
					break;
				}
				else if (big)
					break;
			}

			if (i != 0)
			{
				i--;
				for (j = var2_SF - 1; j > -1; j--)
				{
					dividend[place + j] -= MOD[i][j];
					if (dividend[place + j] < 0)
					{
						dividend[place + j - 1]--;
						dividend[place + j] += 10;
					}
				}
				result[place] = i + 1;
			}
			else
				result[place] = 0;

			if (dividend[place] != 0)
				next_digit = true;
			else
				next_digit = false;
		}
	}

	if (result[0] == 0 && var1_SF != var2_SF)
	{
		for (i = 0; i < 29; i++)
			result[i] = result[i + 1];
	}
	return result_sign;
}

int get_remainder(char var1[], int var1_SF, bool var1_sign, char var2[], int var2_SF, bool var2_sign, char result[])
{
	int i, j;

	char temp[30] = {2, 0};
	int temp_SF = 1;
	bool temp_sign = true;

	bool result_sign;

	char MOD[9][30];		// multiples of divisor
	int MOD_next_digit = 0; // the index of MOD exceeding digit of the smallest divisor.
	int place;
	int goal = var1_SF - var2_SF + 1;
	bool next_digit = false;
	bool big; // Is MOD bigger than the part of dividend?

	numcpy(result, var1);

	result_sign = var1_sign;

	if (var1_SF < var2_SF)
		return result_sign;			  // It doesn't matter.
	if (var2_SF == 1 && var2[0] == 0) // Is it tried to divide by 0?
		return -1;					  // error

	// get divisors.

	numcpy(MOD[0], var2);

	for (i = 1; i < 9; i++)
	{
		if (multiple(var2, var2_SF, var2_sign, temp, temp_SF, temp_sign, MOD[i]) == -1)
		{
			MOD_next_digit = i;
			break;
		}
		else if (get_SF(MOD[i]) != var2_SF && (!MOD_next_digit))
		{
			MOD_next_digit = i;
		}
		temp[0]++;
	}
	if (!MOD_next_digit)
		MOD_next_digit = 9;
	for (place = 0; place != goal; place++)
	{
		big = false;
		if (next_digit)
		{
			for (i = MOD_next_digit; i < 9; i++)
			{
				for (j = 0; j < var2_SF + 1; j++)
				{
					if (MOD[i][j] > result[place + j - 1])
					{
						big = true;
						break;
					}
					else if (MOD[i][j] < result[place + j - 1])
						break;
				}
				if (j == var2_SF && MOD[i][j] == result[place + j - 1])
				{ // short circuit evaluate prevents segmentation fault.
					big = false;
					i++;
					break;
				}
				else if (big)
				{
					break;
				}
			}
			if (i == MOD_next_digit)
			{ // if dividend is bigger than number of next digit or dividend is bigger than the biggest divisor?
				i--;
				for (j = var2_SF - 1; j > -1; j--)
				{
					result[place + j] -= MOD[i][j];
					if (result[place + j] < 0)
					{
						result[place + j - 1]--;
						result[place + j] += 10;
					}
				}
			}
			else
			{
				i--;
				for (j = var2_SF; j > -1; j--)
				{
					result[place + j - 1] -= MOD[i][j];
					if (result[place + j - 1] < 0)
					{
						result[place + j - 2]--;
						result[place + j - 1] += 10;
					}
				}
			}

			if (result[place] != 0)
				next_digit = true;
			else
				next_digit = false;
		}
		else
		{
			for (i = 0; i < MOD_next_digit; i++)
			{
				for (j = 0; j < var2_SF; j++)
				{
					if (MOD[i][j] > result[place + j])
					{
						big = true;
						break;
					}
					else if (MOD[i][j] < result[place + j])
						break;
				}
				if (j == var2_SF - 1 && MOD[i][j] == result[place + j])
				{ // short circuit evaluate prevents segmentation fault.
					i++;
					break;
				}
				else if (big)
					break;
			}
			if (i != 0)
			{
				i--;
				for (j = var2_SF - 1; j > -1; j--)
				{
					result[place + j] -= MOD[i][j];
					if (result[place + j] < 0)
					{
						result[place + j - 1]--;
						result[place + j] += 10;
					}
				}
			}

			if (result[place] != 0)
				next_digit = true;
			else
				next_digit = false;
		}
	}

	for (i = 0; i < var1_SF; i++)
	{
		if (result[i] != 0)
			break;
	}
	if (i != var1_SF)
	{
		for (j = i; j < var1_SF; j++)
		{
			result[j - i] = result[j];
		}
		for (j = var1_SF - i; j < var1_SF; j++)
		{
			result[j] = -48;
		}
	}
	else
	{ // if the result is 0
		for (i = 1; i < var1_SF; i++)
			result[i] = -48;
	}

	return result_sign;
}

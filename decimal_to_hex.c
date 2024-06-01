#include <stdbool.h>

// declaration
int decimal2hex(char[], char[]);

// definition
int decimal2hex(char input[], char result[]) {
	static char number[31];
	int i;
	int operand_integer;
	float operand_real;
	bool type; // true : int | false : float

	for ( i = 0; i < 100 && input[i] == ' '; i++ )
		;
	i++; // it must be the operator - '#'.
	for ( ; i < 100 && input[i]; i++ )

	return 1;
}

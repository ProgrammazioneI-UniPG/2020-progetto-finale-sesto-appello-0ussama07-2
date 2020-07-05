#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"

void init_rand()
{
	time_t t;
	srand((unsigned)time(&t));
}

int within(int value, int min, int max)
{
	return value >= min && value <= max;
}

void clear_input()
{
	while ((_ = getchar()) != '\n' && _ != EOF);
}

void clear_output()
{
	system("cls");
}

void press_any_key(const char* action)
{
	printf("\n Press any [KEY] to %s", action);
	clear_input();
	_ = getchar();
}

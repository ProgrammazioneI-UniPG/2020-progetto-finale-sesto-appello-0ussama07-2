#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "game.h"

int main()
{
	init_rand();

	int invalid = 0;

	do
	{
		invalid = 0;

		clear_output();
		printf(" ============ Arvais vs Hartornen ============\n");
		printf(" [1] Create Tunnels\n");
		printf(" [2] Play\n");
		printf(" [3] End Game\n");
		printf(" =============================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			create_tunnels();
			break;
		case 2:
			play();
			break;
		case 3:
			end_game();
			return 0;
		default:
			invalid = 1;
		}
	} while (1);
}

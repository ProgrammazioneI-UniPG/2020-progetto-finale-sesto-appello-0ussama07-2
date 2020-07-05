#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"
#include "game.h"

// Config
static int min_tunnel_length = 1; // 1 for testing | 10 for playing
static int energy_tank_size = 4;
static int collection_tank_size = 10;

static Cavern_chances default_chances = {
	50, 30, 20,
	50, 35, 15
};

// Players

static Player arvais = { NULL, { 40, 0, 20, 40, 50, 30, 20, 50, 35, 15 }, { NULL, 4, 0}, 0, 0 };
static Player hartornen = { NULL, { 40, 0, 20, 40, 50, 30, 20, 50, 35, 15 }, { NULL, 4, 0}, 0, 0 };

// Trackers

static Cavern* ultimate_cavern = NULL;
static Cavern* penultimate_cavern = NULL;

static int number_of_caverns = 0;
static int finished_creation = 0;

static int turn = 0;
static int solo_play = 0;
static int game_over = 0;

// Prototypes

static int keep_tunnel();
static void create_tunnel(Player*);
static void insert_cavern(Player*);
static void delete_cavern(Player*);
static void render_tunnel(Cavern*);
static void finish_tunnel(Player*);

static void advance(Player*);
static void knock_down(Player*);
static void exit_tunnel(Player*);
static void update_chances(Player* player);

static void go_around(Excavator*);
static void move_to_next(Excavator*);

static void increase_energy(Excavator*, int);
static void decrease_energy(Excavator*, int);
static void collect_molasses(Excavator*, int);
static void empty_tanks(Excavator*);

static Cavern* create_cavern(Cavern_chances);
static Cavern* get_next_cavern(Cavern*);
static Cavern_direction get_next_direction(Cavern*);
static Cavern_direction get_empty_direction(Cavern*);
static void set_next_cavern(Cavern*, Cavern*, Cavern_direction);
static void delete_tunnel(Player*);

static Cavern* get_next_cavern(Cavern*);
static Unexpected_type get_unexpected(Cavern_chances);
static Molasses_quantity get_molasses(Cavern_chances);

static void free_memory();

// Create Tunnels

void create_tunnels()
{
	int invalid = 0;

	do
	{
		invalid = 0;

		clear_output();
		printf(" ============ PLAYER ============\n");
		printf(" [1] Avaris\n");
		printf(" [2] Hartornen\n");
		printf(" [0] Go Back\n");
		printf(" ================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			create_tunnel(&arvais);
			break;
		case 2:
			create_tunnel(&hartornen);
			break;
		case 0:
			return;
		default:
			invalid = 1;
		}
	} while (1);
}

static int keep_tunnel()
{
	int invalid = 0;

	do
	{
		invalid = 0;

		clear_output();
		printf(" A tunnel already exists. Would you like to keep it?\n\n");
		printf(" [1] Yes \n");
		printf(" [2] No \n\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			return 1;
		case 2:
			return 0;
		default:
			invalid = 1;
		}
	} while (invalid == 1);

	return 1;
}

static void create_tunnel(Player* player)
{
	if (player->tunnel != NULL)
	{
		if (keep_tunnel())
		{
			return;
		}
		else
		{
			delete_tunnel(player);
		}
	}

	ultimate_cavern = NULL;
	penultimate_cavern = NULL;

	number_of_caverns = 0;
	finished_creation = 0;

	int invalid;

	do
	{
		invalid = 0;

		clear_output();
		printf(" ============ CREATE TUNNEL ============\n");
		printf(" [1] Insert Cavern\n");
		printf(" [2] Delete Cavern\n");
		printf(" [3] Render Tunnel\n");
		printf(" [4] Finish Tunnel\n");
		printf(" =======================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			insert_cavern(player);
			break;
		case 2:
			delete_cavern(player);
			break;
		case 3:
			render_tunnel(player->tunnel);
			break;
		case 4:
			finish_tunnel(player);
			break;
		default:
			invalid = 1;
		}
	} while (finished_creation != 1);
}

static void insert_cavern(Player* player)
{
	Cavern* new_cavern = create_cavern(default_chances);

	int invalid;

	do
	{
		invalid = 0;

		clear_output();
		printf(" ============ STATE ============\n");
		printf(" [1] Normal\n");
		printf(" [2] Special\n");
		printf(" [3] Bumpy\n");
		printf(" [0] Cancel\n");
		printf(" ===================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			new_cavern->state = normal_cavern;
			break;
		case 2:
			new_cavern->state = special_cavern;
			break;
		case 3:
			new_cavern->state = bumpy_cavern;
			break;
		case 0:
			free(new_cavern);
			return;
		default:
			invalid = 1;
			break;
		}
	} while (invalid == 1);

	if (ultimate_cavern != NULL) do
	{
		invalid = 0;

		clear_output();
		printf(" ============ DIRECTION ============\n");
		printf(" [1] Forward\n");
		printf(" [2] Left\n");
		printf(" [3] Right\n");
		printf(" [0] Cancel\n");
		printf(" ===================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			ultimate_cavern->forward = new_cavern;
			break;
		case 2:
			ultimate_cavern->left = new_cavern;
			break;
		case 3:
			ultimate_cavern->right = new_cavern;
			break;
		case 0:
			free(new_cavern);
			return;
		default:
			invalid = 1;
			break;
		}
	} while (invalid);

	if (number_of_caverns > 0)
	{
		penultimate_cavern = ultimate_cavern;
	}
	else
	{
		player->tunnel = new_cavern;
	}

	ultimate_cavern = new_cavern;
	
	number_of_caverns++;

	clear_output();
	printf(" New cavern inserted\n");
	press_any_key("Go Back");
}

static void delete_cavern(Player* player)
{
	if (player->tunnel == NULL)
	{
		clear_output();
		printf(" There is no cavern to be deleted\n");
		press_any_key("Go Back");

		return;
	}

	free(ultimate_cavern);

	if (number_of_caverns > 1)
	{
		penultimate_cavern->forward = NULL;
		penultimate_cavern->left = NULL;
		penultimate_cavern->right= NULL;

		ultimate_cavern = penultimate_cavern;
	}
	else
	{
		player->tunnel = NULL;

		ultimate_cavern = NULL;
	}

	number_of_caverns--;

	clear_output();
	printf(" The last cavern was deleted\n");
	press_any_key("Go Back");
}

static void render_tunnel(Cavern* tunnel)
{
	if (tunnel == NULL)
	{
		clear_output();
		printf(" There is not tunnel to be rendered\n");
		press_any_key("Go Back");

		return;
	}

	clear_output();
	printf("  --------- ------------ ---------- ---------\n");
	printf(" | state   | unexpected | molasses | next    |\n");
	printf("  --------- ------------ ---------- ---------\n");

	Cavern* current = tunnel;

	while (current != NULL)
	{
		switch (current->state)
		{
		case normal_cavern:
			printf(" | Normal  ");
			break;
		case special_cavern:
			printf(" | Special ");
			break;
		case bumpy_cavern:
			printf(" | Bumpy   ");
			break;
		default:
			printf(" | Unknown ");
			break;
		}

		switch (current->unexprected)
		{
		case no_unexpected:
			printf("| None       ");
			break;
		case unexpected_collapse:
			printf("| Collapse   ");
			break;
		case unexpected_worm:
			printf("| Worm       ");
			break;
		default:
			printf("| Unknown    ");
			break;
		}

		switch (current->molasses)
		{
		case no_molasses:
			printf("| Zero     ");
			break;
		case few_molasses:
			printf("| Few      ");
			break;
		case many_molasses:
			printf("| Many     ");
			break;
		default:
			printf("| Unknown  ");
			break;
		}

		if (current->forward != NULL)
		{
			printf("| Forward |\n");
			current = current->forward;
		}
		else if (current->left != NULL)
		{
			printf("| Left    |\n");
			current = current->left;
		}
		else if (current->right != NULL)
		{
			printf("| Right   |\n");
			current = current->right;
		}
		else
		{
			printf("| None    |\n");
			current = NULL;
		}
	}

	printf("  --------- ------------ ---------- ---------\n");
	press_any_key("Go Back");
}

static void finish_tunnel(Player* player)
{
	if (number_of_caverns >= min_tunnel_length)
	{
		player->excavator.position = player->tunnel;
		finished_creation = 1;

		clear_output();
		printf(" Tunnel creation finished\n");
		press_any_key("Go Back");
	}
	else
	{
		clear_output();
		printf(" %d more caverns required to finish the tunnel\n", min_tunnel_length - number_of_caverns);
		press_any_key("Go Back");
	}
}

// Play

void play()
{
	arvais.excavator.position = arvais.tunnel;
	hartornen.excavator.position = hartornen.tunnel;

	turn = 1;
	Player* player = &arvais;

	int invalid = 0;

	do
	{
		invalid = 0;

		clear_output();

		switch (turn)
		{
		case 1:
			printf(" ============ ARVAIS ============\n");
			break;
		case 2:
			printf(" =========== HARTORNEN ==========\n");
			break;
		}

		
		if (get_next_cavern(player->excavator.position) != NULL)
		{
			printf(" [1] Advance\n");
		}

		printf(" [2] Knock Down\n");

		if (player->excavator.position->state == exit_cavern)
		{
			printf(" [3] Exit\n");
		}

		printf(" ================================\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			if (get_next_cavern(player->excavator.position) != NULL)
			{
				advance(player);
			}
			else
			{
				invalid = 1;
			}
			break;
		case 2:
			knock_down(player);
			break;
		case 3:
			if (player->excavator.position->state == exit_cavern)
			{
				exit_tunnel(player);
			}
			else
			{
				invalid = 1;
			}
			break;
		default:
			invalid = 1;
			break;
		}

		if (invalid != 1)
		{
			player->total_turns++;

			if (player->excavator.energy_tank < 0)
			{
				game_over = 1;
			}

			if (solo_play != 1)
			{
				switch (turn)
				{
				case 1:
					update_chances(&arvais);
					turn = 2;
					player = &hartornen;
					break;
				case 2:
					update_chances(&hartornen);
					turn = 1;
					player = &arvais;
					break;
				}
			}
		}
	} while (game_over == 0);

	clear_output();
	printf(" Game Over\n\n");

	if (arvais.finished == 0 || arvais.excavator.energy_tank < 0)
	{
		printf(" Hartornen Wins\n");
	}
	else if (hartornen.finished == 0 || hartornen.excavator.energy_tank < 0)
	{
		printf(" Arvais Wins\n");
	}
	else if (arvais.excavator.collection_tank < hartornen.excavator.collection_tank)
	{
		printf(" Hartornen Wins\n");
	}
	else if (arvais.excavator.collection_tank > hartornen.excavator.collection_tank)
	{
		printf(" Hartornen Wins\n");
	}
	else
	{
		printf(" Fight");
	}

	press_any_key("Go Back");
}

static void advance(Player* player)
{
	int percentage = rand() % 100 + 1;

	if (within(percentage, 1, 25))
	{
		go_around(&player->excavator);

		return;
	}

	move_to_next(&player->excavator);
}

static void knock_down(Player* player)
{
	// delete current tunnel before digging a new one
	Cavern* current = get_next_cavern(player->excavator.position);
	Cavern* free_me = NULL;

	while (current != NULL)
	{
		free_me = current;
		current = get_next_cavern(current);
		free(free_me);
	}

	Cavern* new_cavern = create_cavern(player->chances);
	set_next_cavern(player->excavator.position, new_cavern, get_empty_direction(player->excavator.position));

	advance(player);
	decrease_energy(&player->excavator, 1);
}

static void exit_tunnel(Player* player)
{
	player->excavator.position = NULL;
	player->finished = 1;
	solo_play = 1;
}

static void update_chances(Player* player)
{

}

static void go_around(Excavator* excavator)
{
	Cavern* next_cavern = get_next_cavern(excavator->position);
	Cavern* new_cavern = create_cavern(default_chances);

	//              new_cavern ____
 	//                             \
	//  position -> next_cavern -> next_cavern->forward | left | right

	set_next_cavern(new_cavern, get_next_cavern(next_cavern), get_next_direction(next_cavern));

	//           ___ new_cavern ___
	//          /                  \
	//  position -> next_cavern -> next_cavern->forward | left | right
	
	set_next_cavern(excavator->position, new_cavern, get_empty_direction(excavator->position));
	
	free(next_cavern);
	move_to_next(excavator);
}

static void move_to_next(Excavator* excavator)
{
	Cavern* next_cavern = get_next_cavern(excavator->position);
	excavator->position = next_cavern;

	switch (next_cavern->state)
	{
	case special_cavern:
		increase_energy(excavator, 1);
		break;
	case bumpy_cavern:
		decrease_energy(excavator, 1);
		break;
	}

	switch (next_cavern->unexprected)
	{
	case unexpected_collapse:
		decrease_energy(excavator, 1);
		break;
	case unexpected_worm:
		empty_tanks(excavator);
		return;
	}
	
	if (next_cavern->molasses == no_molasses)
	{
		return;
	}

	int quantity = 0;

	switch (next_cavern->molasses)
	{
	case few_molasses:
		quantity = 1;
	case many_molasses:
		quantity = 3;
		break;
	}

	int invalid;

	do
	{
		invalid = 0;
		
		clear_output();
		printf(" %d molasses found. Where do you want to put them?\n\n", quantity);
		printf(" [1] Enery Tank \n");
		printf(" [2] Collection Tank \n\n");

		if (invalid == 1)
		{
			printf(" Invalid command. Try again!\n\n");
		}

		int command = 0;

		printf(" >>> ");
		_ = scanf("%d", &command);

		switch (command)
		{
		case 1:
			increase_energy(excavator, quantity);
			break;
		case 2:
			collect_molasses(excavator, quantity);
			break;
		default:
			invalid = 1;
		}
	} while (invalid == 1);
}

static void increase_energy(Excavator* excavator, int count)
{
	excavator->energy_tank += count;

	if (excavator->energy_tank > energy_tank_size)
	{
		excavator->energy_tank = energy_tank_size;
	}
}

static void decrease_energy(Excavator* excavator, int count)
{
	excavator->energy_tank -= count;
}

static void collect_molasses(Excavator* excavator, int count)
{
	excavator->collection_tank += count;

	if (excavator->collection_tank > collection_tank_size)
	{
		excavator->collection_tank = collection_tank_size;
	}
}

static void empty_tanks(Excavator* excavator)
{
	excavator->energy_tank = 0;
	excavator->collection_tank = 0;
}

// End Game

void end_game()
{
	free_memory();

	clear_output();
	printf(" Game Over\n");
	press_any_key("Exit");
}

// Other

static Cavern* create_cavern(Cavern_chances chances)
{
	Cavern* new_cavern = (Cavern*)malloc(sizeof(Cavern));
	
	if (new_cavern == NULL)
	{
		free_memory();

		clear_output();
		printf(" Unable to allocate memory for a new cavern\n\n");
		press_any_key("Exit");

		exit(-1);
	}

	new_cavern->state = get_cavern_type(chances);
	new_cavern->unexprected = get_unexpected(chances);
	new_cavern->molasses = get_molasses(chances);
	new_cavern->forward = NULL;
	new_cavern->left = NULL;
	new_cavern->right = NULL;

	return new_cavern;
}

static Cavern_direction get_next_direction(Cavern* cavern)
{
	if (cavern->forward != NULL)
	{
		return forward_dircetion;
	}
	else if (cavern->left != NULL)
	{
		return left_direction;
	}
	else if (cavern->right != NULL)
	{
		return right_direction;
	}

	return no_direction;
}

static Cavern_direction get_empty_direction(Cavern* cavern)
{
	if (cavern->forward == NULL)
	{
		return forward_dircetion;
	}
	else if (cavern->left == NULL)
	{
		return left_direction;
	}
	else if (cavern->right == NULL)
	{
		return right_direction;
	}

	return no_direction;
}

static Cavern* get_next_cavern(Cavern* cavern)
{
	Cavern* next_cavern = NULL;

	if (cavern->forward != NULL)
	{
		return cavern->forward;
	}
	else if (cavern->left != NULL)
	{
		return cavern->left;
	}
	else if (cavern->right != NULL)
	{
		return cavern->right;
	}

	return NULL;
}

static void set_next_cavern(Cavern* cavern, Cavern* next_cavern, Cavern_direction direction)
{
	switch (direction)
	{
	case forward_dircetion:
		cavern->forward = next_cavern;
		break;
	case left_direction:
		cavern->left = next_cavern;
		break;
	case right_direction:
		cavern->right = next_cavern;
		break;
	}
}

static void delete_tunnel(Player* player)
{
	if (player->tunnel == NULL)
	{
		return;
	}

	Cavern* current = player->tunnel;
	Cavern* free_me = NULL;

	while (current != NULL)
	{
		free_me = current;

		current = get_next_cavern(current);

		free(free_me);
	}

	player->tunnel = NULL;
}

static Cavern_type get_cavern_type(Cavern_chances chances)
{
	int percent = rand() % 100;

	int min = 0;
	int max = min + chances.normal_cavern;

	if (within(percent, min, max) == 1)
	{
		return normal_cavern;
	}

	min = max;
	max = min + chances.special_cavern;

	if (within(percent, min, max) == 1)
	{
		return special_cavern;
	}

	min = max;
	max = min + chances.bumpy_cavern;

	if (within(percent, min, max) == 1)
	{
		return special_cavern;
	}

	return exit_cavern;
}

static Unexpected_type get_unexpected(Cavern_chances chances)
{
	int percent = rand() % 100;

	int min = 0;
	int max = min + chances.no_unexpected;

	if (within(percent, min, max) == 1)
	{
		return no_unexpected;
	}

	min = max;
	max = min + chances.unexpected_collapse;

	if (within(percent, min, max) == 1)
	{
		return unexpected_collapse;
	}
	
	return unexpected_worm;
}

static Molasses_quantity get_molasses(Cavern_chances chances)
{
	int percent = rand() % 100 + 1;

	if (within(percent, 1, 50) == 1) // 50%
	{
		return no_molasses;
	}
	else if (within(percent, 51, 80) == 1) // 30%
	{
		return few_molasses;
	}
	else // 20%
	{
		return many_molasses;
	}
}

// Clean

static void free_memory()
{
	delete_tunnel(&arvais);
	delete_tunnel(&hartornen);
}

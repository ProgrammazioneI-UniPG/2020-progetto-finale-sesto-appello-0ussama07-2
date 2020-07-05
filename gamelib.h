#ifndef GAME_HEADER
#define GAME_HEADER

typedef enum Cavern_type {
	normal_cavern = 1,
	special_cavern = 2,
	bumpy_cavern = 3,
	exit_cavern = 4
} Cavern_type;

typedef enum Cavern_direction {
	no_direction,
	forward_dircetion,
	left_direction,
	right_direction
} Cavern_direction;

typedef enum Unexpected_type {
	no_unexpected = 1,
	unexpected_collapse = 2,
	unexpected_worm = 3
} Unexpected_type;

typedef enum Molasses_quantity {
	no_molasses = 1,
	few_molasses = 2,
	many_molasses = 3
} Molasses_quantity;

typedef struct Cavern {
	Cavern_type state;
	Unexpected_type unexprected;
	Molasses_quantity molasses;
	struct Cavern* forward;
	struct Cavern* left;
	struct Cavern* right;
} Cavern;

typedef struct Cavern_chances {
	int no_unexpected;
	int unexpected_collapse;
	int unexpected_worm;
	int no_molasses;
	int few_molasses;
	int many_molasses;
} Cavern_chances;

typedef struct Excavator {
	Cavern* position;
	int energy_tank;
	int collection_tank;
} Excavator;

typedef struct Player {
	Cavern* tunnel;
	Cavern_chances chances;
	Excavator excavator;
	int total_turns;
	int finished;
} Player;

void create_tunnels();

void play();

void end_game();

#endif // !GAME_HEADER

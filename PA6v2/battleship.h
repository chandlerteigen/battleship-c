#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//the board is square, so only one dimension is defined.
#define BOARD_DIM 10

//defining the lengths of each ship for use in the placement functions
#define NUM_SHIPS 5
#define CARRIER_LEN 5
#define BATTLESHIP_LEN 4
#define CRUISER_LEN 3
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 2
#define DOWN 0
#define RIGHT 1
#define EMPTY '~'


typedef struct cell
{
	int occupied;
	char symbol;
} Cell;
typedef struct ship
{
	int length;
	int symbol;
	char name[20];
	//int is_sunk = 0;
} Ship;

typedef struct stats
{
	int hits;
	int misses;
	int shots;
	int won;
} Stats;



/**************************************************************************
* Function: board_fill
* Description: This function initializes the Cell variables inside the board array.
* Input parameters: 2D Cell array, board dimensions
* Returns: Nothing
* Preconditions: initialization of 2D Cell array.
* Postconditions: Cell variables in board are initialized.
**************************************************************************/

void board_fill(Cell board[][BOARD_DIM], int board_dim);

/**************************************************************************
* Function: board_fill
* Description: This function initializes the Ship variables in the 
* Input parameters: 2D Cell array, board dimensions
* Returns: Nothing
* Preconditions: initialization of 2D Cell array.
* Postconditions: Cell variables in board are initialized.
**************************************************************************/

void ship_list_fill(Ship ship_list[5], int num_of_ships);

void clear_screen(void);

void enter_pause(void);

void print_board(Cell board[][BOARD_DIM], int board_dim, int current_player);

void display_menu(void);

void choose_ship_placement_method(Cell player_board[][BOARD_DIM], Ship ship_list[NUM_SHIPS]);

int select_who_starts(void);

void randomly_place_ships_on_board(Cell board[][BOARD_DIM], Ship ship_list[NUM_SHIPS]);

void manually_place_ships_on_board(Cell board[][BOARD_DIM], Ship ship_list[NUM_SHIPS]);

int generate_random_num(int max_size);

int check_ship_placement(Cell board[][BOARD_DIM], int start_row, int start_col, int end_row, int end_col);

int check_if_cells_contiguous(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length);

int check_if_horizontal_or_vertical(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length);

int check_if_in_bounds(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length);

void place_ship(Cell board[][BOARD_DIM], char ship_char, int start_row, int end_row, int start_col, int end_col);


/***************************************************************************************************************************/
//turn functions and shot functions

/*
void player_turn(Cell computer_board[][BOARD_DIM], Cell player_board[][BOARD_DIM], FILE *outfile);
*/
void turn(Cell player_board[][BOARD_DIM], Cell opponent_board[][BOARD_DIM], int player, int opponent, Stats *stats, FILE *outfile);

int check_shot(Cell board[][BOARD_DIM], int shot_row, int shot_col);

char board_update(Cell abord[][BOARD_DIM], int shot_row, int shot_col, int shot_result);

void stat_update(Stats *stats, int shot_result);

void sink_check(Cell board[][BOARD_DIM], char symbol_hit, int player, FILE *outfile);

int win_check(Cell board[][BOARD_DIM], int player,Stats *stats, FILE *outfile);

void print_winner(FILE *outfile, int player);

void print_sunk_ship(FILE *outfile, char ship_symbol, int player);

void print_coordinate(FILE *outfile, int row, int col, int player, int shot_result);

void print_stats(FILE *outfile, Stats *stats, int player);
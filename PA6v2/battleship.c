#include "battleship.h"



void board_fill(Cell board[][BOARD_DIM], int board_dim)
{
	for (int row_index = 0; row_index < board_dim; row_index++)
	{
		for (int col_index = 0; col_index < board_dim; col_index++)
		{
			Cell new_cell;
			new_cell.occupied = 0;
			new_cell.symbol = '-';
			board[row_index][col_index] = new_cell;
		}
	}
}

void ship_list_fill(Ship ship_list[5], int num_of_ships)
{
	char ship_chars[5] = { 'c','b','r','s','d' };
	int ship_lengths[5] = { 5, 4, 3, 3, 2 };
	char ship_names[5][20] = { {"carrier"}, {"battleship"}, {"cruiser"}, {"submarine"}, {"destroyer"} };

	for (int ship_index = 0; ship_index < num_of_ships; ship_index++)
	{
		Ship new_ship;
		new_ship.length = ship_lengths[ship_index];
		new_ship.symbol = ship_chars[ship_index];
		strcpy(new_ship.name, ship_names[ship_index]);
		ship_list[ship_index] = new_ship;
	}
}

void clear_screen(void)
{
	system("cls");
}

void enter_pause(void)
{
	printf("Enter 1 to Continue: \n");
	int user_input = 0;
	do
	{
		scanf("%d", &user_input);
	} while (user_input != 1);
}

void print_board(Cell board[][BOARD_DIM], int board_dim, int current_player)
{
	printf("Player %d's board:\n", current_player);
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	for (int row_index = 0; row_index < board_dim; row_index++)
	{
		printf("%d ", row_index);
		for (int col_index = 0; col_index < board_dim; col_index++)
		{
			
			if (current_player == 2 && board[row_index][col_index].symbol != '-' && board[row_index][col_index].symbol != 'm' && board[row_index][col_index].symbol != '*')
			{
				printf("- ");
			}
			else
			{
				printf("%c ", board[row_index][col_index].symbol);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void display_menu(void)
{
	printf("***** Welcome to Battleship! *****\nRules of the Game:\n\n1. This is a two player game.\n2. Player1 is you and Player2 is the computer.");
	printf("\nI could write the rest of the rules here, but you already know how to\nplay and this a computer science class, not a writing class...\n");
	enter_pause();
	clear_screen();
}

void choose_ship_placement_method(Cell player_board[][BOARD_DIM], Ship ship_list[NUM_SHIPS])
{
	int placement_choice = 0;
	printf("Please select from the following menu:\n1. Enter positions of ships manually.\n2. Allow the program to randomly select positions of ships.\n");
	do
	{
		scanf("%d", &placement_choice);
		if (placement_choice != 1 && placement_choice != 2)
		{
			printf("invalid selection...\n");
		}
	} while (placement_choice != 1 && placement_choice != 2);

	if (placement_choice == 1)
	{
		manually_place_ships_on_board(player_board, ship_list);
	}
	else if (placement_choice == 2)
	{
		printf("randomly selecting...\n");
		randomly_place_ships_on_board(player_board, ship_list);
	}
}

int select_who_starts(void)
{
	int player = 0;
	player = generate_random_num(1);
	return player;
}

int generate_random_num(int max_size)
{
	int num = 0;
	num = rand() % (max_size + 1);
	return num;
}

void randomly_place_ships_on_board(Cell board[][BOARD_DIM], Ship ship_list[NUM_SHIPS])
{

	int rand_row = 0, rand_col = 0, current_ship_length = 0, current_direction = 0, end_row = 0, end_col = 0, placement_is_valid = 0;
	char current_ship_char;

	for (int ship_index = 0; ship_index < NUM_SHIPS; ship_index++)
	{

		current_ship_length = ship_list[ship_index].length;
		
		current_ship_char = ship_list[ship_index].symbol;
		do
		{
			current_direction = generate_random_num(1);	//random direction, 0 or 1

			if (current_direction == DOWN)
			{
				rand_row = generate_random_num(BOARD_DIM - current_ship_length);
				rand_col = generate_random_num(BOARD_DIM - 1);
				end_row = rand_row + (current_ship_length - 1);
				end_col = rand_col;
			}
			else if (current_direction == RIGHT)
			{
				rand_row = generate_random_num(BOARD_DIM - 1);
				rand_col = generate_random_num(BOARD_DIM - current_ship_length);
				end_row = rand_row;
				end_col = rand_col + (current_ship_length - 1);
			}
			placement_is_valid = check_ship_placement(board, rand_row, rand_col, end_row, end_col);
		} while (!placement_is_valid);

		place_ship(board, current_ship_char, rand_row, end_row, rand_col, end_col);
	}


}

void manually_place_ships_on_board(Cell board[][BOARD_DIM], Ship ship_list[NUM_SHIPS])
{
	int row_choices[5] = { 0 }, col_choices[5] = { 0 }, current_ship_length = 0, is_vacant = 0, is_contiguous = 0, is_linear = 0, is_in_bounds;
	char current_ship_char = '\0', current_ship_name[20] = "";
	int start_row = 0, end_row = 0, start_col = 0, end_col = 0;
	for (int ship_index = 0; ship_index < NUM_SHIPS; ship_index++)
	{
		current_ship_length = ship_list[ship_index].length;
		current_ship_char = ship_list[ship_index].symbol;
		strcpy(current_ship_name, ship_list[ship_index].name);

		do
		{
			printf("\nPlease enter the %d cells to place the %s on: ", current_ship_length, current_ship_name);

			for (int cell_num = 0; cell_num < current_ship_length; cell_num++)
			{
				scanf("%d%d", &row_choices[cell_num], &col_choices[cell_num]);

				start_row = row_choices[0];
				end_row = row_choices[current_ship_length - 1];
				start_col = col_choices[0];
				end_col = col_choices[current_ship_length - 1];

			}
			is_contiguous = check_if_cells_contiguous(board, row_choices, col_choices, current_ship_length);
			is_vacant = check_ship_placement(board, start_row, start_col, end_row, end_col);
			is_linear = check_if_horizontal_or_vertical(board, row_choices, col_choices, current_ship_length);
			is_in_bounds = check_if_in_bounds(board, row_choices, col_choices, current_ship_length);
			if (!is_vacant)
			{
				printf("\nYour placement is on top of another ship...\n");
			}
			//maybe split the while statement into variables and then put the varables in the while...
		} while (!is_contiguous || !is_vacant || !is_linear || !is_in_bounds);
	
		place_ship(board, current_ship_char, start_row, end_row, start_col, end_col);
	}
}

int check_ship_placement(Cell board[][BOARD_DIM], int start_row, int start_col, int end_row, int end_col)
{
	int is_valid = 1;

	if (start_row == end_row)
	{
		for (int col_index = start_col; col_index <= end_col; col_index++)
		{
			if (board[start_row][col_index].occupied)
			{
				is_valid = 0;
			}
		}
	}

	if (start_col == end_col)
	{
		for (int row_index = start_row; row_index <= end_row; row_index++)
		{
			if (board[row_index][start_col].occupied)
			{
				is_valid = 0;
			}
		}
	}
	return is_valid;
}

int check_if_cells_contiguous(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length)
{
	int is_adjacent = 1;
	double rise = 0, run = 0, distance_between_cells = 0.0;
	
	//checks the distance between consecutive cells choices. if cells are adjacent, distance should = 1
	for (int choice_index = 0; choice_index < (ship_length - 1); choice_index++)
	{
		run = col_choices[choice_index + 1] - col_choices[choice_index];
		rise = row_choices[choice_index + 1] - row_choices[choice_index];
		distance_between_cells = sqrt( pow( run, 2) + pow( rise, 2) );
		//printf("distance between %d and %d: %lf\n", choice_index, choice_index + 1, distance_between_cells);
		if ((distance_between_cells > 1.2) || (distance_between_cells < 0.8))
		{
			is_adjacent = 0;
		}
	}
	//printf("is_adjacent: %d", is_adjacent);
	
	if (!is_adjacent)
	{
		printf("\nThe cells entered are not adjacent...\n");
	}
	
	return is_adjacent;
}

int check_if_horizontal_or_vertical(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length)
{
	//if this function is called, the cell choices have already been determined to be adjacent in order.
	int is_horizontal_or_vertical = 0, is_horizontal = 1, is_vertical = 1, initial_distance = 0, new_distance = 0;
	initial_distance = col_choices[1] - col_choices[0];
	if (row_choices[0] == row_choices[1])
	{
		is_vertical = 0;
		for (int choice_index = 0; choice_index < ship_length - 1; choice_index++)
		{
			new_distance = col_choices[choice_index + 1] - col_choices[choice_index];
			if (new_distance != initial_distance)
			{
				is_horizontal = 0;
			}
		}

	}
	initial_distance = row_choices[1] - row_choices[0];
	if (col_choices[0] == col_choices[1])
	{
		is_horizontal = 0;
		for (int choice_index = 0; choice_index < ship_length - 1; choice_index++)
		{
			new_distance = row_choices[choice_index + 1] - row_choices[choice_index];
			if (new_distance != initial_distance)
			{
				is_vertical = 0;
			}
		}
	}
		
	if (is_vertical || is_horizontal)
	{	
		is_horizontal_or_vertical = 1;
	}

	else
	{
		printf("\nYour placement was not horizontol or vertical...\n");
	}
		return is_horizontal_or_vertical;
} 

int check_if_in_bounds(Cell board[][BOARD_DIM], int row_choices[5], int col_choices[5], int ship_length)
{
	int in_bounds = 1;


	for (int choice_index = 0; choice_index < ship_length; choice_index++)
	{
		if ( row_choices[choice_index] > 9 || col_choices[choice_index] > 9 )
		{
			in_bounds = 0;
		}
		else if (row_choices[choice_index] < 0 || col_choices[choice_index] < 0)
		{
			in_bounds = 0;
		}
	}
	if (!in_bounds)
	{
		printf("\nYour placement was out of bounds...\n");
	}
	return in_bounds;
}

void place_ship(Cell board[][BOARD_DIM], char ship_char, int start_row, int end_row, int start_col, int end_col)
{
	if (start_row == end_row)
	{
		for (int col_index = start_col; col_index <= end_col; ++col_index)
		{
			board[start_row][col_index].symbol = ship_char;
			board[start_row][col_index].occupied = 1;
		}
	}

	if (start_col == end_col)
	{
		for (int row_index = start_row; row_index <= end_row; ++row_index)
		{
			board[row_index][start_col].symbol = ship_char;
			board[row_index][start_col].occupied = 1;
		}
	}
}

/*********************************************************************************/
//turn functions and shot functions


void turn(Cell player_board[][BOARD_DIM], Cell opponent_board[][BOARD_DIM], int player, int opponent, Stats *stats, FILE *outfile)
{
	


	int shot_row = 0, shot_col = 0, shot_result = 0;
	char symbol_hit = '\0';
	if (player == 2)
	{

		do
		{
			shot_row = generate_random_num(9);
			shot_col = generate_random_num(9);
			shot_result = check_shot(opponent_board, shot_row, shot_col);

		} while (shot_result == -1);
	}
	else if (player == 1)
	{
		printf("Please enter a target: \n");
		do
		{
			scanf("%d%d", &shot_row, &shot_col);
			shot_result = check_shot(opponent_board, shot_row, shot_col);
		} while (shot_result == -1);
	}

	clear_screen();
	printf("The computer fired: \n");
	symbol_hit = board_update(opponent_board, shot_row, shot_col, shot_result);
	printf("updating %d's stats",player);
	stat_update(stats, shot_result);
	print_coordinate(outfile, shot_row, shot_col, player, shot_result);
	sink_check(opponent_board, symbol_hit, opponent, outfile);
	//print_board(player_board, BOARD_DIM, player);
	//print_board(opponent_board, BOARD_DIM, opponent);
}

int check_shot(Cell board[][BOARD_DIM], int shot_row, int shot_col)
{
	//-1 = already hit, 1 = hit, 0 = miss

	int shot_result = 0;
	if (shot_row > 9 || shot_row < 0 || shot_col > 9 || shot_col < 0)
	{
		shot_result = -1;
		printf("Your shot is out of bounds...\n");
	}
	else if (board[shot_row][shot_col].symbol == '-')
	{
		shot_result = 0;
	}
	else if (board[shot_row][shot_col].symbol == '*' || board[shot_row][shot_col].symbol == 'm')
	{
		printf("invalid shot\n");
		shot_result = -1;
	}
	else
	{
		shot_result = 1;
	}
	return shot_result;
}

char board_update(Cell board[][BOARD_DIM], int shot_row, int shot_col, int shot_result)
{
	char symbol_hit = '-';
	if (shot_result == 1)
	{
		symbol_hit = board[shot_row][shot_col].symbol;
		printf("%d,%d is a hit!\n", shot_row, shot_col);
		board[shot_row][shot_col].symbol = '*';
	}
	else if (shot_result == 0)
	{
		printf("%d,%d is a miss...\n", shot_row, shot_col);
		board[shot_row][shot_col].symbol = 'm';
	}
	return symbol_hit;
}

void stat_update(Stats *stats, int shot_result)
{
	if (shot_result)
	{
		stats->shots++;
		stats->hits++;
		
	}
	else
	{
		stats->shots++;
		stats->misses++;
	}
}

void sink_check(Cell board[][BOARD_DIM], char symbol_hit, int player, FILE *outfile)
{
	int ship_was_sunk = 1;
	for (int row_index = 0; row_index < BOARD_DIM; row_index++)
	{
		for (int col_index = 0; col_index < BOARD_DIM; col_index++)
		{
			if (board[row_index][col_index].symbol == symbol_hit)
			{
				ship_was_sunk = 0;
			}
		} 
	}
	if (ship_was_sunk)
	{
		print_sunk_ship(outfile, symbol_hit, player);
	}
}

int win_check(Cell board[][BOARD_DIM], int player, Stats *stats, FILE *outfile)
{
	int is_game_won = 1;
	for (int row_index = 0; row_index < BOARD_DIM; row_index++)
	{
		for (int col_index = 0; col_index < BOARD_DIM; col_index++)
		{
			if (board[row_index][col_index].symbol != '-' && board[row_index][col_index].symbol != '*' && board[row_index][col_index].symbol != 'm')
			{
				is_game_won = 0;
			}
		}
	}
	if (is_game_won)
	{
		print_winner(outfile, player);
		stats->won = 1;
	}
	return is_game_won;
}

void print_winner(FILE *outfile, int player)
{
	printf("Player %d has won the Game.\n", player);
	fprintf(outfile, "Player %d has won the Game.\n", player);
}

void print_sunk_ship(FILE *outfile, char ship_symbol, int player)
{
	if (ship_symbol == 'c')
	{
		fprintf(outfile, "Player %d's Carrier sunk.\n", player);
	}
	else if (ship_symbol == 'b')
	{
		fprintf(outfile, "Player %d's Battleship sunk.\n", player);
	}
	else if (ship_symbol == 'r')
	{
		fprintf(outfile, "Player %d's Cruiser sunk.\n", player);
	}
	else if (ship_symbol == 's')
	{
		fprintf(outfile, "Player %d's Submarine sunk.\n", player);
	}
	else if (ship_symbol == 'd')
	{
		fprintf(outfile, "Player %d's Destroyer sunk.\n", player);
	}
}

void print_coordinate(FILE *outfile, int row, int col, int player, int shot_result)
{
	if (shot_result)
	{
		fprintf(outfile, "Player %d: %d,%d Hit\n", player, row, col);
	}
	else
	{
		fprintf(outfile, "Player %d: %d,%d Miss\n", player, row, col);
	}
}

void print_stats(FILE *outfile, Stats *stats, int player)
{
	double hit_percent = 0.0;
	double hits = 0.0;
	hits = stats->hits;
	hit_percent = hits / (stats->shots) * 100;

	fprintf(outfile, "\n\nPlayer %d's Stats:\n", player);
	fprintf(outfile, "Shots: %d\n", stats->shots);
	fprintf(outfile, "Hits: %d\n", stats->hits);
	fprintf(outfile, "Misses: %d\n", stats->misses);
	fprintf(outfile, "Hit Ratio: %lf\n", hit_percent);
	if (stats->won)
	{
		fprintf(outfile, "Result: Won");
	}
	else
	{
		fprintf(outfile, "Result: Lost\n");
	}
}
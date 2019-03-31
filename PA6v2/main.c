#include "battleship.h"

int main()
{
	unsigned start_time = time(NULL);
	srand(start_time);

	FILE *outfile = NULL;
	outfile = fopen("battleship.log", "w");
	
	Cell player_board[10][10];
	Cell computer_board[10][10];
	Cell computer_board_view[10][10];
	Ship ship_list[5];
	Stats player_stats = {0,0,0,0};
	Stats computer_stats = {0,0,0,0};
	

	
	board_fill(player_board, BOARD_DIM);
	board_fill(computer_board, BOARD_DIM);
	board_fill(computer_board_view, BOARD_DIM);
	ship_list_fill(ship_list, NUM_SHIPS);

	int placement_choice = 0, game_running = 1;
	
	display_menu();

	choose_ship_placement_method(player_board, ship_list);

	randomly_place_ships_on_board(computer_board, ship_list);
	printf("Player 2's board has been generated.\n");

	print_board(computer_board, BOARD_DIM, 2);
	print_board(player_board, BOARD_DIM, 1);
	
	
	do
	{
		turn(player_board, computer_board, 1, 2, &player_stats, outfile);//changed
		//print boards
		print_board(computer_board, BOARD_DIM, 2);
		print_board(player_board, BOARD_DIM, 1);
		game_running = !win_check(computer_board, 1,&player_stats, outfile);
		if (!game_running)
		{
			break;
		}
		printf("The computer will now play.\n");
		enter_pause();
		turn(computer_board, player_board, 2, 1, &computer_stats, outfile);
		//print boards
		print_board(computer_board, BOARD_DIM, 2);
		print_board(player_board, BOARD_DIM, 1);
		game_running = !win_check(player_board, 2, &computer_stats, outfile); 
		
	} while (game_running);

	printf("Game is over\n");
	print_stats(outfile, &player_stats, 1);
	print_stats(outfile, &computer_stats, 2);
	
	fclose(outfile);
	return 0;

}
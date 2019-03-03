#include "taag.h"

void					menu_play(t_data data)
{
	(void)data;
	string path = "";

	int size = 0;
	for (int i = 0; i < 6; i++)
		if (account->actor_array[i] != NULL)
			size++;

	if (size == 0)
		return ;

	menu_choose_map(t_data(1, &path));

	if (path != "")
	{
		account->start_game(MAP_PATH + path + MAP_EXT);

		if (account->board.board_size.x >= 5 && account->board.board_size.y >= 5)
		{
			account->board.placement_phase_tutorial(account->actor_array);
		}
	}
}

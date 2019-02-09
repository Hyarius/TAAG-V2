#include "taag.h"

void					menu_play(t_data data)
{
	(void)data;
	/*SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	while (play)
	{
		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
		}
		render_screen(true);
	}*/

	string path = "";

	menu_choose_map(t_data(1, &path));

	if (path != "")
	{
		t_game_engine	game = s_game_engine(MAP_PATH + path + MAP_EXT);

		if (game.board.board_size.x >= 5 && game.board.board_size.y >= 5)
			game.placement_phase();
	}
}

#include "taag.h"

vector<string>		list_file_load;
string 				*text_list_load[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_load.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_load.size()) || *index + i < 0)
			*(text_list_load[i]) = "";
		else
			*(text_list_load[i]) = list_file_load[i + *index];
	}
}

static void		quit_load(t_data data)// player / entry_name / entry_path / pool / play / num
{
	t_actor		*player = (t_actor *)(data.data[0]);
	string		*entry_name = (string *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
	int			*pool_value = (int *)(data.data[3]);
	bool 		*play = (bool *)(data.data[4]);
	int			i = (int &)(data.data[5]);
	size_t		*tile_index = *(size_t **)(data.data[6]);
	t_vect		*sprite_target = (t_vect *)(data.data[7]);

	if (*(text_list_load[i]) != "")
	{
		*player = read_actor(ACTOR_PATH + *(text_list_load[i]) + ACTOR_EXT);
		size_t t = 0;
		for(t = 0; &(sprite_map[human_sprite_name[t]]) != player->tile;t++)
			;
		*tile_index = t;
		*sprite_target = player->sprite;
		*entry_path = *(text_list_load[i]);
		*entry_name = player->name;
		t_actor base;
		*pool_value = 30 - ((player->stat.hp.max - base.stat.hp.max) + (player->stat.pa.max - base.stat.pa.max) + (player->stat.pm.max - base.stat.pm.max) + (player->stat.init - base.stat.init));
		*play = false;
	}
}

void			menu_load_actor(t_data data) // 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_actor		*player = (t_actor *)(data.data[1]);
	string		*entry_name = (string *)(data.data[2]);
	string		*entry_path = (string *)(data.data[3]);
	int			*pool_value = (int *)(data.data[4]);
	size_t		*tile_index = *(size_t **)(data.data[5]);
	t_vect		*sprite_target = *(t_vect **)(data.data[6]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Which file did you want to load ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	list_file_load = list_files(ACTOR_PATH, ACTOR_EXT);
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_load.size()) ? list_file_load[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_load, t_data(8, player, entry_name, entry_path, pool_value, &play, i, &tile_index, sprite_target));// player / entry_name / entry_path / pool / play / num
		text_list_load[i] = &(button->button->text);
		gui.add(button);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();

		render_screen();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
			{
				modify_index(t_data(2, &index, -3));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(2, &index, 3));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -3));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 3));
			}
		}
	}
}

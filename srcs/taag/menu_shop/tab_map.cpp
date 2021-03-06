#include "taag.h"

vector<s_shop_item *>		map_item_vector;
s_shop_item					*map_item_list[18];

void				check_map_lock()
{
	vector<string>		list_base = list_files(MAP_PATH, MAP_EXT);

	if (account->tuto_state < TUTO_SIZE)
		list_base = { "tutorial-00" };
	map_item_vector.clear();

	for (size_t i = 0; i < list_base.size(); i++)
	{
		if (check_file_exist(SHOP_MAP_PATH + list_base[i] + SHOP_EXT) == true)
		{
			s_shop_item tmp = s_shop_item(SHOP_MAP_PATH + list_base[i] + SHOP_EXT);
			map_item_vector.push_back(new t_shop_item(tmp));
		}
	}
}

void			actualize_map_tab(int *index)
{
	check_map_lock();
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(map_item_vector.size()))
			map_item_list[i] = NULL;
		else
			map_item_list[i] = map_item_vector[i + (*index)];
	}
}

void			modify_index_map_tab(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -12 && *index + delta < ((int)(map_item_vector.size()) / 6) * 6 + 6)
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(map_item_vector.size()))
			map_item_list[i] = NULL;
		else
			map_item_list[i] = map_item_vector[i + (*index)];
	}
}

void		buy_map(t_data data)
{
	s_shop_item **item = (s_shop_item **)(data.data[0]);
	int			*index = (int *)(data.data[1]);

	if (item == NULL || *item == NULL)
		return ;

	if (account->add_gold(-((*item)->price)) == true)
	{
		account->map_unlock.push_back((*item)->to_add);
		sort(account->map_unlock.begin(), account->map_unlock.end());
		check_map_lock();
		for (int i = 0; i < 18; i++)
		{
			if (i + (*index) < 0 || i + *index >= (int)(map_item_vector.size()))
				map_item_list[i] = NULL;
			else
				map_item_list[i] = map_item_vector[i + (*index)];
		}
		save_game_engine();
	}
}

void create_tab_map(t_data data)
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at create_tab_map : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");

	write_in_log("Setting data value : ");
	t_gui *gui = (t_gui *)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	*gui = t_gui();
	write_in_log("Setting complete\n");

	write_in_log("Check map locked : ");
	check_map_lock();
	write_in_log("DONE\n");

	write_in_log("Setting map lock list : ");
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(map_item_vector.size()))
			map_item_list[i] = NULL;
		else
			map_item_list[i] = map_item_vector[i + (*index)];
	}
	write_in_log("DONE\n");

	write_in_log("Setting buttons variable : ");
	t_vect size = t_vect(3.6, 5.2);
	t_vect icon_size = t_vect(1.2, 1.2);
	t_vect price_size = t_vect(1.5, 0.6);
	write_in_log("DONE\n");

	write_in_log("Creation buy button : ");
	for (int i = 0; i < 18; i++)
	{
		t_vect coord = t_vect(1.25 + (i % 6) * size.x + (0.2 * (i % 6)), (2.4 + (i / 6) * size.y + (0.4 * (i / 6))));
		gui->add(new t_shop_map(new t_button(new s_text_button(
				"", DARK_GREY,
				gui->unit * coord,
				gui->unit * size, 4,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				buy_map, t_data(2, &(map_item_list[i]), index)),
				&(map_item_list[i]),
				gui->unit * (coord + t_vect((size.x - icon_size.x) / 2.0, 0.2)),
				gui->unit * icon_size, gui->unit * (coord + t_vect(0.2, 0.4 + icon_size.y)),
				gui->unit * (coord + (t_vect((size.x - price_size.x) / 2.0, size.y - price_size.y - 0.2))),
				gui->unit * price_size));
	}
	write_in_log("Creation complete\n");

	write_in_log("Gui up button : ");
	gui->add(new t_button(new t_text_button(
						" up ", BLACK,
						gui->unit * t_vect(26, 3), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_map_tab, t_data(2, index, -6)));
	write_in_log("Creation complete\n");

	write_in_log("Gui down button : ");
	gui->add(new t_button(new t_text_button(
						"down", BLACK,
						gui->unit * t_vect(26, 13), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_map_tab, t_data(2, index, 6)));
	write_in_log("Creation complete\n");
}

#include "taag.h"

t_spell						*spell_list_shop[18];
s_shop_item					*spell_item_list[18];
vector<s_shop_item *>		spell_item_vector;

void				check_spell_lock()
{
	vector<string>		list_base = list_files(SPELL_PATH, SPELL_EXT);

	spell_item_vector.clear();

	for (size_t i = 0; i < list_base.size(); i++)
	{
		if (check_file_exist(SHOP_SPELL_PATH + list_base[i] + SHOP_EXT) == true)
		{
			s_shop_item tmp = s_shop_item(SHOP_SPELL_PATH + list_base[i] + SHOP_EXT);

			spell_item_vector.push_back(new t_shop_item(tmp));
		}
	}
}

void			modify_index_spell_tab(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -12 && *index + delta < ((int)(spell_item_vector.size()) / 6) * 6 + 6)
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(spell_item_vector.size()))
		{
			spell_list_shop[i] = &(spell_map["NULL"]);
			spell_item_list[i] = NULL;
		}
		else
		{
			spell_list_shop[i] = &(spell_map[spell_item_vector[i + *index]->to_add]);
			spell_item_list[i] = spell_item_vector[i + (*index)];
		}
	}
}

void		buy_spell(t_data data)
{
	s_shop_item **item = (s_shop_item **)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	if (item == NULL || *item == NULL)
		return ;

	if (account->add_gold(-((*item)->price)) == true)
	{
		account->spell_unlock.push_back((*item)->to_add);
		sort(account->spell_unlock.begin(), account->spell_unlock.end());
		check_spell_lock();
		for (int i = 0; i < 18; i++)
		{
			if (i + (*index) < 0 || i + *index >= (int)(spell_item_vector.size()))
			{
				spell_list_shop[i] = &(spell_map["NULL"]);
				spell_item_list[i] = NULL;
			}
			else
			{
				spell_list_shop[i] = &(spell_map[spell_item_vector[i + *index]->to_add]);
				spell_item_list[i] = spell_item_vector[i + (*index)];
			}
		}
		save_game_engine();
	}
}

void create_tab_spell(t_data data)
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at create_tab_spell : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");

	write_in_log("Setting data value : ");
	t_gui *gui = (t_gui *)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	*gui = t_gui();
	write_in_log("Setting complete\n");

	write_in_log("Check spell locked : ");
	check_spell_lock();
	write_in_log("DONE\n");

	write_in_log("Setting spell lock list : ");

	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(spell_item_vector.size()))
		{
			spell_list_shop[i] = &(spell_map["NULL"]);
			spell_item_list[i] = NULL;
		}
		else
		{
			spell_list_shop[i] = &(spell_map[spell_item_vector[i + *index]->to_add]);
			spell_item_list[i] = spell_item_vector[i + (*index)];
		}
	}

	write_in_log("DONE\n");

	write_in_log("Setting buttons variable : ");
	int i = 0;

	t_vect size = t_vect(3.6, 5.2);
	write_in_log("DONE\n");

	write_in_log("Creation buy button : ");
	for (int i = 0; i < 18; i++)
	{
		gui->add(new s_shop_spell(new t_spell_card(&(spell_list_shop[i]),
							gui->unit * t_vect(1.25 + (i % 6) * size.x + (0.2 * (i % 6)), (2.4 + (i / 6) * size.y + (0.4 * (i / 6)))),
							gui->unit * size,
							buy_spell, t_data(2, &(spell_item_list[i]), index)),
							&(spell_item_list[i]), gui->unit * t_vect(1.3, -0.2), gui->unit * t_vect(size.x - 1.5, 0.6)));
	}
	write_in_log("Creation complete\n");

	write_in_log("Gui up button : ");

	gui->add(new t_button(new t_text_button(
						" up ", BLACK,
						gui->unit * t_vect(26, 3), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_spell_tab, t_data(2, index, -6)));
	write_in_log("Creation complete\n");

	write_in_log("Gui down button : ");

	gui->add(new t_button(new t_text_button(
						"down", BLACK,
						gui->unit * t_vect(26, 13), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_spell_tab, t_data(2, index, 6)));
	write_in_log("Creation complete\n");
}

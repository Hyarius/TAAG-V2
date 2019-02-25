#include "taag.h"

int 			s_game_engine::calc_max_exp(int level)
{
	return (level * level);
}

void			s_game_engine::add_exp(int delta)
{
	exp.value += delta;
	if (exp.value >= exp.max)
	{
		exp.value -= exp.max;
		level++;
	}
}

bool			s_game_engine::add_gold(int delta)
{
	if (gold + delta < 0)
		return (false);
	gold += delta;
	return (true);
}

void			save_game_engine()
{
	string text = "level:" + to_string(account->level) + "\n";
	text.append("exp:" + to_string(account->exp.value) + "\n");
	text.append("gold:" + to_string(account->gold) + "\n");
	text.append("char");
	for (size_t i = 0; i < 6; i++)
		text.append(":" + account->actor[i]);
	text.append("\n");
	text.append("spell:NULL");
	for (size_t i = 0; i < account->spell_unlock.size(); i++)
		text.append(":" + account->spell_unlock[i]);
	text.append("\n");
	text.append("tile");
	for (size_t i = 0; i < account->tile_unlock.size(); i++)
		text.append(":" + account->tile_unlock[i]);
	rewrite_on_file("ressources/game_object/game_engine/saved_game.eng", text);
}

s_game_engine::s_game_engine()
{
	ifstream		myfile;
	myfile.open("ressources/game_object/game_engine/saved_game.eng");
	if (myfile.fail())
		printf("can't open such file : %s\n", "ressources/game_object/game_engine/saved_game.eng");
	level = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	exp = s_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()), calc_max_exp(level));
	gold = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());

	vector<string> tab = get_strsplit(&myfile, ":", 7);

	for (size_t i = 1; i < tab.size(); i++)
		actor.push_back(tab[i]);

	if (actor.size() != 6)
		error_exit_full("not the good number of actor", actor.size());

	tab = get_strsplit(&myfile, ":", -1);

	for (size_t i = 1; i < tab.size(); i++)
		spell_unlock.push_back(tab[i]);

	tab = get_strsplit(&myfile, ":", -1);

	for (size_t i = 1; i < tab.size(); i++)
		tile_unlock.push_back(tab[i]);

	board = t_game_board();
	set_game_engine(&(this->board));
}

void		s_game_engine::start_game(string path)
{
	board = t_game_board(path);
	set_game_engine(&(this->board));
	actor_array.clear();
	for (size_t i = 0; i < actor.size(); i++)
	{
		if (actor[i] != "NULL")
		{
			t_actor *new_actor = new t_actor(read_actor(ACTOR_PATH + actor[i] + ACTOR_EXT));
			new_actor->team = 1;
			actor_array.push_back(new_actor);
		}
	}
}

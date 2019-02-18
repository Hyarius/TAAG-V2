#include "taag.h"

s_game_board::s_game_board()
{
	vector<string>	node_file = list_files(NODE_PATH, NODE_EXT);

	size_t i = 0;
	while (i < node_file.size())
	{
		node_list.push_back(read_node(NODE_PATH + node_file[i] + NODE_EXT));
		i++;
	}
	board_size = t_vect(0, 0);
	zoom = 1.5;
	sprite_unit = t_vect(32, 32);
	offset = get_win_size() / 2;
	target = t_vect(-board_size.x / 2, -board_size.y / 2);
	cursor_tile = get_interface_tile("simple_cursor");
}

s_game_board::s_game_board(string p_path)
{
	ifstream		myfile;
	vector<string>	tab;
	t_vect			coord;
	size_t			index;

	myfile.open(p_path);
	if (myfile.fail())
		printf("can't open such file : %s\n", p_path.c_str());

	vector<string>	node_file = list_files(NODE_PATH, NODE_EXT);

	size_t i = 0;
	while (i < node_file.size())
	{
		node_list.push_back(read_node(NODE_PATH + node_file[i] + NODE_EXT));
		i++;
	}
	while (!(myfile.eof()))
	{
		tab = get_strsplit(&myfile, ":", -1);
		if (tab.size() < 3)
			break;
		coord = t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str()));
		index = atoi(tab[2].c_str());
		if (board_size.x <= coord.x)
		{
			board_size.x = coord.x + 1;
			cell_layer.resize((size_t)(board_size.x));
		}
		if (board_size.y <= coord.y)
		{
			board_size.y = coord.y + 1;
			size_t count = 0;
			while (count < cell_layer.size())
			{
				cell_layer[count].resize((size_t)(board_size.y + 1));
				count++;
			}
		}
		if (cell_layer[(size_t)(coord.x)].size() < (size_t)(board_size.y))
			cell_layer[(size_t)(coord.x)].resize((size_t)(board_size.y + 1));
		if (index < node_list.size())
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)] = t_cell(coord, &(node_list[index]));
		if (tab.size() >= 5)
		{
			int team = atoi(tab[4].c_str());
			if (team == 0)
				cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor = new t_actor(read_actor(OBS_PATH + tab[3] + OBS_EXT));
			else
				cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor = new t_actor(read_actor(MONSTER_PATH + tab[3] + ACTOR_EXT));
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->path =	tab[3];
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->coord = coord;
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->team = atoi(tab[4].c_str());
			actor_list.push_back(cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor);
			if (cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->team == 2)
				enemy_list.push_back(cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor);
		}
	}
	while (!myfile.eof())
	{
		tab = get_strsplit(&myfile, ":", -1);
		if (tab.size() <= 1)
			break;
		int team = atoi(tab[2].c_str());
		if (team == 0)
			placement_list.push_back(t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str())));
		if (team == 2)
			enemy_placement_list.push_back(t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str())));
	}
	myfile.close();

	zoom = 1.5;
	sprite_unit = t_vect(32, 32);
	offset = get_win_size() / 2;
	target = t_vect(-board_size.x / 2, -board_size.y / 2);
	cursor_tile = &interface_map["simple_cursor"];
}

void				s_game_board::draw_cell_border()
{
	t_vect size = sprite_unit * zoom;

	int i = 0;
	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			//((t_vect(i, j) + target) * size + offset
			t_vect coord = (t_vect(i, j) + target) * size + offset;
			if (i == 0)
				draw_rectangle(coord, t_vect(1, size.y), t_color(0.4, 0.4, 0.4));
			if (j == 0)
				draw_rectangle(coord, t_vect(size.x, 1), t_color(0.4, 0.4, 0.4));
			draw_rectangle(coord + t_vect(size.x, 0), t_vect(1, size.y), t_color(0.4, 0.4, 0.4));
			draw_rectangle(coord + t_vect(0, size.y), t_vect(size.x, 1), t_color(0.4, 0.4, 0.4));
			j++;
		}
		i++;
	}
}

void				s_game_board::draw_animation()
{
	t_vect size = sprite_unit * zoom;

	int i = 0;
	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			if (get_cell(i, j))
			{
				size_t count = 0;
				while (count < get_cell(i, j)->animation.size())
				{
					get_cell(i, j)->animation[count].draw_self(t_vect(i, j) + 0.5, target, offset, size);
					if (get_cell(i, j)->animation[count].index >= get_cell(i, j)->animation[count].len)
						get_cell(i, j)->animation.erase(get_cell(i, j)->animation.begin() + count);
					else
						count++;
				}
			}
			j++;
		}
		i++;
	}
}

t_cell				*s_game_board::get_cell(int x, int y)
{
	if (x < 0 || x >= board_size.x || y < 0 || y >= board_size.y)
		return (NULL);
	if (cell_layer[x][y].node == NULL)
		return (NULL);
	return (&(cell_layer[x][y]));
}

t_cell				*s_game_board::get_cell(t_vect target)
{
	if ((int)(target.x) < 0 || (int)(target.x) >= board_size.x || (int)(target.y) < 0 || (int)(target.y) >= board_size.y)
		return (NULL);
	if (cell_layer[(int)(target.x)][(int)(target.y)].node == NULL)
		return (NULL);
	return (&(cell_layer[(int)(target.x)][(int)(target.y)]));
}

t_vect				s_game_board::get_mouse_pos()
{
	t_vect mouse = (get_mouse_coord() - offset) / (sprite_unit * zoom) - target;

	if (mouse.x < 0)
		return (t_vect(-1, -1));
	else if (mouse.x >= board_size.x)
		return (t_vect(-1, -1));
	if (mouse.y < 0)
		return (t_vect(-1, -1));
	else if (mouse.y >= board_size.y)
		return (t_vect(-1, -1));

	return (t_vect((int)(mouse.x), (int)(mouse.y)));
}

void				s_game_board::draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite)
{
	cursor_tile->prepare_print((coord + target) * size + offset, size, sprite);
}

void				s_game_board::draw_self()
{
	t_vect size = sprite_unit * zoom;
	//(coord + target) * size + offset, size, sprite);
		t_tileset *tile= get_interface_tile("simple_board");
	int i = 0;
	while (i < board_size.x)
	{
		int j = 0;
		while (j < board_size.y)
		{
			if (i == 0 && j == 0) // top - left corner
			{
				tile->prepare_print((t_vect(-1 , -1) + target) * size + offset, size, t_vect(0, 0));
				tile->prepare_print((t_vect(-1 , 0) + target) * size + offset, size, t_vect(0, 1));
				tile->prepare_print((t_vect(0 , -1) + target) * size + offset, size, t_vect(1, 0));
				tile->prepare_print((t_vect(0 , 0) + target) * size + offset, size, t_vect(1, 1));
			}
			else if (i == board_size.x - 1 && j == 0) // top - right corner
			{
				tile->prepare_print((t_vect(i , j - 1) + target) * size + offset, size, t_vect(3, 0));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 1));
				tile->prepare_print((t_vect(i + 1 , j - 1) + target) * size + offset, size, t_vect(4, 0));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 1));
			}
			else if (i == 0 && j == board_size.y - 1) // down - left corner
			{
				tile->prepare_print((t_vect(i - 1, j) + target) * size + offset, size, t_vect(0, 3));
				tile->prepare_print((t_vect(i - 1, j + 1) + target) * size + offset, size, t_vect(0, 4));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(1, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(1, 4));
			}
			else if (i == board_size.x - 1 && j == board_size.y - 1) // down - right corner
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(3, 4));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 3));
				tile->prepare_print((t_vect(i + 1, j + 1) + target) * size + offset, size, t_vect(4, 4));
			}
			else if (i == 0) // left side
			{
				tile->prepare_print((t_vect(i - 1, j) + target) * size + offset, size, t_vect(0, 2));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(1, 2));
			}
			else if (i == board_size.x - 1) // right side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 2));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 2));
			}
			else if (j == 0) // top side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 1));
				tile->prepare_print((t_vect(i , j - 1) + target) * size + offset, size, t_vect(2, 0));
			}
			else if (j == board_size.y - 1) // down side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(2, 4));
			}
			else
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 2));
			j++;
		}
		i++;
	}
	render_triangle_texture(tile->texture_id);
}


void				s_game_board::add_actor(t_actor *new_actor)
{
	actor_list.push_back(new_actor);
}

void				s_game_board::remove_actor(t_actor *old_actor)
{
	size_t count = 0;

	if (old_actor->team == 2)
	{
		while (count < enemy_list.size() && old_actor != enemy_list[count])
			count++;
		if (count < enemy_list.size())
			enemy_list.erase(enemy_list.begin() + count);
	}
	count = 0;
	while (count < actor_list.size() && old_actor != actor_list[count])
		count++;
	if (count < actor_list.size())
	{
		actor_list.erase(actor_list.begin() + count);
	}
}

void				s_game_board::draw_mouse_cursor()
{
	t_vect mouse = get_mouse_pos();

	t_vect size = sprite_unit * zoom;
	if (mouse != t_vect(-1, -1))
		draw_cursor(mouse, target, size, offset, t_vect(1, 0));
	render_triangle_texture(cursor_tile->texture_id);

}

bool				s_game_board::check_anim()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{

			if (get_cell(i, j))
			{
				size_t k = 0;
				while (k < get_cell(i, j)->animation.size())
				{
					if (get_cell(i, j)->animation[k].index < get_cell(i, j)->animation[k].len)
						return (false);
					k++;
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

void				s_game_board::draw_cell_layer()
{
	int i = 0;
	t_vect size = sprite_unit * zoom;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			cell_layer[i][j].draw_cell(target, offset, size);
			j++;
		}
		i++;
	}
	if (board_size == t_vect(0, 0))
		return;
	render_triangle_texture(node_list[0].tile->texture_id);
}

void				s_game_board::draw_visual_info()
{
	size_t i = 0;
	t_vect size = sprite_unit * zoom;

	while (i < actor_list.size())
	{
		actor_list[i]->draw_visual_info(target, offset, size, zoom);
		i++;
	}

	i = 0;
	while (i < board_size.x)
	{
		int j = 0;
		while (j < board_size.y)
		{
			cell_layer[i][j].draw_visual_info(target, offset, size, zoom);
			j++;
		}
		i++;
	}
}

void				s_game_board::draw_cursor_layer()
{
	int i = 0;
	t_vect size = sprite_unit * zoom;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			if (cell_layer[i][j].cursor != t_vect(0, 0))
				draw_cursor(t_vect(i, j), target, size, offset, cell_layer[i][j].cursor);
			j++;
		}
		i++;
	}
	render_triangle_texture(interface_map["simple_cursor"].texture_id);
}

void				s_game_board::draw_placement()
{
	t_vect size = sprite_unit * zoom;

	for (size_t i = 0; i < placement_list.size(); i++)
		cursor_tile->draw_self((placement_list[i] + target) * size + offset, size, t_vect(3, 0));
	for (size_t i = 0; i < enemy_placement_list.size(); i++)
		cursor_tile->draw_self((enemy_placement_list[i] + target) * size + offset, size, t_vect(4, 0));
}

void				s_game_board::reset_board()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			cell_layer[i][j].m_dist = 999;
			cell_layer[i][j].v_dist = 999;
			cell_layer[i][j].cursor = t_vect(0, 0);
			j++;
		}
		i++;
	}
}

void				s_game_board::draw_actor_list()
{
	size_t i = 0;
	t_vect size = sprite_unit * zoom;

	while (i < actor_list.size())
	{
		actor_list[i]->draw_self(target, offset, size);
		i++;
	}
}


void				s_game_board::handle_mouvement(SDL_Event *event)
{
	if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_MIDDLE)
	{
		target.x += (double)(event->motion.xrel) / (sprite_unit.x * zoom);
		target.y += (double)(event->motion.yrel) / (sprite_unit.y * zoom);
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	}
	else
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
}

void				s_game_board::handle_zoom(SDL_Event *event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0 && zoom < 10)
			zoom *= 1.1;
		else if (event->wheel.y < 0 && zoom >= 1)
			zoom *= 0.9;
	}
}

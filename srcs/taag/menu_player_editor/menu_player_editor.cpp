#include "taag.h"

static string 			create_basic_actor()
{
	string text = "name:\n";
	text.append("tileset:H00\n");
	text.append("sprite_pos:0:0\n");
	text.append("health:20\n");
	text.append("action:4\n");
	text.append("mouvement:2\n");
	text.append("initiative:5\n");
	text.append("spell0:NULL\n");
	text.append("spell1:NULL\n");
	text.append("spell2:NULL\n");
	text.append("spell3:NULL\n");
	text.append("spell4:NULL\n");
	text.append("spell5:NULL");
	return (text);
}

static void				select_entry(t_data data)
{
	t_entry *entry_name = (t_entry *)(data.data[0]);
	t_gui	*gui = (t_gui *)(data.data[1]);

	entry_name->entry->selected = true;
	gui->entry = entry_name->entry;
	SDL_StartTextInput();
}

void					menu_actor_editor(t_data data)
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_actor_editor : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");

	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	double i = 0;

	write_in_log("Gui button background : ");
	t_button *back_ground = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);
	write_in_log("Creation complete\n");

	write_in_log("Path setting : ");
	string *path = (string *)(data.data[0]);
	t_actor actor;
	if (*path == "NULL" || check_file_exist(ACTOR_PATH + *path + ACTOR_EXT) == false)
	{
		actor = t_actor();
		actor.tile = get_sprite_tile(account->tile_unlock[0]);
	}
	else
		actor = read_actor(ACTOR_PATH + (*path) + ACTOR_EXT);
	write_in_log("Path entry was : [" + (ACTOR_PATH + (*path) + ACTOR_EXT) + "]\n");

	write_in_log("\n");
	write_in_log("Gui button entry_name : ");
	t_entry *entry_name = new s_entry(new s_text_entry(	"Name of your character", actor.name, BLACK,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0.0)));
	i++;
	write_in_log("Creation complete\n");
	string *name = &(entry_name->entry->text);

	entry_name->entry->max_len = 32;

	write_in_log("Gui button pool iterator : ");
	t_actor base;
	int pool = account->calc_pool() - ((actor.stat.hp.max - base.stat.hp.max) / 2 + (actor.stat.pa.max - base.stat.pa.max) * 3 + (actor.stat.pm.max - base.stat.pm.max) * 3 + (actor.stat.init - base.stat.init));
	t_iterator *pool_iterator = new s_iterator(&pool, &pool, 0, 0, 0, 100,
		new t_button(new s_text_button(
				"Attrib points (AtbP) left : ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(3.5, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);
	i++;
	write_in_log("Creation complete with pool = " + to_string(pool) + "\n");

	write_in_log("Gui button hp iterator : ");
	t_iterator *hp_iterator = new s_iterator(&(actor.stat.hp.max), &pool, 2, 1, 15, 150,
		new t_button(new s_text_button(
				"Health points (2 hp / AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button action iterator : ");
	t_iterator *action_iterator = new s_iterator(&(actor.stat.pa.max), &pool, 1, 3, 4, 12,
		new t_button(new s_text_button(
				"Action points (1 act / 3 AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button mouvement iterator : ");
	t_iterator *mouvement_iterator = new s_iterator(&(actor.stat.pm.max), &pool, 1, 3, 2, 6,
		new t_button(new s_text_button(
				"Mouvement points (1 mvt / 3 AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button initiative iterator : ");
	t_iterator *initiative_iterator = new s_iterator(&(actor.stat.init), &pool, 1, 1, 0, 12,
		new t_button(new s_text_button(
				"Initiative (1 init / AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;
	write_in_log("Creation complete\n");

	size_t	*tile_index;
	t_vect	*sprite_target;

	i += 3.4;
	write_in_log("Gui button save : ");
	t_button	*save_button = new t_button(new s_text_button(
		"Save actor", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_save_actor, t_data(6, &gui, &actor, path, &tile_index, &sprite_target, &play));//0 - gui / 1 - t_actor * / 2 - & file name
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button load : ");
	t_button	*load_button = new t_button(new s_text_button(
			"Load actor", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_load_actor, t_data(7, &gui, &actor, name, path, &pool, &tile_index, &sprite_target));// 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button delete : ");
	t_button	*delete_button = new t_button(new s_text_button(
			"Delete actor", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit,
			t_vect(8, 1) * gui.unit,
			5,
			t_color(0.4, 0.4, 0.4),
			t_color(0.6, 0.6, 0.6)),
			menu_delete_actor,
			t_data(7, &gui, &actor, name, path, &pool, &tile_index, &sprite_target));// 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
	i++;
	write_in_log("Creation complete\n");

	write_in_log("Gui button quit : ");
	t_button	*quit_button = new t_button(new s_text_button(
			"Quit", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_quit, t_data(2, &gui, &play));
	i++;
	write_in_log("Creation complete\n");

	i = 1;
	write_in_log("Gui button tileset selector : ");
	t_tileset_button *tile_button = new s_tileset_button(actor.tile, actor.sprite, t_vect(4, 0),
			t_vect(14.9, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1 + (1.2 * (i + 1)), 1 + (1.2 * (i + 1))) * gui.unit, 5);
	//t_vect *sprite = &(tile_button->selected);
	t_sprite_iterator *tileset_selector = new t_sprite_iterator(
				t_vect(3, 4), &(account->tile_unlock),
				new t_button(new s_text_button(
						"Tileset : ", DARK_GREY,
						t_vect(9.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
						"-", DARK_GREY,
						t_vect(13.7, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(tile_button,
						NULL, NULL),
				new t_button(new s_text_button(
						"+", DARK_GREY,
						t_vect(18.5, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	((t_tileset_button *)(tileset_selector->container->button))->data_left = t_data(4, &gui, &(tileset_selector->i), &((t_tileset_button *)(tileset_selector->container->button))->selected, &(account->tile_unlock));
	i++;
	write_in_log("Creation complete\n");

	tile_index = &(tileset_selector->i);
	sprite_target = &(((t_tileset_button *)(tileset_selector->container->button))->selected);

	write_in_log("\n");
	write_in_log("Adding button to gui : ");
	gui.add(back_ground);
	gui.add(ENTRY_NUM, entry_name);
	gui.add(pool_iterator);
	gui.add(hp_iterator);
	gui.add(action_iterator);
	gui.add(mouvement_iterator);
	gui.add(initiative_iterator);
	gui.add(tileset_selector);
	gui.add(save_button);
	gui.add(load_button);
	gui.add(delete_button);
	gui.add(quit_button);
	write_in_log("Adding complete\n");

	write_in_log("\n");
	write_in_log("Adding spell button : ");
	for (int i = 0; i < 6; i++)
		gui.add(new t_spell_card(&actor.spell[i], gui.unit * t_vect(9.2 + (i % 3) * 4 + (0.2 * (i % 3)), (4.6 + (i / 3) * 6 + (0.2 * (i / 3)))), gui.unit * t_vect(4, 6), menu_choose_spell, t_data(3, &gui, i, &actor, &play)));
	write_in_log("Adding complete\n");
	write_in_log("\n");

	write_in_log("Creating tutorial button : ");
	if (gui_tutorial[3].object_list.size() == 0)
	{
		gui_tutorial[3] = t_gui(30, 20);

		gui_tutorial[3].add(TUTORIAL_NUM, new s_tutorial_button(NULL,
			new t_button(new s_paragraph_button(
					"Here is one of the most important part of this game : the character editor \n Here, you will be able to edit your character spells, stats and even looks ! \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
					t_vect(22, 1) * gui_tutorial[0].unit, t_vect(7, 18) * gui_tutorial[0].unit, 5, //object info
					t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[4].object_list.size() == 0)
	{
		gui_tutorial[4].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 1) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"Here, you can write your character name, pseudo, or whatever you want to be the most powerful and famous player out there ! \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.5, 0.75) * gui_tutorial[0].unit, t_vect(7, 4.5) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[5].object_list.size() == 0)
	{
		gui_tutorial[5].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 2.2) * gui.unit, t_vect(8, 6) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"Here, you can find 2 types of things : \n First, you have the amount of attribute points left to give. You will receive some at every level up ! \n You can spend them into the 4 types of caracteristics : \n \n - Health Points : Represent how much damage you can take before you faint \n - Action Points : Points used in combat to cast spell \n - Mouvement Points : Points used in combat to move around the map \n - Initiative : Determine in which order you and your enemy will play \n \n You can find how much attrib. points every caracteristic cost next to the caracteristic name. \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.5, 0.75) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[6].object_list.size() == 0)
	{
		gui_tutorial[6].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(13.7, 1.0) * gui.unit, t_vect(5.8, 3.4) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"Here is the sprite selector, where you can choose the look of your character. \n You can select it by pressing the \"<\" or \">\" button, or by simply clicking at the center of the sprite to open the selection menu. \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(11.6, 4.6) * gui_tutorial[0].unit, t_vect(10, 4.5) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[7].object_list.size() == 0)
	{
		gui_tutorial[7].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(9.2, 4.6) * gui.unit, t_vect(12.4, 12.2) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"Here, you will find the most important feature of this menu : the spell selector ! \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.2, 1.5) * gui_tutorial[0].unit, t_vect(12.4, 3) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[8].object_list.size() == 0)
	{
		gui_tutorial[8].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(9.2, 4.6) * gui.unit, t_vect(4, 6) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_choose_spell, t_data(3, &gui, 0, &actor)),
			new t_button(new s_paragraph_button(
				"Click on this spell, to open the spell selector menu.", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.2, 1) * gui_tutorial[0].unit, t_vect(12.4, 3) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true));
	}
	((s_tutorial_button *)(gui_tutorial[8].object_list[TUTORIAL_NUM][0]))->button->button->data_left = t_data(3, &gui, 0, &actor);

	if (gui_tutorial[12].object_list.size() == 0)
	{
		gui_tutorial[12] = t_gui(30, 20);

		gui_tutorial[12].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(9.2, 4.6) * gui.unit, t_vect(4, 6) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"All right ! Now our character has one spell ! \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.2, 1) * gui_tutorial[0].unit, t_vect(12.4, 3) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[13].object_list.size() == 0)
	{
		gui_tutorial[13].add(TUTORIAL_NUM, new s_tutorial_button(NULL,
			new t_button(new s_paragraph_button(
				"Now that we got a spell, we should go and play a game no ? Before that, we need to give this character a proper name ! \n Click on the name entry, and give him an awesome name. Then click on the \"Save actor \" button to save this actor.", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.2, 1) * gui_tutorial[0].unit, t_vect(12.4, 3) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true));
		gui_tutorial[13].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 1) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			select_entry, t_data(2, entry_name, &gui)), NULL, false));
		gui_tutorial[13].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 12.28) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_save_actor, t_data(6, &gui, &actor, path, &tile_index, &sprite_target, &play)), NULL, true));
	}
	((s_tutorial_button *)(gui_tutorial[13].object_list[TUTORIAL_NUM][1]))->button->button->data_left = t_data(2, entry_name, &gui);
	((s_tutorial_button *)(gui_tutorial[13].object_list[TUTORIAL_NUM][2]))->button->button->data_left = t_data(6, &gui, &actor, path, &tile_index, &sprite_target, &play);
	write_in_log("Creating complete\n");
	write_in_log("\n");

	static int a = 2;
	while (play)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_player_editor loop ---\n");
			write_in_log("Setting actor variable : ");
		}
		actor.name = *name;
		actor.tile = get_sprite_tile(account->tile_unlock[tileset_selector->i % account->tile_unlock.size()]);
		actor.sprite = *sprite_target;

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Prepare screen : ");
		}
		prepare_screen();
		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Drawing gui : ");
		}
		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[(size_t)(account->tuto_state)].draw_self();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Render screen : ");
		}
		render_screen(true);
		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Event loop : ");
			a--;
		}
		
		if (SDL_PollEvent(&event) == 1)
		{
			if (a >= 1)
			{
				write_in_log("DONE\n");
				write_in_log("Check SDL/OpenGL : ");
				printOpenGLError();
				printSDLError();
				write_in_log("No error\n");
				a--;
			}
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				menu_quit(t_data(2, &gui, &play));
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if ((size_t)(account->tuto_state) < gui_tutorial.size() && gui_tutorial[(size_t)(account->tuto_state)].object_list.size())
					gui_tutorial[(size_t)(account->tuto_state)].click(&event);
				else
					gui.click(&event);
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);

		}
	}
	account->recharge_actor();
}

#include "taag.h"

/*
TODO :
faire un menu start, et un menu play
*/

#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
		return (0);


	//generate_charset_tileset();
	//generate_animation_tileset();
	//generate_node_file();

	bool play = true;

	if (play == true)
	{
		window_initialisation(argv[0]);
		read_tileset();
		init_actions();
		read_spell();

		menu_start();
	}

	return (0);
}

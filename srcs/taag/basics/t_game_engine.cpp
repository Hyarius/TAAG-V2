#include "taag.h"

s_game_engine::s_game_engine()
{

}

s_game_engine::s_game_engine(string p_path)
{
	board = t_game_board(p_path);
	set_game_engine(&(this->board));
}
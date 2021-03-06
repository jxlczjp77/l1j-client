#include "draw_maint_til.h"
#include "globals.h"
#include "resources/prepared_graphics.h"
#include "resources/tile.h"
#include "sdl_user.h"
#include "widgets/sdl_text_button.h"
#include "widgets/sdl_widget.h"

draw_maint_til::draw_maint_til(sdl_user *self)
	: sdl_drawmode(self)
{
	draw_mtx = SDL_CreateMutex();
	owner->game_music.change_music("sound/music0.mp3");
	
	num_gfx = 0;
	gfx = 0;
	
	num_widgets = 1;
	
	widgets = new sdl_widget*[num_widgets];
	widgets[0] = new sdl_text_button("Return", 320, 255, owner, 
		(funcptr*)new dam_ptr(owner, DRAWMODE_ADMIN_MAIN));
	widgets[0]->set_key_focus(true);
	
	tileset = new tile;
	background = 23;
	tile_num = 0;
	cur_tile = 0;

	tileset->delay_load(background, self);
	update_tile();
}

void draw_maint_til::key_press(SDL_KeyboardEvent *button)
{
	sdl_drawmode::key_press(button);
	if (button->type == SDL_KEYDOWN)
	{
		switch(button->keysym.sym)
		{
			case SDLK_LEFT:
				if (background > 0)
				{
					background--;
					if (tileset != 0)
					{
						delete tileset;
						tileset = 0;
					}
					tileset = new tile;
					tileset->delay_load(background, owner);
					update_tile();
				}
				break;
			case SDLK_RIGHT:
				background++;
				if (tileset != 0)
				{
					delete tileset;
					tileset = 0;
				}
				tileset = new tile;
				tileset->delay_load(background, owner);
				update_tile();
				break;
			case SDLK_UP:
				if ((tile_num + 1) < tileset->get_amnt())
				{
					tile_num++;
					update_tile();
				}
				break;
			case SDLK_DOWN:
				if (tile_num > 0)
				{
					tile_num--;
					update_tile();
				}
				break;
			default:
				break;
		}
	}
}

bool draw_maint_til::quit_request()
{
	//owner->stop_thread = true;
	return true;
}

bool draw_maint_til::mouse_leave()
{
	return false;
}

draw_maint_til::~draw_maint_til()
{
	delete tileset;
	tileset = 0;
}

//updates the graphic to reflect the current tile
void draw_maint_til::update_tile()
{
	cur_tile = tileset->get_tile_left(tile_num);
}

void draw_maint_til::draw(SDL_Surface *display)
{
	while (SDL_mutexP(draw_mtx) == -1) {};
	SDL_FillRect(display, NULL, 0);
	sdl_drawmode::draw(display);
	char temp[50];
	memset(temp, 0, 50);
	if (tileset != 0)
	{
		if (cur_tile != 0)
		{
			cur_tile->draw(display);
			sprintf(temp, "Using tileset %d.til, tile %d", background, tile_num);
		}
		else
		{
			update_tile();
			sprintf(temp, "Tileset %d.til, tile %d invalid?", background, tile_num);
		}
	}
	else
	{
		sprintf(temp, "Tileset %d.til does not exist", background);
	}
	lineage_font.draw(display, 320, 240, temp, 0x7392);
	SDL_mutexV(draw_mtx);
}
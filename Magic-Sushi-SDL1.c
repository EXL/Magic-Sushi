/*
 * About:
 *   Port the "Magic Sushi" game from MTK OS to SDL1 library and Web using Emscripten.
 *
 * Author:
 *   EXL, nehochupechatat, OldPhonePreservation
 *
 * License:
 *   MIT
 *
 * History:
 *   15-Sep-2022: Created initial draft/demo version.
 *
 * Compile commands:
 *   $ clear && clear && gcc Magic-Sushi-SDL1.c -o Magic-Sushi-SDL1 \
 *       -lSDL -lSDL_mixer && strip -s Magic-Sushi-SDL1 && ./Magic-Sushi-SDL1
 */

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 320
#define WINDOW_BPP 16

typedef SDL_bool BOOL;
typedef SDL_bool MMI_BOOL;
typedef Uint8 U8;
typedef Uint16 U16;
typedef Sint16 S16;
typedef Sint32 S32;
#define TRUE SDL_TRUE
#define FALSE SDL_FALSE
#define MMI_TRUE SDL_TRUE
#define MMI_FALSE SDL_FALSE

static SDL_bool app_exec_flag = SDL_FALSE;
static SDL_Surface *screen = NULL;

typedef struct {
	BOOL is_gameover;
	BOOL is_new_game;
	U8 game_level;
	U16 game_grade;
	U8 timer_elapse;
	U8 click_mode;
	U8 selected;
	U8 level;
	U8 bonus;
	BOOL special;
	U8 special_x;
	U8 special_y;
	U8 old_x;
	U8 old_y;
	S16 remainder;
	U8 check;
	U8 total_time;
	U8 remain_time;
	U16 tick;
	U8 animation_step;
	U8 animation_mode;
	U8 moving;

	U8 last_key;
	U8 cursor_x;
	U8 cursor_y;

	U8 softkey_state;

	SDL_Surface *background;
	SDL_Surface *foreground;

	U8 map[8][8];
	U8 i_map[8][8];
	U8 type_num;
	S16 solution_start_vertical[8][4];
	S16 solution_end_vertical[8][4];
	S16 solution_start_horizontal[8][4];
	S16 solution_end_horizontal[8][4];

	S32 background_midi;
	S32 move_midi;
	S32 select_midi;

	MMI_BOOL is_right_arrow_pressed;
	MMI_BOOL is_frame_need_updated;
} gx_magicsushi_context_struct;

gx_magicsushi_context_struct g_gx_magicsushi_context = {
	FALSE, /* is_gameover */
	TRUE,  /* is_new_game */
	0,     /* game_level */
	0,     /* game_grade */
	100,   /* timer_elapse */
	0,     /* click_mode */
	0,     /* selected */
	1,     /* level */
	0,     /* bonus */
	0,     /* special */
	0,     /* special x */
	0,     /* special y */
	0,     /* old x */
	0,     /* old x */
	15,    /* remainder */
	0,     /* check */
	50,    /* total time */
	50,    /* remain time */
	0,     /* tick */
	0,     /* animation_step */
	0,     /* animation_mode */
	0,     /* moving */
	0,     /* last key */
	0,     /* cursorx */
	0,     /* cursory */
	0      /* SOFTKEY STATE */
};

static void mmi_gx_magicsushi_init_game(void) {
	U8 i;

	g_gx_magicsushi_context.click_mode = 0;
	g_gx_magicsushi_context.level = 1;
	g_gx_magicsushi_context.bonus = 1;
	g_gx_magicsushi_context.remainder = 50;
	g_gx_magicsushi_context.total_time = 50;
	g_gx_magicsushi_context.remain_time = 50;
	g_gx_magicsushi_context.tick = 0;
	g_gx_magicsushi_context.type_num = 7;
	g_gx_magicsushi_context.animation_step = 0;
	g_gx_magicsushi_context.selected = 0;
	g_gx_magicsushi_context.moving = 1;
	g_gx_magicsushi_context.special = 0;
	g_gx_magicsushi_context.game_grade = 0;
	g_gx_magicsushi_context.check = 0;
	g_gx_magicsushi_context.cursor_x = 0;
	g_gx_magicsushi_context.cursor_y = 0;
	g_gx_magicsushi_context.last_key = 0;
	g_gx_magicsushi_context.special_x = 0;
	g_gx_magicsushi_context.special_y = 0;
	g_gx_magicsushi_context.is_right_arrow_pressed = MMI_FALSE;
	g_gx_magicsushi_context.is_frame_need_updated = MMI_FALSE;

	for (i = 0; i < 8; i++) {
		g_gx_magicsushi_context.solution_start_horizontal[i][0] = -1;
		g_gx_magicsushi_context.solution_start_vertical[i][0] = -1;
	}
	mmi_gx_magicsushi_gen_map();
}

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "SDL_Init error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		fprintf(stderr, "SDL_Init error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	while (!app_exec_flag) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				app_exec_flag = SDL_TRUE;
				break;
			case SDL_QUIT:
				app_exec_flag = SDL_TRUE;
				break;
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			default:
				break;
			}
		}

		SDL_Delay(100);
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}

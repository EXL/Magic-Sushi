/*
 * About:
 *   Port the "Magic Sushi" game from MTK OS to SDL2 library and Web using Emscripten.
 *
 * Author:
 *   EXL, nehochupechatat, OldPhonePreservation
 *
 * License:
 *   MIT
 *
 * History:
 *   16-Sep-2022: Implemented engine + shell concept.
 *   15-Sep-2022: Created initial draft/demo version.
 *
 * Compile commands:
 *   $ clear && clear && gcc Magic-Sushi-SDL2.c Magic-Sushi-Engine.c -o Magic-Sushi-SDL2 \
 *       -lSDL2 -lSDL2_image -lSDL2_mixer && strip -s Magic-Sushi-SDL2 && ./Magic-Sushi-SDL2
 *
 * More information:
 *   https://www.youtube.com/watch?v=Mmv73Kck6Co
 */

#include "Magic-Sushi-Head.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>

#include <time.h>

typedef enum MUSIC_TRACKS {
	MUSIC_BACKGROUND,
	MUSIC_BACKGROUND_LOWCOST,
	MUSIC_CRASH,
	MUSIC_GAMEOVER,
	MUSIC_MAX
} MUSIC_TRACK;
static Mix_Music *music_tracks[MUSIC_MAX] = { NULL };
static Sint32 volume_old = -1;

static SDL_Texture *textures[TEXTURE_MAX] = { NULL };

static SDL_bool exit_main_loop = SDL_FALSE;
static SDL_Renderer *render = NULL;

static SDL_Rect clip_stack[10] = { { 0, 0, 0, 0} };
static Sint32 sp = -1;

static void Music_Load(void) {
	// TODO: Load Music
#if 0
	music_tracks[MUSIC_BACKGROUND] = Mix_LoadMUS("assets/GAME_F1RACE_BGM.ogg");
	music_tracks[MUSIC_BACKGROUND_LOWCOST] = Mix_LoadMUS("assets/GAME_F1RACE_BGM_LOWCOST.ogg");
	music_tracks[MUSIC_CRASH] = Mix_LoadMUS("assets/GAME_F1RACE_CRASH.ogg");
	music_tracks[MUSIC_GAMEOVER] = Mix_LoadMUS("assets/GAME_F1RACE_GAMEOVER.ogg");
#endif
}

static void Music_Play(MUSIC_TRACK track, Sint32 loop) {
	Mix_PlayMusic(music_tracks[track], loop);
}

static void Music_Unload(void) {
	int i = 0;
	for (; i < MUSIC_MAX; ++i)
		if (music_tracks[i])
			Mix_FreeMusic(music_tracks[i]);
}

static void Texture_Create_Bitmap(const char *filepath, TEXTURE texture_id) {
	SDL_Surface *bitmap = IMG_Load(filepath);
	textures[texture_id] = SDL_CreateTextureFromSurface(render, bitmap);
	SDL_FreeSurface(bitmap);
}

static void Texture_Load(void) {
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_0.png", IMG_ID_GX_MAGICSUSHI_NUMBER_0);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_1.png", IMG_ID_GX_MAGICSUSHI_NUMBER_1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_2.png", IMG_ID_GX_MAGICSUSHI_NUMBER_2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_3.png", IMG_ID_GX_MAGICSUSHI_NUMBER_3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_4.png", IMG_ID_GX_MAGICSUSHI_NUMBER_4);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_5.png", IMG_ID_GX_MAGICSUSHI_NUMBER_5);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_6.png", IMG_ID_GX_MAGICSUSHI_NUMBER_6);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_7.png", IMG_ID_GX_MAGICSUSHI_NUMBER_7);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_8.png", IMG_ID_GX_MAGICSUSHI_NUMBER_8);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_9.png", IMG_ID_GX_MAGICSUSHI_NUMBER_9);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_null.png", IMG_ID_GX_MAGICSUSHI_TYPE_NULL);
	Texture_Create_Bitmap("Assets/gx_magicsushi_select.png", IMG_ID_GX_MAGICSUSHI_SELECTED);
	Texture_Create_Bitmap("Assets/gx_magicsushi_progress.png", IMG_ID_GX_MAGICSUSHI_PROGRESS);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_1.png", IMG_ID_GX_MAGICSUSHI_TYPE_0);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_2.png", IMG_ID_GX_MAGICSUSHI_TYPE_1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_3.png", IMG_ID_GX_MAGICSUSHI_TYPE_2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_4.png", IMG_ID_GX_MAGICSUSHI_TYPE_3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_5.png", IMG_ID_GX_MAGICSUSHI_TYPE_4);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_6.png", IMG_ID_GX_MAGICSUSHI_TYPE_5);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_7.png", IMG_ID_GX_MAGICSUSHI_TYPE_6);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_8.png", IMG_ID_GX_MAGICSUSHI_TYPE_7);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_1.png", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_2.png", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_3.png", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_background.png", IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	Texture_Create_Bitmap("Assets/gx_magicsushi_gameover_TimeOut_E.png", IMG_ID_GX_MAGICSUSHI_GAMEOVER);
	Texture_Create_Bitmap("Assets/gx_magicsushi_uplevel.png", IMG_ID_GX_MAGICSUSHI_UPLEVEL);
	Texture_Create_Bitmap("Assets/gx_magicsushi_nomoremove.png", IMG_ID_GX_MAGICSUSHI_NOMOREMOVE);
	Texture_Create_Bitmap("Assets/gx_magicsushi_select1.png", IMG_ID_GX_MAGICSUSHI_CURSOR);
	Texture_Create_Bitmap("Assets/gx_magicsushi_down.png", IMG_ID_GX_MAGICSUSHI_DOWN);
	Texture_Create_Bitmap("Assets/gx_magicsushi_up.png", IMG_ID_GX_MAGICSUSHI_UP);
	Texture_Create_Bitmap("Assets/gx_magicsushi_ScoreBG.png", IMG_ID_GX_MAGICSUSHI_GRADEMAP);
	Texture_Create_Bitmap("Assets/gx_magicsushi_Food.png", IMG_ID_GX_MAGICSUSHI_GOPIC);

	textures[TEXTURE_SCREEN] =
		SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_WIDTH, TEXTURE_HEIGHT);
}

static void Texture_Draw(Sint32 x, Sint32 y, TEXTURE texture_id) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	SDL_QueryTexture(textures[texture_id], NULL, NULL, &r.w, &r.h);
	SDL_RenderCopy(render, textures[texture_id], NULL, &r);
}

static void Texture_Draw_Piece(Sint32 x, Sint32 y, Sint32 w, Sint32 h, TEXTURE texture_id) {
	SDL_Rect r = { x, y, w - x, h - y};
	SDL_RenderCopy(render, textures[texture_id], &r, &r);
}

static void Texture_Unload(void) {
	int i = 0;
	for (; i < TEXTURE_MAX; ++i)
		if (textures[i])
			SDL_DestroyTexture(textures[i]);
}

static void mouse_handler(SDL_MouseButtonEvent event, EVENT mouse) {
	mmi_pen_point_struct pos = { event.x, event.y };
	switch (mouse) {
		case MOUSE_EVENT_DOWN:
			mmi_gx_magicsushi_pen_down_hdlr(pos);
			break;
		case MOUSE_EVENT_UP:
			mmi_gx_magicsushi_pen_up_hdlr(pos);
			break;
		default:
			break;
	}
}


static void mouse_motion_handler(SDL_MouseMotionEvent event) {
	mmi_pen_point_struct pos = { event.x, event.y };
	mmi_gx_magicsushi_pen_move_hdlr(pos);
}

static void key_handler(S32 key, EVENT keydown) {
	switch (keydown) {
		case KEY_EVENT_DOWN:
			switch (key) {
				case SDLK_KP_5:
				case SDLK_RETURN:
				case SDLK_SPACE:
					mmi_gx_magicsushi_key_5_release();
					break;
				case SDLK_RIGHT:
				case SDLK_KP_6:
					mmi_gx_magicsushi_key_6_down();
					break;
			}
			break;
		case KEY_EVENT_UP:
			switch (key) {
				case SDLK_UP:
				case SDLK_KP_2:
					mmi_gx_magicsushi_key_2_release();
					break;
				case SDLK_LEFT:
				case SDLK_KP_4:
					mmi_gx_magicsushi_key_4_release();
					break;
				case SDLK_RIGHT:
				case SDLK_KP_6:
					mmi_gx_magicsushi_key_6_release();
					break;
				case SDLK_DOWN:
				case SDLK_KP_8:
					mmi_gx_magicsushi_key_8_release();
					break;
			}
			break;
		default:
			break;
	}
}

static void main_loop_step(SDL_Texture *texture) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit_main_loop = SDL_TRUE;
				break;
			case SDL_KEYDOWN:
				key_handler(event.key.keysym.sym, KEY_EVENT_DOWN);
				break;
			case SDL_KEYUP:
				key_handler(event.key.keysym.sym, KEY_EVENT_UP);
				break;
			case SDL_MOUSEMOTION:
				mouse_motion_handler(event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_handler(event.button, MOUSE_EVENT_DOWN);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_handler(event.button, MOUSE_EVENT_UP);
				break;
		}
	}
	SDL_SetRenderTarget(render, texture);

	mmi_gx_magicsushi_cyclic_timer();

	SDL_SetRenderTarget(render, NULL);
	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = WINDOW_WIDTH;
	rectangle.h = WINDOW_HEIGHT;
	SDL_RenderCopy(render, texture, &rectangle, NULL);
	SDL_RenderPresent(render);
}

/* ==================================================== STUBS ======================================================= */

void GFX_PLAY_SOUND_EFFECTS_MIDI(S32 music_id) {
	fprintf(stderr, "ENTER: GFX_PLAY_SOUND_EFFECTS_MIDI: %d.\n", music_id);
}

void GFX_STOP_SOUND_EFFECTS_MIDI(S32 music_id) {
	fprintf(stderr, "ENTER: GFX_STOP_SOUND_EFFECTS_MIDI: %d.\n", music_id);
}

void GFX_PLAY_BACKGROUND_SOUND(S32 music_id) {
	fprintf(stderr, "ENTER: GFX_PLAY_BACKGROUND_SOUND: %d.\n", music_id);
}

void GFX_STOP_BACKGROUND_SOUND(S32 music_id) {
	fprintf(stderr, "ENTER: GFX_STOP_BACKGROUND_SOUND %d.\n", music_id);
}

void mmi_gfx_draw_gameover_screen(S32 gameover_id, S32 field_id, S32 pic_id, U16 grade) {
	fprintf(stderr, "ENTER: mmi_gfx_draw_gameover_screen: %d %d %d %hu.\n", gameover_id, field_id, pic_id, grade);
}

void gdi_layer_push_clip(void) {
//	fprintf(stderr, "ENTER: gdi_layer_push_clip: %d.\n", sp);
	sp++;
	SDL_RenderGetClipRect(render, &clip_stack[sp]);
}

void gdi_layer_pop_clip(void) {
//	fprintf(stderr, "ENTER: gdi_layer_pop_clip: %d.\n", sp);
	sp--;
	if (sp < 0)
		SDL_RenderSetClipRect(render, NULL);
	else
		SDL_RenderSetClipRect(render, &clip_stack[sp]);
}

void gdi_layer_set_clip(S32 x, S32 y, S32 w, S32 h) {
//	fprintf(stderr, "ENTER: gdi_layer_set_clip %d %d %d %d.\n", x, y, w, h);
	SDL_Rect r = { x, y, w - x, h - y };
	SDL_RenderSetClipRect(render, &r);
}

void gdi_layer_clear_background(U32 c) {
//	fprintf(stderr, "ENTER: gdi_layer_clear_background %u.\n", c);
	SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
	SDL_RenderClear(render);
}

void gdi_layer_set_active(gdi_handle layer) {
//	fprintf(stderr, "ENTER: gdi_layer_set_active.\n");
}

void gdi_image_draw_id(S32 x, S32 y, TEXTURE texture_id) {
//	fprintf(stderr, "ENTER: gdi_image_draw_id %d %d %d.\n", x, y, texture_id);
	Texture_Draw(x, y, texture_id);
}

void gdi_draw_solid_rect(S32 x, S32 y, S32 w, S32 h, U32 c) {
//	fprintf(stderr, "ENTER: gdi_draw_solid_rect %d %d %d %d %u.\n", x, y, w, h, c);
	SDL_Rect r = { x, y, w - x, h - y };
	if (c == GDI_COLOR_TRANSPARENT) {
		// TODO: Damn! This hack is so ugly. UGLY!!1
		Texture_Draw_Piece(x - 2, y - 2, w + 2, h + 2, IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	} else {
		switch (c) {
			case GDI_COLOR_RED:
				SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
				break;
			case GDI_COLOR_GREEN:
				SDL_SetRenderDrawColor(render, 0, 255, 0, 0);
				break;
			case GDI_COLOR_BLUE:
				SDL_SetRenderDrawColor(render, 0, 0, 255, 0);
				break;
		}
		SDL_RenderFillRect(render, &r);
	}
}

/* ================================================================================================================== */

int main(SDL_UNUSED int argc, SDL_UNUSED char *argv[]) {
	srand(time(0));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "SDL_Init Error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *window = SDL_CreateWindow("Magic Sushi",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Surface *icon = SDL_LoadBMP("Assets/gx_magicsushi_icon.bmp");
	if (icon == NULL)
		fprintf(stderr, "SDL_LoadBMP Error: %s.\n", SDL_GetError());
	else {
		SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 128, 255, 128)); // Icon transparent mask.
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (render == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s.\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	Texture_Load();

	int result = Mix_Init(MIX_INIT_OGG);
	if (result != MIX_INIT_OGG) {
		fprintf(stderr, "Mix_Init Error: %s.\n", Mix_GetError());
		return EXIT_FAILURE;
	}
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 4096) == -1) {
		fprintf(stderr, "Mix_OpenAudio Error: %s.\n", Mix_GetError());
		return EXIT_FAILURE;
	}

	Music_Load();

	SDL_SetRenderTarget(render, textures[TEXTURE_SCREEN]);
	SDL_RenderClear(render);

	mmi_gx_magicsushi_enter_game();

	SDL_SetRenderTarget(render, NULL);

	while (!exit_main_loop) {
		main_loop_step(textures[TEXTURE_SCREEN]);
		SDL_Delay(FPS_COUNTER); // ~10 fps.
	}

	Mix_CloseAudio();
	Music_Unload();
	Texture_Unload();

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

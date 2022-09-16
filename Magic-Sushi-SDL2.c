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
 *   15-Sep-2022: Created initial draft/demo version.
 *
 * Compile commands:
 *   $ clear && clear && gcc Magic-Sushi-SDL2.c Magic-Sushi-Engine.c -o Magic-Sushi-SDL2 \
 *       -lSDL2 -lSDL2_mixer && strip -s Magic-Sushi-SDL2 && ./Magic-Sushi-SDL2
 */

#include "Magic-Sushi-Head.h"

#include <SDL2/SDL.h>
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
	SDL_Surface *bitmap = SDL_LoadBMP(filepath);
	textures[texture_id] = SDL_CreateTextureFromSurface(render, bitmap);
	SDL_FreeSurface(bitmap);
}

static void Texture_Load(void) {
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_0.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_0);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_1.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_2.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_3.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_4.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_4);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_5.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_5);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_6.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_6);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_7.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_7);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_8.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_8);
	Texture_Create_Bitmap("Assets/gx_magicsushi_num_9.bmp", IMG_ID_GX_MAGICSUSHI_NUMBER_9);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_null.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_NULL);
	Texture_Create_Bitmap("Assets/gx_magicsushi_select.bmp", IMG_ID_GX_MAGICSUSHI_SELECTED);
	Texture_Create_Bitmap("Assets/gx_magicsushi_progress.bmp", IMG_ID_GX_MAGICSUSHI_PROGRESS);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_1.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_0);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_2.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_3.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_4.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_5.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_4);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_6.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_5);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_7.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_6);
	Texture_Create_Bitmap("Assets/gx_magicsushi_item_8.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_7);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_1.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC1);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_2.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC2);
	Texture_Create_Bitmap("Assets/gx_magicsushi_magic_3.bmp", IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC3);
	Texture_Create_Bitmap("Assets/gx_magicsushi_background.bmp", IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	Texture_Create_Bitmap("Assets/gx_magicsushi_gameover_TimeOut_E.bmp", IMG_ID_GX_MAGICSUSHI_GAMEOVER);
	Texture_Create_Bitmap("Assets/gx_magicsushi_uplevel.bmp", IMG_ID_GX_MAGICSUSHI_UPLEVEL);
	Texture_Create_Bitmap("Assets/gx_magicsushi_nomoremove.bmp", IMG_ID_GX_MAGICSUSHI_NOMOREMOVE);
	Texture_Create_Bitmap("Assets/gx_magicsushi_select1.bmp", IMG_ID_GX_MAGICSUSHI_CURSOR);
	Texture_Create_Bitmap("Assets/gx_magicsushi_down.bmp", IMG_ID_GX_MAGICSUSHI_DOWN);
	Texture_Create_Bitmap("Assets/gx_magicsushi_up.bmp", IMG_ID_GX_MAGICSUSHI_UP);
	Texture_Create_Bitmap("Assets/gx_magicsushi_ScoreBG.bmp", IMG_ID_GX_MAGICSUSHI_GRADEMAP);
	Texture_Create_Bitmap("Assets/gx_magicsushi_Food.bmp", IMG_ID_GX_MAGICSUSHI_GOPIC);

	textures[TEXTURE_SCREEN] =
		SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_WIDTH, TEXTURE_HEIGHT);
}

static void Texture_Draw(Sint32 x, Sint32 y, TEXTURE texture_id) {
	SDL_Rect rectangle;
	rectangle.x = x;
	rectangle.y = y;
	SDL_QueryTexture(textures[texture_id], NULL, NULL, &rectangle.w, &rectangle.h);
	SDL_RenderCopy(render, textures[texture_id], NULL, &rectangle);
}

static void Texture_Unload(void) {
	int i = 0;
	for (; i < TEXTURE_MAX; ++i)
		if (textures[i])
			SDL_DestroyTexture(textures[i]);
}

static void main_loop_step(SDL_Texture *texture) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit_main_loop = SDL_TRUE;
				break;
			case SDL_KEYDOWN:
				fprintf(stderr, "PushKey\n");
				break;
			case SDL_KEYUP:
				fprintf(stderr, "ReleaseKey\n");
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
	fprintf(stderr, "PLAY MUSIC %d.\n", music_id);
}

void GFX_STOP_SOUND_EFFECTS_MIDI(S32 music_id) {
	fprintf(stderr, "STOP MUSIC %d.\n", music_id);
}

void GFX_PLAY_BACKGROUND_SOUND(S32 music_id) {
	fprintf(stderr, "PLAY MUSIC %d.\n", music_id);
}

void GFX_STOP_BACKGROUND_SOUND(S32 music_id) {
	fprintf(stderr, "STOP MUSIC %d.\n", music_id);
}

void mmi_gfx_draw_gameover_screen(S32 gameover_id, S32 field_id, S32 pic_id, U16 grade) {
	fprintf(stderr, "DRAW GAMEOVER %d %d %d %hu.\n", gameover_id, field_id, pic_id, grade);
}

void gdi_layer_push_clip(void) {
	fprintf(stderr, "ENTER: gdi_layer_push_clip.\n");
}

void gdi_layer_pop_clip(void) {
	fprintf(stderr, "ENTER: gdi_layer_pop_clip.\n");
}

void gdi_layer_set_clip(S32 x, S32 y, S32 w, S32 h) {
	fprintf(stderr, "ENTER: gdi_layer_set_clip %d %d %d %d.\n", x, y, w, h);
	SDL_Rect r = { x, y, w, h };
	SDL_RenderSetClipRect(render, &r);
}

void gdi_layer_clear_background(U32 c) {
	fprintf(stderr, "ENTER: gdi_layer_clear_background %u.\n", c);
	SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
	SDL_RenderClear(render);
}

void gdi_layer_set_active(gdi_handle layer) {
	fprintf(stderr, "ENTER: gdi_layer_set_active.\n");
}

void gdi_image_draw_id(S32 x, S32 y, TEXTURE texture_id) {
	fprintf(stderr, "ENTER: gdi_image_draw_id %d %d %d.\n", x, y, texture_id);
	Texture_Draw(x, y, texture_id);
}

void gdi_draw_solid_rect(S32 x, S32 y, S32 w, S32 h, U32 c) {
	fprintf(stderr, "ENTER: gdi_draw_solid_rect %d %d %d %d %u.\n", x, y, w, h, c);
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
		SDL_Delay(100); // 10 fps.
	}

	Mix_CloseAudio();
	Music_Unload();
	Texture_Unload();

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

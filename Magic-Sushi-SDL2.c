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

	//TODO: F1Race_Cyclic_Timer();

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
}

void gdi_layer_clear_background(U32 c) {
	fprintf(stderr, "ENTER: gdi_layer_clear_background %u.\n", c);
}

void gdi_layer_set_active(gdi_handle layer) {
	fprintf(stderr, "ENTER: gdi_layer_set_active.\n");
}

void gdi_image_draw_id(S32 x, S32 y, TEXTURE texture_id) {
	fprintf(stderr, "ENTER: gdi_image_draw_id %d %d %d.\n", x, y, texture_id);
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

	// TODO: Window Icon!!!
#if 0
	SDL_Surface *icon = SDL_LoadBMP("assets/GAME_F1RACE_ICON.bmp");
	if (icon == NULL)
		fprintf(stderr, "SDL_LoadBMP Error: %s.\n", SDL_GetError());
	else {
		SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 36, 227, 113)); // Icon transparent mask.
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}
#endif

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

	//TODO: First Frame!!!

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

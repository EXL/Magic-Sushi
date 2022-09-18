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
 *   17-Sep-2022: Implemented SDL1 version based on SDL2 version.
 *   15-Sep-2022: Created initial draft/demo version.
 *
 * Compile commands:
 *   $ clear && clear && gcc Magic-Sushi-SDL1.c Magic-Sushi-Engine.c -o Magic-Sushi-SDL1 \
 *       -lSDL -lSDL_image -lSDL_mixer && strip -s Magic-Sushi-SDL1 && ./Magic-Sushi-SDL1
 */

#include "Magic-Sushi-Wrapper.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <time.h>

static Mix_Music *music_tracks[MUSIC_MAX] = { NULL };
static Sint32 volume_music_old = -1;

static Mix_Chunk *sound_effects[SOUND_MAX] = { NULL };
static Sint32 volume_channel_old = -1;

static SDL_Surface *back_screen = NULL;
static SDL_Surface *textures[TEXTURE_MAX] = { NULL };

static SDL_bool exit_main_loop = SDL_FALSE;
static SDL_Rect clip_stack[10] = { { 0, 0, 0, 0} };
static Sint32 sp = -1;

#ifdef __EMSCRIPTEN__
typedef struct {
	SDL_Surface *texture;
} CONTEXT_EMSCRIPTEN;
#endif

#ifdef _EZX
static Uint32 time_a = 0;
static Uint32 time_b = 0;
static Uint32 time_since_last_frame = 0;
#endif

static void Music_Sound_Load(void) {
#ifndef _EZX
	sound_effects[SOUND_SELECT] = Mix_LoadWAV("Assets/gx_magicsushi_select.ogg");
#endif
	sound_effects[SOUND_MOVE] = Mix_LoadWAV("Assets/gx_magicsushi_move.ogg");

	music_tracks[MUSIC_BACKGROUND] = Mix_LoadMUS("Assets/gx_magicsushi_bgm.ogg");
	music_tracks[MUSIC_GAMEOVER] = Mix_LoadMUS("Assets/gx_magicsushi_timeout.ogg");
}

static void Music_Play(MUSIC_TRACK track, Sint32 loop) {
	Mix_PlayMusic(music_tracks[track], loop);
}

static void Sound_Play(SOUND_EFFECT track, Sint32 loop) {
	Mix_PlayChannel(MIX_SFX_CHANNEL, sound_effects[track], loop);
}

static void Music_Sound_Unload(void) {
	int i = 0;
	for (; i < MUSIC_MAX; ++i)
		if (music_tracks[i])
			Mix_FreeMusic(music_tracks[i]);
	for (i = 0; i < SOUND_MAX; ++i)
		if (sound_effects[i])
			Mix_FreeChunk(sound_effects[i]);
}

static void Texture_Create_Bitmap(const char *filepath, TEXTURE texture_id) {
	SDL_Surface *bitmap = IMG_Load(filepath);
	if (bitmap == NULL)
		E("WARNING: Cannot open '%s' file!\n", filepath);
	textures[texture_id] = bitmap;
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

	textures[TEXTURE_SCREEN] = back_screen;
}

static void Texture_Draw(Sint32 x, Sint32 y, TEXTURE texture_id) {
	SDL_Rect r = { x, y, textures[texture_id]->w, textures[texture_id]->h };
	SDL_BlitSurface(textures[texture_id], NULL, back_screen, &r);
}

static void Texture_Draw_Back(Sint32 x, Sint32 y, Sint32 w, Sint32 h, TEXTURE texture_id) {
	SDL_Rect r = { x, y, w - x, h - y};
	SDL_BlitSurface(textures[texture_id], &r, back_screen, &r);
}

static void Texture_Draw_Piece(Sint32 x, Sint32 y, Sint32 w, Sint32 h, Sint32 xx, Sint32 yy, TEXTURE texture_id) {
	SDL_Rect r = { x, y, w - x, h - y};
	SDL_Rect d = { xx, yy, r.w, r.h };
	SDL_BlitSurface(textures[texture_id], &r, back_screen, &d);
}

static void Texture_Unload(void) {
	int i = 0;
	for (; i < TEXTURE_MAX; ++i)
		if (textures[i])
			SDL_FreeSurface(textures[i]);
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
				case SDLK_5:
				case SDLK_KP5:
				case SDLK_RETURN:
				case SDLK_SPACE:
					mmi_gx_magicsushi_key_5_release();
					break;
				case SDLK_RIGHT:
				case SDLK_6:
				case SDLK_KP6:
					mmi_gx_magicsushi_key_6_down();
					break;
#ifdef _EZX
				case SDLK_PAUSE:
#endif
				case SDLK_m:
				case SDLK_7:
				case SDLK_KP7:
					if (volume_music_old == -1 && volume_channel_old == -1) {
						volume_music_old = Mix_VolumeMusic(0);
						volume_channel_old = Mix_Volume(MIX_SFX_CHANNEL, 0);
					} else {
						Mix_VolumeMusic(volume_music_old);
						Mix_Volume(MIX_SFX_CHANNEL, volume_channel_old);
						volume_music_old = -1;
						volume_channel_old = -1;
					}
					break;
				case SDLK_ESCAPE:
					exit_main_loop = SDL_TRUE;
					break;
			}
			break;
		case KEY_EVENT_UP:
			switch (key) {
				case SDLK_UP:
				case SDLK_2:
				case SDLK_KP8:
					mmi_gx_magicsushi_key_2_release();
					break;
				case SDLK_LEFT:
				case SDLK_4:
				case SDLK_KP4:
					mmi_gx_magicsushi_key_4_release();
					break;
				case SDLK_RIGHT:
				case SDLK_6:
				case SDLK_KP6:
					mmi_gx_magicsushi_key_6_release();
					break;
				case SDLK_DOWN:
				case SDLK_8:
				case SDLK_KP2:
					mmi_gx_magicsushi_key_8_release();
					break;
			}
			break;
		default:
			break;
	}
}

static U32 Get_Color(U32 c) {
	U32 color;
	switch (c) {
		default:
			color = SDL_MapRGB(back_screen->format, 255, 255, 255);
			break;
		case bg_color:
			color = SDL_MapRGB(back_screen->format, 255, 244, 212);
			break;
		case fg_color:
			color = SDL_MapRGB(back_screen->format, 255, 189, 8);
			break;
		case GDI_COLOR_RED:
			color = SDL_MapRGB(back_screen->format, 255, 0, 0);
			break;
		case GDI_COLOR_GREEN:
			color = SDL_MapRGB(back_screen->format, 0, 255, 0);
			break;
		case GDI_COLOR_BLUE:
			color = SDL_MapRGB(back_screen->format, 0, 0, 255);
			break;
	}
	SDL_SetColorKey(back_screen, 0, color);
	return color;
}

/* ==================================================== STUBS ======================================================= */

void GFX_PLAY_SOUND_EFFECTS_MIDI(S32 music_id) {
	D("ENTER: %s: %d.\n", __func__, music_id);
	if (!mmi_gx_magicsushi_on_gameover_screen())
		Sound_Play(music_id, 0);
}

void GFX_STOP_SOUND_EFFECTS_MIDI(S32 music_id) {
	D("ENTER: %s: %d.\n", __func__, music_id);
	Mix_HaltChannel(MIX_SFX_CHANNEL);
}

void GFX_PLAY_BACKGROUND_SOUND(S32 music_id) {
	D("ENTER: %s: %d.\n", __func__, music_id);
	if (music_id == MUSIC_BACKGROUND)
		Music_Play(music_id, -1);
	else if (music_id == MUSIC_GAMEOVER)
		Music_Play(music_id, 0);
}

void GFX_STOP_BACKGROUND_SOUND(S32 music_id) {
	D("ENTER: %s: %d.\n", __func__, music_id);
	Mix_HaltMusic();
}

void mmi_gfx_draw_gameover_screen(S32 gameover_id, S32 field_id, S32 pic_id, U16 grade) {
	D("ENTER: %s: %d %d %d %hu.\n", __func__, gameover_id, field_id, pic_id, grade);
#if defined(_320x480)
	gdi_image_draw_id(83, 80, gameover_id);
	gdi_image_draw_id(65, 150, field_id);
	gdi_image_draw_id(65, 240, pic_id);
	gdi_draw_solid_rect(70, 155, 257, 212, fg_color);
	Texture_Draw_Piece(16, 45, 58, 63, 123, 175, IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	mmi_gx_magicsushi_draw_digit(195, 181, grade);
#elif defined(_240x320)
	gdi_image_draw_id(45, 50, gameover_id);
	gdi_image_draw_id(50, 100, field_id);
	gdi_image_draw_id(50, 190, pic_id);
	gdi_draw_solid_rect(55, 105, 187, 162, fg_color);
	Texture_Draw_Piece(14, 25, 50, 38, 75, 125, IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	mmi_gx_magicsushi_draw_digit(155, 127, grade);
#else
#error "Unknown screen resolution, please set it here for Game Over screen!"
#endif
}

void gdi_layer_push_clip(void) {
	D("ENTER: %s: %d.\n", __func__, sp);
	sp++;
	SDL_GetClipRect(back_screen, &clip_stack[sp]);
}

void gdi_layer_pop_clip(void) {
	D("ENTER: %s: %d.\n", __func__, sp);
	sp--;
	if (sp < 0)
		SDL_SetClipRect(back_screen, NULL);
	else
		SDL_SetClipRect(back_screen, &clip_stack[sp]);
}

void gdi_layer_set_clip(S32 x, S32 y, S32 w, S32 h) {
	D("ENTER: %s: %d %d %d %d.\n", __func__, x, y, w, h);
	SDL_Rect r = { x, y, w - x, h - y };
	SDL_SetClipRect(back_screen, &r);
}

void gdi_layer_clear_background(U32 c) {
	D("ENTER: %s: %u.\n", __func__, c);
	SDL_FillRect(back_screen, NULL, Get_Color(c));
}

void gdi_image_draw_id(S32 x, S32 y, TEXTURE texture_id) {
	D("ENTER: %s: %d %d %d.\n", __func__, x, y, texture_id);
	Texture_Draw(x, y, texture_id);
}

void gdi_draw_solid_rect(S32 x, S32 y, S32 w, S32 h, U32 c) {
	D("ENTER: %s: %d %d %d %d %u.\n", __func__, x, y, w, h, c);
	SDL_Rect r = { x, y, w - x, h - y };
	if (c == GDI_COLOR_TRANSPARENT) {
		// TODO: Damn! This hack is so ugly. UGLY!!1
		Texture_Draw_Back(x - 2, y - 2, w + 2, h + 2, IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	} else {
		SDL_FillRect(back_screen, &r, Get_Color(c));
	}
}

void GoBackHistory(void) {
	D("ENTER: %s: .\n", __func__);
#ifndef __EMSCRIPTEN__
	exit_main_loop = SDL_TRUE;
#endif
}

/* ================================================================================================================== */

static void main_loop_step(SDL_Surface *texture) {
#ifdef _EZX
	Uint32 delta_time = 0;
#endif
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

#ifdef _EZX
	time_b = time_a;
	time_a = SDL_GetTicks();

	if (time_a > time_b) {
		delta_time = time_a - time_b;
	}
	else {
		delta_time = time_b - time_a;
	}
	time_since_last_frame += delta_time;

	if (time_since_last_frame >= 1000 / FPS_EMSCRIPTEN_COUNTER) {
		mmi_gx_magicsushi_cyclic_timer();
		time_since_last_frame = 0;
	}
#else
	mmi_gx_magicsushi_cyclic_timer();
#endif

	SDL_Rect r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_BlitSurface(back_screen, NULL, texture, &r);
	SDL_UpdateRect(texture, r.x, r.y, r.w, r.h);
}

#ifdef __EMSCRIPTEN__
static void main_loop_emscripten(void *arguments) {
	CONTEXT_EMSCRIPTEN *context = arguments;
	main_loop_step(context->texture);
}
#endif

int main(int argc, char *argv[]) {
	srand(time(0));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		E("SDL_Init error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Surface *main_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_HWSURFACE);
	if (main_screen == NULL) {
		E("SDL_SetVideoMode error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_WM_SetCaption("Magic Sushi", NULL);
	SDL_Surface *icon = SDL_LoadBMP("Assets/gx_magicsushi_icon.bmp");
	if (icon == NULL)
		E("SDL_LoadBMP error: %s.\n", SDL_GetError());
	else {
		SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 128, 255, 128)); // Icon transparent mask.
		SDL_WM_SetIcon(icon, NULL);
		SDL_FreeSurface(icon);
	}

	back_screen = SDL_CreateRGBSurface(SDL_HWSURFACE, TEXTURE_WIDTH, TEXTURE_HEIGHT, WINDOW_BPP, 0, 0, 0, 0);
	if (back_screen == NULL) {
		E("SDL_CreateRGBSurface error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	Texture_Load();

	int result = Mix_Init(MIX_INIT_OGG);
	if (result != MIX_INIT_OGG) {
		E("Mix_Init error: %s.\n", Mix_GetError());
		return EXIT_FAILURE;
	}
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 4096) == -1) {
		E("Mix_OpenAudio error: %s.\n", Mix_GetError());
		return EXIT_FAILURE;
	}
	Music_Sound_Load();
	volume_music_old = Mix_VolumeMusic(SDL_MIX_MAXVOLUME / 2);
	volume_channel_old = Mix_Volume(MIX_SFX_CHANNEL, SDL_MIX_MAXVOLUME);

	mmi_gx_magicsushi_enter_game();

#ifndef __EMSCRIPTEN__
	while (!exit_main_loop) {
		main_loop_step(main_screen);
#ifndef _EZX
		SDL_Delay(FPS_COUNTER); // ~10 FPS.
#endif
	}
#else
	CONTEXT_EMSCRIPTEN context;
	context.texture = main_screen;
	emscripten_set_main_loop_arg(main_loop_emscripten, &context, FPS_EMSCRIPTEN_COUNTER, 1); // 10 FPS.
#endif

	Mix_CloseAudio();
	Music_Sound_Unload();
	Texture_Unload();

	SDL_FreeSurface(main_screen);
	SDL_Quit();

	return EXIT_SUCCESS;
}

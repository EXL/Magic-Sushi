#ifndef MAGIC_SUSHI_TYPES_H
#define MAGIC_SUSHI_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define __MMI_GAME_MAGICSUSHI__
#define __MMI_TOUCH_SCREEN__
#define __MMI_GAME_MULTICHANNEL_SOUND__
// #define __MMI_GAME_MULTI_LANGUAGE_SUPPORT__

#define FIX_CLIPPING_HACK
#define FIX_GAMEOVER_HACK
// #define USE_DEBUG_OUTPUT

#ifdef USE_DEBUG_OUTPUT
#define D(format, ...) do { fprintf(stderr, format, __VA_ARGS__); } while (0)
#else
#define D(format, ...)
#endif
#define E(format, ...) do { fprintf(stderr, format, __VA_ARGS__); } while (0)

#if defined(_240x320)
#define __MMI_MAINLCD_240X320__
#define WINDOW_WIDTH                                   (240)
#define WINDOW_HEIGHT                                  (320)
#define TEXTURE_WIDTH                                  (240)
#define TEXTURE_HEIGHT                                 (320)
#elif defined(_320x480)
#define __MMI_MAINLCD_320X480__
#define WINDOW_WIDTH                                   (320)
#define WINDOW_HEIGHT                                  (480)
#define TEXTURE_WIDTH                                  (320)
#define TEXTURE_HEIGHT                                 (480)
#else
#error "Unknown screen resolution, please set it here!"
#endif
// #define __MMI_MAINLCD_320X240__
// #define __MMI_MAINLCD_240X400__
#define WINDOW_BPP                                     (16)

#define FPS_COUNTER                                    (100) // 100 ms, ~10 FPS.
#define FPS_EMSCRIPTEN_COUNTER                         (10)  // 10 FPS.
#define MIX_SFX_CHANNEL                                (-1)

#define __align(x)
#define gdi_handle void *

#define FALSE 0
#define TRUE 1
#define MMI_FALSE 0
#define MMI_TRUE 1

typedef int8_t S8;
typedef uint8_t U8;
typedef _Bool BOOL;
typedef _Bool MMI_BOOL;
typedef int16_t S16;
typedef uint16_t U16;
typedef int32_t S32;
typedef uint32_t U32;
typedef float FLOAT;

typedef struct {
	S32 x;
	S32 y;
} mmi_pen_point_struct;

typedef enum COLORS {
	GDI_COLOR_TRANSPARENT,
	GDI_COLOR_GREEN,
	GDI_COLOR_RED,
	GDI_COLOR_BLUE,
	bg_color,
	fg_color
} COLOR;

typedef enum KEYS {
	KEY_5,
	KEY_IP,
	KEY_2,
	KEY_UP_ARROW,
	KEY_8,
	KEY_DOWN_ARROW,
	KEY_4,
	KEY_LEFT_ARROW,
	KEY_6,
	KEY_RIGHT_ARROW,
	KEY_RSK
} KEY;

typedef enum EVENTS {
	KEY_EVENT_UP,
	KEY_EVENT_DOWN,
	MOUSE_EVENT_MOTION,
	MOUSE_EVENT_UP,
	MOUSE_EVENT_DOWN,
} EVENT;

typedef enum MUSIC_TRACKS {
	MUSIC_BACKGROUND,
	MUSIC_GAMEOVER,
	MUSIC_MAX
} MUSIC_TRACK;

typedef enum SOUND_EFFECTS {
	SOUND_MOVE,
	SOUND_SELECT,
	SOUND_MAX
} SOUND_EFFECT;

typedef enum TEXTURES {
	IMG_ID_GX_MAGICSUSHI_NUMBER_0,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_1,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_2,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_3,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_4,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_5,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_6,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_7,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_8,           // OK
	IMG_ID_GX_MAGICSUSHI_NUMBER_9,           // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_NULL,          // OK
	IMG_ID_GX_MAGICSUSHI_SELECTED,           // OK
	IMG_ID_GX_MAGICSUSHI_PROGRESS,           // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_0,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_1,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_2,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_3,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_4,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_5,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_6,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_7,             // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC1,        // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC2,        // OK
	IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC3,        // OK
	IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND,    // OK
	IMG_ID_GX_MAGICSUSHI_GAMEOVER,           // OK
	IMG_ID_GX_MAGICSUSHI_UPLEVEL,            // OK
	IMG_ID_GX_MAGICSUSHI_NOMOREMOVE,         // OK
	IMG_ID_GX_MAGICSUSHI_CURSOR,             // OK
	IMG_ID_GX_MAGICSUSHI_DOWN,               // OK
	IMG_ID_GX_MAGICSUSHI_UP,                 // OK
	IMG_ID_GX_MAGICSUSHI_GRADEMAP,           // OK
	IMG_ID_GX_MAGICSUSHI_GOPIC,              // OK
	TEXTURE_SCREEN,                          // OK
	TEXTURE_MAX
} TEXTURE;

/* ==================================================== STUBS ======================================================= */

#define mmi_gfx_entry_menu_screen()
#define GFX_CLOSE_SOUND_EFFECTS_MIDI(x)
#define GFX_STOP_DUMMY_BACKGROUND_SOUND()
#define GFX_CLOSE_DUMMY_BACKGROUND_SOUND()
#define GFX_CLOSE_BACKGROUND_SOUND(x)
#define GFX_PLAY_AUDIO_COMPLETE()
#define GFX_OPEN_DUMMY_BACKGROUND_SOUND()
#define GFX_PLAY_DUMMY_BACKGROUND_SOUND()
#define GFX_OPEN_SOUND_EFFECTS_MIDI(a, b, c, d)
#define GFX_OPEN_BACKGROUND_SOUND(x, y, z)

#define mmi_gfx_entry_gameover_screen()
#define SetKeyHandler(x, y, z)
#define gui_cancel_timer(x)
#define gui_start_timer(x, y)
#define gdi_layer_free(x)
#define gdi_layer_set_active(x)
#define gdi_layer_multi_layer_disable()
#define gdi_layer_multi_layer_enable()
#define gdi_layer_get_base_handle(x)
#define gdi_layer_create(a, b, c, d, e)
#define gdi_layer_set_source_key(x, y)
#define gdi_layer_flatten_to_base(a, b, c, d)
#define gdi_layer_blt(a, b, c, d, e, f, g, h) // Blitter function.
#define wgui_register_pen_down_handler(x)
#define wgui_register_pen_up_handler(x)
#define wgui_register_pen_move_handler(x)

extern void GFX_PLAY_SOUND_EFFECTS_MIDI(S32 music_id);
extern void GFX_STOP_SOUND_EFFECTS_MIDI(S32 music_id);
extern void GFX_PLAY_BACKGROUND_SOUND(S32 music_id);
extern void GFX_STOP_BACKGROUND_SOUND(S32 music_id);
extern void mmi_gfx_draw_gameover_screen(S32 gameover_id, S32 field_id, S32 pic_id, U16 grade);

extern void gdi_layer_push_clip(void);
extern void gdi_layer_pop_clip(void);
extern void gdi_layer_set_clip(S32 x, S32 y, S32 w, S32 h);
extern void gdi_layer_clear_background(U32 c);

extern void gdi_image_draw_id(S32 x, S32 y, TEXTURE texture_id);
extern void gdi_draw_solid_rect(S32 x, S32 y, S32 w, S32 h, U32 c);

extern void mmi_gx_magicsushi_enter_game(void);
extern void mmi_gx_magicsushi_cyclic_timer(void);
extern void mmi_gx_magicsushi_key_6_down(void);
extern void mmi_gx_magicsushi_key_2_release(void);
extern void mmi_gx_magicsushi_key_4_release(void);
extern void mmi_gx_magicsushi_key_5_release(void);
extern void mmi_gx_magicsushi_key_6_release(void);
extern void mmi_gx_magicsushi_key_8_release(void);
extern void mmi_gx_magicsushi_pen_down_hdlr(mmi_pen_point_struct pos);
extern void mmi_gx_magicsushi_pen_up_hdlr(mmi_pen_point_struct pos);
extern void mmi_gx_magicsushi_pen_move_hdlr(mmi_pen_point_struct pos);
extern void mmi_gx_magicsushi_draw_digit(S16 x, S16 y, U32 digit);
extern BOOL mmi_gx_magicsushi_on_gameover_screen(void);

extern void GoBackHistory(void); // Looks like exit/quit application function.

/* ================================================================================================================== */

#endif /* !MAGIC_SUSHI_TYPES_H */

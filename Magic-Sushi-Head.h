#ifndef MAGIC_SUSHI_TYPES_H
#define MAGIC_SUSHI_TYPES_H

#include <stdint.h>
#include <stdlib.h>

#define __MMI_GAME_MAGICSUSHI__
#define __MMI_TOUCH_SCREEN__
#define __MMI_GAME_MULTICHANNEL_SOUND__
// #define __MMI_GAME_MULTI_LANGUAGE_SUPPORT__

#define FIX_CLIPPING_HACK
#define FIX_GAMEOVER_HACK

#define __MMI_MAINLCD_240X320__
// #define __MMI_MAINLCD_320X240__
// #define __MMI_MAINLCD_240X400__
// #define __MMI_MAINLCD_320X480__

#define WINDOW_WIDTH                                   (240)
#define WINDOW_HEIGHT                                  (320)
#define TEXTURE_WIDTH                                  (240)
#define TEXTURE_HEIGHT                                 (320)
#define FPS_COUNTER                                    (100) // 10 FPS.

#define __align(x)
#define gdi_handle void * // TODO: Fix that
//#define gdi_act_color_from_rgb(a, r, g, b) { a, r, g, b }

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
typedef float FLOAT; // TODO: maybe double?

typedef struct {
	S32 x;
	S32 y;
} mmi_pen_point_struct;

//typedef struct {
//	U8 a;
//	U8 r;
//	U8 g;
//	U8 b;
//} gdi_color;

#define GDI_COLOR_TRANSPARENT 0
#define GDI_COLOR_GREEN 1
#define GDI_COLOR_RED 2
#define GDI_COLOR_BLUE 3
#define bg_color 4

#define KEY_5 5
#define KEY_IP 5
#define KEY_2 2
#define KEY_UP_ARROW 2
#define KEY_8 8
#define KEY_DOWN_ARROW 8
#define KEY_4 4
#define KEY_LEFT_ARROW 4
#define KEY_6 6
#define KEY_RIGHT_ARROW 6
#define KEY_RSK 11
#define KEY_EVENT_DOWN 1
#define KEY_EVENT_UP 0

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
#define GFX_PLAY_AUDIO_COMPLETE() // TODO: GameOver sound?
#define GFX_OPEN_DUMMY_BACKGROUND_SOUND()
#define GFX_PLAY_DUMMY_BACKGROUND_SOUND()
#define GFX_OPEN_SOUND_EFFECTS_MIDI(a, b, c, d)
#define GFX_OPEN_BACKGROUND_SOUND(x, y, z)

#define mmi_gfx_entry_gameover_screen()
#define SetKeyHandler(x, y, z)
#define gui_cancel_timer(x)
#define gui_start_timer(x, y)
#define gdi_layer_free(x)
#define gdi_layer_multi_layer_disable()
#define gdi_layer_multi_layer_enable()
#define gdi_layer_get_base_handle(x)
#define gdi_layer_create(a, b, c, d, e)
#define gdi_layer_set_source_key(x, y)
#define gdi_layer_flatten_to_base(a, b, c, d)
#define gdi_layer_blt(a, b, c, d, e, f, g, h) // TODO: Blitter.
#define GoBackHistory() // TODO: idk what this.
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
extern void gdi_layer_set_active(gdi_handle layer);

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

/* ================================================================================================================== */

#endif /* !MAGIC_SUSHI_TYPES_H */

#ifndef MAGIC_SUSHI_TYPES_H
#define MAGIC_SUSHI_TYPES_H

#include <stdint.h>

#define __MMI_GAME_MAGICSUSHI__
#define __MMI_TOUCH_SCREEN__
#define __MMI_GAME_MULTICHANNEL_SOUND__
// #define __MMI_GAME_MULTI_LANGUAGE_SUPPORT__

#define __MMI_MAINLCD_240X320__
// #define __MMI_MAINLCD_320X240__
// #define __MMI_MAINLCD_240X400__
// #define __MMI_MAINLCD_320X480__

#define WINDOW_WIDTH                                   (240)
#define WINDOW_HEIGHT                                  (320)
#define TEXTURE_WIDTH                                  (240)
#define TEXTURE_HEIGHT                                 (320)

#define __align(x)
#define gdi_handle void * // TODO: Fix that
#define gdi_act_color_from_rgb(a, r, g, b) { a, r, g, b }

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

typedef struct {
	U8 a;
	U8 r;
	U8 g;
	U8 b;
} gdi_color;

#define GDI_COLOR_TRANSPARENT 0
#define GDI_COLOR_GREEN 1
#define GDI_COLOR_RED 2
#define GDI_COLOR_BLUE 3

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
	IMG_ID_GX_MAGICSUSHI_NUMBER_0,
	IMG_ID_GX_MAGICSUSHI_NUMBER_1,
	IMG_ID_GX_MAGICSUSHI_NUMBER_2,
	IMG_ID_GX_MAGICSUSHI_NUMBER_3,
	IMG_ID_GX_MAGICSUSHI_NUMBER_4,
	IMG_ID_GX_MAGICSUSHI_NUMBER_5,
	IMG_ID_GX_MAGICSUSHI_NUMBER_6,
	IMG_ID_GX_MAGICSUSHI_NUMBER_7,
	IMG_ID_GX_MAGICSUSHI_NUMBER_8,
	IMG_ID_GX_MAGICSUSHI_NUMBER_9,
	IMG_ID_GX_MAGICSUSHI_TYPE_0,
	// TODO:
	TEXTURE_SCREEN,
	IMG_ID_GX_MAGICSUSHI_DOWN,
	IMG_ID_GX_MAGICSUSHI_UP,
	IMG_ID_GX_MAGICSUSHI_CURSOR,
	IMG_ID_GX_MAGICSUSHI_SELECTED,
	IMG_ID_GX_MAGICSUSHI_TYPE_NULL,
	IMG_ID_GX_MAGICSUSHI_NOMOREMOVE,
	IMG_ID_GX_MAGICSUSHI_UPLEVEL,
	IMG_ID_GX_MAGICSUSHI_GAMEOVER,
	IMG_ID_GX_MAGICSUSHI_GRADEMAP,
	IMG_ID_GX_MAGICSUSHI_GOPIC,
	IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND,
	TEXTURE_MAX
} TEXTURE;

#endif /* !MAGIC_SUSHI_TYPES_H */

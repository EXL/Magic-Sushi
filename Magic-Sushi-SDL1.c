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

#include <time.h>

#define MAINLCD_240X320
#define MMI_GX_MAGICSUSHI_LCD_BPP                   (16)

#if defined(MAINLCD_240X320)
#define MMI_GX_MAGICSUSHI_LCD_HEIGHT                (320)
#define MMI_GX_MAGICSUSHI_LCD_WIDTH                 (240)
#define MMI_GX_MAGICSUSHI_ORIGIN_X                  (8)
#define MMI_GX_MAGICSUSHI_ORIGIN_Y                  (43)
#define MMI_GX_MAGICSUSHI_SIZE                      (28)
#define MMI_GX_MAGICSUSHI_ANIMATION_STEP            (3)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_X           (75)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_Y           (276)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_X      (75)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y      (291)
#define MMI_GX_MAGICSUSHI_TIME_MAX_WIDTH            (175)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X1            (70)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y1            (270)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X2            (230)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y2            (300)
#define MMI_GX_MAGICSUSHI_SOFTKEY_X                 (190)
#define MMI_GX_MAGICSUSHI_SOFTKEY_Y                 (299)
#define MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH             (40)
#define MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT            (20)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_X          (105)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_Y          (26)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_X          (105)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_Y          (8)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X1            (50)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y1            (4)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X2            (120)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y2            (40)
#define MMI_GX_MAGICSUSHI_DIGIT_WIDTH               (10)
#define MMI_GX_MAGICSUSHI_LEFT_WIDTH                (8)
#define MMI_GX_MAGICSUSHI_RIGHT_WIDTH               (8)
#define MMI_GX_MAGICSUSHI_TOP_WIDTH                 (43)
#define MMI_GX_MAGICSUSHI_FOOD_AREA_WIDTH           (224)
#define MMI_GX_MAGICSUSHI_FOOD_AREA_HEIGHT          (224)
#define MMI_GX_MAGICSUSHI_HINT_POSITION_X           (20)
#define MMI_GX_MAGICSUSHI_HINT_POSITION_Y           (80)
#define MMI_GX_MAGICSUSHI_GAMEOVER_SCORE_POSITION_X (130)
#define MMI_GX_MAGICSUSHI_GAMEOVER_SCORE_POSITION_Y (130)
#define MMI_GX_MAGICSUSHI_TIME_WIDTH                (4)
#elif defined(MAINLCD_320X480)
#define MMI_GX_MAGICSUSHI_LCD_HEIGHT                (480)
#define MMI_GX_MAGICSUSHI_LCD_WIDTH                 (320)
#define MMI_GX_MAGICSUSHI_ORIGIN_X                  (8)
#define MMI_GX_MAGICSUSHI_ORIGIN_Y                  (101)
#define MMI_GX_MAGICSUSHI_SIZE                      (38)
#define MMI_GX_MAGICSUSHI_ANIMATION_STEP            (3)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_X           (87)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_Y           (419)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_X      (87)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y      (441)
#define MMI_GX_MAGICSUSHI_TIME_MAX_WIDTH            (225)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X1            (81)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y1            (414)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X2            (317)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y2            (454)
#define MMI_GX_MAGICSUSHI_SOFTKEY_X                 (283)
#define MMI_GX_MAGICSUSHI_SOFTKEY_Y                 (455)
#define MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH             (40)
#define MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT            (20)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_X          (200)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_Y          (51)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_X          (200)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_Y          (21)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X1            (100)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y1            (8)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X2            (220)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y2            (72)
#define MMI_GX_MAGICSUSHI_DIGIT_WIDTH               (7)
#define MMI_GX_MAGICSUSHI_LEFT_WIDTH                (8)
#define MMI_GX_MAGICSUSHI_RIGHT_WIDTH               (8)
#define MMI_GX_MAGICSUSHI_TOP_WIDTH                 (101)
#define MMI_GX_MAGICSUSHI_FOOD_AREA_WIDTH           (303)
#define MMI_GX_MAGICSUSHI_FOOD_AREA_HEIGHT          (303)
#define MMI_GX_MAGICSUSHI_HINT_POSITION_X           (65)
#define MMI_GX_MAGICSUSHI_HINT_POSITION_Y           (224)
#define MMI_GX_MAGICSUSHI_GAMEOVER_SCORE_POSITION_X (130)
#define MMI_GX_MAGICSUSHI_GAMEOVER_SCORE_POSITION_Y (130)
#define MMI_GX_MAGICSUSHI_TIME_WIDTH                (8)
#else
#error "Unknown screen resolution!"
#endif

typedef SDL_bool BOOL;
typedef SDL_bool MMI_BOOL;
typedef Sint8 S8;
typedef Uint8 U8;
typedef Sint16 S16;
typedef Uint16 U16;
typedef Sint32 S32;
typedef Uint32 U32;
typedef float FLOAT;
#define TRUE SDL_TRUE
#define FALSE SDL_FALSE
#define MMI_TRUE SDL_TRUE
#define MMI_FALSE SDL_FALSE

#define RAND_ITEM(i,j,item)                                                                      \
{                                                                                                \
	item =rand()%1000;                                                                           \
	if (item <5 && !g_gx_magicsushi_context.special)                                             \
	{g_gx_magicsushi_context.map[i][j] = 8;g_gx_magicsushi_context.special=TRUE;}                \
	else if (item <10 && !g_gx_magicsushi_context.special)                                       \
	{g_gx_magicsushi_context.map[i][j] = 9;g_gx_magicsushi_context.special=TRUE;}                \
	else if (item <15 && !g_gx_magicsushi_context.special)                                       \
	{g_gx_magicsushi_context.map[i][j] = 10;g_gx_magicsushi_context.special=TRUE;}               \
	else                                                                                         \
	g_gx_magicsushi_context.map[i][j] = item%g_gx_magicsushi_context.type_num;                   \
	g_gx_magicsushi_context.i_map[i][j] = 8;                                                     \
}

#define CHECK_VERTICAL(x,start_y,end_y,length,iNum,flag)                                         \
{                                                                                                \
	length=1;                                                                                    \
	for(++start_y; start_y <= end_y; start_y++)                                                  \
	{                                                                                            \
	  if( g_gx_magicsushi_context.map[x][start_y] == g_gx_magicsushi_context.map[x][start_y -1]) \
		 length++;                                                                               \
	  else                                                                                       \
	  {                                                                                          \
		 if(length > 2)                                                                          \
		 {                                                                                       \
			g_gx_magicsushi_context.solution_start_vertical[x][iNum] = start_y - length;         \
			g_gx_magicsushi_context.solution_end_vertical[x][iNum] = start_y - 1;                \
			g_gx_magicsushi_context.solution_start_vertical[x][++iNum] = -1;                     \
			flag =1;                                                                             \
		 }                                                                                       \
		 length = 1;                                                                             \
	  }                                                                                          \
	}                                                                                            \
	if(length > 2)                                                                               \
	{                                                                                            \
	  g_gx_magicsushi_context.solution_start_vertical[x][iNum] = start_y - length;               \
	  g_gx_magicsushi_context.solution_end_vertical[x][iNum] = start_y - 1;                      \
	  g_gx_magicsushi_context.solution_start_vertical[x][++iNum] = -1;                           \
	  flag =1;                                                                                   \
	}                                                                                            \
	g_gx_magicsushi_context.solution_start_vertical[x][iNum] = -1;                               \
}

#define CHECK_HORIZONTAL(y,start_x,end_x,length,iNum,flag)                                       \
{                                                                                                \
	length=1;                                                                                    \
	for(++start_x; start_x <= end_x; start_x++)                                                  \
	{                                                                                            \
	  if( g_gx_magicsushi_context.map[start_x][y] == g_gx_magicsushi_context.map[start_x -1][y]) \
		 length++;                                                                               \
	  else                                                                                       \
	  {                                                                                          \
		 if(length > 2)                                                                          \
		 {                                                                                       \
			g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = start_x - length ;      \
			g_gx_magicsushi_context.solution_end_horizontal[y][iNum] = start_x - 1;              \
			g_gx_magicsushi_context.solution_start_horizontal[y][++iNum] = -1;                   \
			flag =1;                                                                             \
		 }                                                                                       \
		 length = 1;                                                                             \
	  }                                                                                          \
	}                                                                                            \
	if(length > 2)                                                                               \
	{                                                                                            \
	  g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = start_x - length;             \
	  g_gx_magicsushi_context.solution_end_horizontal[y][iNum] = start_x - 1;                    \
	  g_gx_magicsushi_context.solution_start_horizontal[y][++iNum] = -1;                         \
	  flag =1;                                                                                   \
	}                                                                                            \
	g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = -1;                             \
}

typedef enum {
	MMI_GX_MAGICSUSHI_DIRECTION_UP,
	MMI_GX_MAGICSUSHI_DIRECTION_DOWN,
	MMI_GX_MAGICSUSHI_DIRECTION_LEFT,
	MMI_GX_MAGICSUSHI_DIRECTION_RIGHT,
	MMI_GX_MAGICSUSHI_DIRECTION_ALL
} mmi_gx_magicsushi_direction_enum;

static SDL_bool app_exec_flag = SDL_FALSE;
static SDL_Surface *screen = NULL;
static SDL_Surface *backscreen = NULL;
static SDL_Surface *active = NULL;

enum BITMAPS {
	IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND,
	IMG_ID_GX_MAGICSUSHI_TYPE_NULL,
	IMG_ID_GX_MAGICSUSHI_TYPE_0,
	IMG_ID_GX_MAGICSUSHI_TYPE_1,
	IMG_ID_GX_MAGICSUSHI_TYPE_2,
	IMG_ID_GX_MAGICSUSHI_TYPE_3,
	IMG_ID_GX_MAGICSUSHI_TYPE_4,
	IMG_ID_GX_MAGICSUSHI_TYPE_5,
	IMG_ID_GX_MAGICSUSHI_TYPE_6,
	IMG_ID_GX_MAGICSUSHI_TYPE_7,
	IMG_ID_GX_MAGICSUSHI_CURSOR,
	IMG_ID_GX_MAGICSUSHI_SELECTED,
	IMG_ID_GX_MAGICSUSHI_NOMOREMOVE,
	IMG_ID_GX_MAGICSUSHI_UPLEVEL,
	BITMAPS_MAX
};
static SDL_Surface *bitmaps[BITMAPS_MAX];

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

static void Set_Active_Surface(SDL_Surface *surface) {
	active = surface;
}

static void Draw_Bitmap_At(S16 x, S16 y, S16 bitmap_id) {
	SDL_Surface *bitmap = bitmaps[bitmap_id];
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.h = bitmap->h;
	r.w = bitmap->w;
	SDL_BlitSurface(bitmap, NULL, active, &r);
}

static void gdi_image_draw_id(S16 x, S16 y, S16 bitmap_id) {
	Draw_Bitmap_At(x, y, bitmap_id);
}

static void mmi_gx_magicsushi_gen_map(void) {
	U16 i, j, item;
	for (j = 0; j < 8; j++)
		for (i = 0; i < 8; i++) {
			RAND_ITEM(i, j, item);
		}

	g_gx_magicsushi_context.animation_step = 8 * MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 6;
}

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

static void mmi_gx_magicsushi_show_magicsushi(void) {
	int i, j, x, y;
	for (i = 0; i < 8; i++) {
		y = (7 - i) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		for (j = 0; j < 8; j++) {
			if (i == g_gx_magicsushi_context.i_map[j][i]) {
				x = j * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
				if (g_gx_magicsushi_context.map[j][i] == 255)
					Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_TYPE_NULL);
				else
					Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[j][i]);
			}
		}
	}
}

static void mmi_gx_magicsushi_gameover(void) {
	fprintf(stderr, "GAME OVER!\n");
	app_exec_flag = SDL_TRUE;
}

static U8 mmi_gx_magicsushi_check_all(void) {
	S16 i;
	U8 flag = 0, length, iNum;
	S8 x, y;

	x = 0;
	y = 7;
	for (i = 0; i < 8; i++) {
		iNum = 0;
		length = 1;
		x = 0;
		CHECK_VERTICAL(i, x, y, length, iNum, flag);

		iNum = 0;
		length = 1;
		x = 0;
		CHECK_HORIZONTAL(i, x, y, length, iNum, flag);
	}
	return flag;
}

static void mmi_gx_magicsushi_update_game_data(void) {
	U8 iNum = 0;
	U8 count, i;
	U8 time = g_gx_magicsushi_context.total_time / 10;
	S16 j;

	for (i = 0; i < 8; i++) {
		while (g_gx_magicsushi_context.solution_start_vertical[i][iNum] != -1) {
			count =
				g_gx_magicsushi_context.solution_end_vertical[i][iNum] -
				g_gx_magicsushi_context.solution_start_vertical[i][iNum] + 1;
			g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level * g_gx_magicsushi_context.bonus;
			g_gx_magicsushi_context.remainder -= 1;
			g_gx_magicsushi_context.remain_time += time;

			j = g_gx_magicsushi_context.solution_start_vertical[i][iNum];
			while (j <= g_gx_magicsushi_context.solution_end_vertical[i][iNum]) {
				g_gx_magicsushi_context.map[i][j] = 255;
				j++;
			}
			iNum++;
		}

		g_gx_magicsushi_context.solution_start_vertical[i][0] = -1;

		iNum = 0;
		while (g_gx_magicsushi_context.solution_start_horizontal[i][iNum] != -1) {
			count =
				g_gx_magicsushi_context.solution_end_horizontal[i][iNum] -
				g_gx_magicsushi_context.solution_start_horizontal[i][iNum] + 1;
			g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level * g_gx_magicsushi_context.bonus;
			g_gx_magicsushi_context.remainder -= 1;
			g_gx_magicsushi_context.remain_time += time;

			j = g_gx_magicsushi_context.solution_start_horizontal[i][iNum];
			while (j <= g_gx_magicsushi_context.solution_end_horizontal[i][iNum]) {
				g_gx_magicsushi_context.map[j][i] = 255;
				j++;
			}
			iNum++;
		}
		iNum = 0;
		g_gx_magicsushi_context.solution_start_horizontal[i][0] = -1;
	}
	g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 8;
	g_gx_magicsushi_context.bonus++;
}

static MMI_BOOL mmi_gx_magicsushi_is_current_move_ok(U8 i, U8 j, U8 food, mmi_gx_magicsushi_direction_enum direction) {
	/*judge up*/
	if (direction != MMI_GX_MAGICSUSHI_DIRECTION_DOWN)
	{
		if (j <= 5)
		{
			if (g_gx_magicsushi_context.map[i][j+1] == food && g_gx_magicsushi_context.map[i][j+2] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	/*judge down*/
	if (direction != MMI_GX_MAGICSUSHI_DIRECTION_UP)
	{
		if (j >= 2)
		{
			if (g_gx_magicsushi_context.map[i][j-1] == food &&
			        g_gx_magicsushi_context.map[i][j-2] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	/*judge left*/
	if (direction != MMI_GX_MAGICSUSHI_DIRECTION_RIGHT)
	{
		if (i >= 2)
		{
			if (g_gx_magicsushi_context.map[i-1][j] == food &&
			        g_gx_magicsushi_context.map[i-2][j] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	/*judge right*/
	if (direction != MMI_GX_MAGICSUSHI_DIRECTION_LEFT)
	{
		if (i <= 5)
		{
			if (g_gx_magicsushi_context.map[i+1][j] == food &&
			        g_gx_magicsushi_context.map[i+2][j] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	/*judge middle*/
	if ((direction == MMI_GX_MAGICSUSHI_DIRECTION_UP) || (direction == MMI_GX_MAGICSUSHI_DIRECTION_DOWN))
	{
		if ((i >= 1) && (i<=6))
		{
			if (g_gx_magicsushi_context.map[i-1][j] == food && g_gx_magicsushi_context.map[i+1][j] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	if ((direction == MMI_GX_MAGICSUSHI_DIRECTION_LEFT) || (direction == MMI_GX_MAGICSUSHI_DIRECTION_RIGHT))
	{
		if ((j >= 1) && (j <= 6))
		{
			if (g_gx_magicsushi_context.map[i][j+1] == food && g_gx_magicsushi_context.map[i][j-1] == food)
			{
				return MMI_TRUE;
			}
		}
	}

	return MMI_FALSE;
}

static void mmi_gx_magicsushi_update_map(void) {
	U8 item;    /* for RAND_ITEM() */
	U8 i, j, movesteps, max_move = 0;

	for (i = 0; i < 8; i++) {
		movesteps = 0;
		for (j = 0; j < 8; j++) {
			if (g_gx_magicsushi_context.map[i][j] == 255)
				movesteps++;
			else if (movesteps > 0) {
				g_gx_magicsushi_context.map[i][j - movesteps] = g_gx_magicsushi_context.map[i][j];
				g_gx_magicsushi_context.i_map[i][j - movesteps] = g_gx_magicsushi_context.i_map[i][j];
			}
		}

		for (j = 8 - movesteps; j < 8; j++)
		{
			RAND_ITEM(i, j, item);
		}
		if (movesteps > max_move)
		{
			max_move = movesteps;
		}
	}
	g_gx_magicsushi_context.animation_step = max_move * MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 6;
}

static U8 mmi_gx_magicsushi_is_moremove(void) {
	U8 i, j;

	if (g_gx_magicsushi_context.special == 1)
		return 1;

	for (i = 0; i <=7; i++) {
		for (j=0; j<=7;j++) {
			U8 food = g_gx_magicsushi_context.map[i][j];

			/*current move up*/
			if (j <= 6) {
				if (mmi_gx_magicsushi_is_current_move_ok(i, (U8)(j+1), food, MMI_GX_MAGICSUSHI_DIRECTION_UP)) {
					return 1;
				}
			}

			/*current move down*/
			if (j >= 1) {
				if (mmi_gx_magicsushi_is_current_move_ok(i, (U8)(j-1), food, MMI_GX_MAGICSUSHI_DIRECTION_DOWN)) {
					return 1;
				}
			}

			/*current move left*/
			if (i >= 1) {
				if (mmi_gx_magicsushi_is_current_move_ok((U8)(i-1), j, food, MMI_GX_MAGICSUSHI_DIRECTION_LEFT)) {
					return 1;
				}
			}

			/*current move right*/
			if (i <= 6) {
				if (mmi_gx_magicsushi_is_current_move_ok((U8)(i + 1), j, food, MMI_GX_MAGICSUSHI_DIRECTION_RIGHT)) {
					return 1;
				}
			}
		}
	}

	return 0;
}

static U8 mmi_gx_magicsushi_check_pair(U8 x1, U8 y1, U8 x2, U8 y2, U8 horizontal) {
	U8 flag = 0, length = 1, iNum = 0;
	S16 dx1, dx2, dy1, dy2;

	if (horizontal) {
		dy1 = y1 - 2;
		dy2 = y1 + 2;
		dx1 = x2 - 2;
		dx2 = x1 + 2;
		if (dy1 < 0)
			dy1 = 0;
		if (dy2 > 7)
			dy2 = 7;
		if (dx1 < 0)
			dx1 = 0;
		if (dx2 > 7)
			dx2 = 7;
		CHECK_HORIZONTAL(y1, dx1, dx2, length, iNum, flag);

		iNum = 0;
		length = 1;
		CHECK_VERTICAL(x1, dy1, dy2, length, iNum, flag);

		dy1 = y1 - 2;
		if (dy1 < 0)
		{
			dy1 = 0;
		}
		iNum = 0;
		length = 1;
		CHECK_VERTICAL(x2, dy1, dy2, length, iNum, flag);

	} else {
		dy1 = y2 - 2;
		dy2 = y1 + 2;
		dx1 = x1 - 2;
		dx2 = x1 + 2;
		if (dy1 < 0)
			dy1 = 0;
		if (dy2 > 7)
			dy2 = 7;
		if (dx1 < 0)
			dx1 = 0;
		if (dx2 > 7)
			dx2 = 7;
		CHECK_VERTICAL(x1, dy1, dy2, length, iNum, flag);
		iNum = 0;
		length = 1;
		CHECK_HORIZONTAL(y1, dx1, dx2, length, iNum, flag);

		dx1 = x1 - 2;
		if (dx1 < 0)
			dx1 = 0;
		iNum = 0;
		length = 1;
		CHECK_HORIZONTAL(y2, dx1, dx2, length, iNum, flag);
	}
	return flag;
}


static void mmi_gx_magicsushi_render(void) {
	U8 i, j, k, item;
	S16 x, y;
	U8 count;
	FLOAT tmp;
	FLOAT tmpfloat;
	FLOAT tmpfloat2;

	i = 0;
	j = 0;
	if (g_gx_magicsushi_context.is_frame_need_updated)
		g_gx_magicsushi_context.is_frame_need_updated = MMI_FALSE;

#if 0
	gdi_layer_set_active(g_gx_magicsushi_context.foreground);
	gdi_layer_push_clip();
	gdi_layer_set_clip(
	            MMI_GX_MAGICSUSHI_ORIGIN_X,
	            MMI_GX_MAGICSUSHI_ORIGIN_Y,
	            MMI_GX_MAGICSUSHI_LCD_WIDTH - 1,
	            MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);
#endif
#ifdef __MMI_TOUCH_SCREEN__
	/* softkey */
	if (g_gx_magicsushi_context.softkey_state == 1)
	{
		gdi_draw_solid_rect(
		            MMI_GX_MAGICSUSHI_SOFTKEY_X,
		            MMI_GX_MAGICSUSHI_SOFTKEY_Y,
		            MMI_GX_MAGICSUSHI_SOFTKEY_X + MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH - 1,
		            MMI_GX_MAGICSUSHI_SOFTKEY_Y + MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT - 1,
		            GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X, MMI_GX_MAGICSUSHI_SOFTKEY_Y, IMG_ID_GX_MAGICSUSHI_DOWN);
		g_gx_magicsushi_context.softkey_state = 0;
	}
	else if (g_gx_magicsushi_context.softkey_state == 2)
	{
		gdi_draw_solid_rect(
		            MMI_GX_MAGICSUSHI_SOFTKEY_X,
		            MMI_GX_MAGICSUSHI_SOFTKEY_Y,
		            MMI_GX_MAGICSUSHI_SOFTKEY_X + MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH - 1,
		            MMI_GX_MAGICSUSHI_SOFTKEY_Y + MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT - 1,
		            GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X, MMI_GX_MAGICSUSHI_SOFTKEY_Y, IMG_ID_GX_MAGICSUSHI_UP);
		g_gx_magicsushi_context.softkey_state = 0;
	}
#endif

	/* draw cursor x y */
	if (g_gx_magicsushi_context.last_key != 0) {
		switch (g_gx_magicsushi_context.last_key) {
		case 1:
			i = g_gx_magicsushi_context.special_x;
			j = g_gx_magicsushi_context.special_y;
			break;
		case 2:
			i = g_gx_magicsushi_context.cursor_x;
			j = g_gx_magicsushi_context.cursor_y - 1;
			break;
		case 4:
			i = g_gx_magicsushi_context.cursor_x + 1;
			j = g_gx_magicsushi_context.cursor_y;
			break;
		case 6:
			i = g_gx_magicsushi_context.cursor_x - 1;
			j = g_gx_magicsushi_context.cursor_y;
			break;
		case 8:
			i = g_gx_magicsushi_context.cursor_x;
			j = g_gx_magicsushi_context.cursor_y + 1;
			break;
		}

		y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;

//		TODO:???
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[i][j]);

//		gdi_image_draw_id(x, y, (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[i][j]));
		g_gx_magicsushi_context.last_key = 0;
	}

	y = (7 - g_gx_magicsushi_context.cursor_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
	x = g_gx_magicsushi_context.cursor_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
//	gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_CURSOR);
	Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_CURSOR);

	/* draw selected item */
	if (g_gx_magicsushi_context.selected == 1)
	{
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);
//		gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);
	}
	else if (g_gx_magicsushi_context.selected == 2)
	{
		g_gx_magicsushi_context.selected = 0;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;

//		TODO: ???
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		Draw_Bitmap_At(x, y, IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context. old_y]);
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context. old_y]));

	}

	/* handling animation */
	switch (g_gx_magicsushi_context.animation_mode)
	{
	case 0:
		/* do nothing */
		break;
	case 1:
		/* do nothnig */
		break;
	case 2:
		/* magic1: delete random selected type item */
		g_gx_magicsushi_context.animation_step--;
		count = 0;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context. old_y]));

		item = g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
		if (g_gx_magicsushi_context.animation_step == 0)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
					if (g_gx_magicsushi_context.map[i][j] == item)
					{
						count++;
						y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
						x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);
						g_gx_magicsushi_context.map[i][j] = 255;

					}
			g_gx_magicsushi_context.game_grade += count * g_gx_magicsushi_context.level;
			count /= 3;
			g_gx_magicsushi_context.remainder -= count;
			g_gx_magicsushi_context.remain_time += 3;

			g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
			g_gx_magicsushi_context.animation_mode = 8;

		}
		break;
	case 3:
		/* magic2: delete items by 3x3 matrix */
		g_gx_magicsushi_context.animation_step--;
		if (g_gx_magicsushi_context.animation_step == 0)
		{
			for (i = 0; i <= 2; i++)
			{
				if (g_gx_magicsushi_context.old_x + i - 1 < 0 || g_gx_magicsushi_context.old_x + i - 1 > 7)
				{
					continue;
				}
				else
				{
					x = (g_gx_magicsushi_context.old_x + i - 1) * MMI_GX_MAGICSUSHI_SIZE +
					        MMI_GX_MAGICSUSHI_LEFT_WIDTH;
				}

				for (j = 0; j <= 2; j++)
				{
					if (g_gx_magicsushi_context.old_y + j - 1 < 0 || g_gx_magicsushi_context.old_y + j - 1 > 7)
					{
						continue;
					}
					else
					{
						y = (8 - g_gx_magicsushi_context.old_y - j) * MMI_GX_MAGICSUSHI_SIZE +
						        MMI_GX_MAGICSUSHI_TOP_WIDTH;
						gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);

						g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x + i -
						        1][g_gx_magicsushi_context.old_y + j - 1] = 255;
					}
				}
			}
			count = 3;
			g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level * count;
			g_gx_magicsushi_context.remainder -= count;
			g_gx_magicsushi_context.remain_time += 3;
			g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
			g_gx_magicsushi_context.animation_mode = 8;
		}

		break;
	case 4:
		/* magic3: delete items by cross deletion */
		g_gx_magicsushi_context.animation_step--;
		if (g_gx_magicsushi_context.animation_step == 0)
		{
			x = (g_gx_magicsushi_context.old_x) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
			for (i = 0; i < 8; i++)
			{
				y = (7 - i) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
				gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);
				g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][i] = 255;

			}
			y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
			for (i = 0; i < 8; i++)
			{
				x = (i) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
				gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_SELECTED);
				g_gx_magicsushi_context.map[i][g_gx_magicsushi_context.old_y] = 255;

			}
			g_gx_magicsushi_context.game_grade += 5 * g_gx_magicsushi_context.level;
			g_gx_magicsushi_context.remainder -= 5;
			g_gx_magicsushi_context.remain_time += 3;
			g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
			g_gx_magicsushi_context.animation_mode = 8;
		}
		break;
	case 5:
		/* up-down switch */
		g_gx_magicsushi_context.animation_step--;
		tmp =
		        (float)(MMI_GX_MAGICSUSHI_ANIMATION_STEP -
		                g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
		tmpfloat = tmp / MMI_GX_MAGICSUSHI_ANIMATION_STEP;

		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		y = (7 - g_gx_magicsushi_context.special_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE
		        + (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE) + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context. old_y]));

		y = (7 - g_gx_magicsushi_context.special_y) * MMI_GX_MAGICSUSHI_SIZE
		        - (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE) + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context. special_x][g_gx_magicsushi_context.special_y]));

		if (g_gx_magicsushi_context.animation_step == 0)
		{
			tmp = g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y];
			g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y] =
			        g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
			g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y] = (U8) tmp;
			if (g_gx_magicsushi_context.check &&
			        !mmi_gx_magicsushi_check_pair(
			            g_gx_magicsushi_context.old_x,
			            g_gx_magicsushi_context.old_y,
			            g_gx_magicsushi_context.special_x,
			            g_gx_magicsushi_context.special_y,
			            0))
			{
				g_gx_magicsushi_context.check = 0;
				g_gx_magicsushi_context.animation_step = 3;
				g_gx_magicsushi_context.animation_mode = 5;
				g_gx_magicsushi_context.click_mode = 0;
			}
			else if (g_gx_magicsushi_context.check)
			{
				mmi_gx_magicsushi_update_game_data();
				g_gx_magicsushi_context.click_mode = 0;
				g_gx_magicsushi_context.check = 0;
			}
			else
			{
				g_gx_magicsushi_context.animation_mode = 0;
				g_gx_magicsushi_context.moving = 0;
				g_gx_magicsushi_context.click_mode = 0;
				g_gx_magicsushi_context.check = 0;
			}
		}
		break;
	case 7:
		/* left-right switch */
		g_gx_magicsushi_context.animation_step--;
		tmp =
		        (float)(MMI_GX_MAGICSUSHI_ANIMATION_STEP -
		                g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
		tmpfloat = tmp / MMI_GX_MAGICSUSHI_ANIMATION_STEP;

		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		x = g_gx_magicsushi_context.special_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
//		gdi_draw_solid_rect(x, y, x + MMI_GX_MAGICSUSHI_SIZE - 1, y + MMI_GX_MAGICSUSHI_SIZE - 1, GDI_COLOR_TRANSPARENT);

		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH -
		        (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE);
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context. old_y]));

		x = g_gx_magicsushi_context.special_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH +
		        (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE);
		gdi_image_draw_id(
		            x,
		            y,
		            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context. special_x][g_gx_magicsushi_context.special_y]));

		if (g_gx_magicsushi_context.animation_step == 0)
		{
			tmp = g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y];
			g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y] =
			        g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
			g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y] = (U8) tmp;
			if (g_gx_magicsushi_context.check &&
			        !mmi_gx_magicsushi_check_pair(
			            g_gx_magicsushi_context.old_x,
			            g_gx_magicsushi_context.old_y,
			            g_gx_magicsushi_context.special_x,
			            g_gx_magicsushi_context.special_y,
			            1))
			{
				g_gx_magicsushi_context.check = 0;
				g_gx_magicsushi_context.animation_step = 3;
				g_gx_magicsushi_context.animation_mode = 7;
				g_gx_magicsushi_context.click_mode = 0;
			}
			else if (g_gx_magicsushi_context.check)
			{
				mmi_gx_magicsushi_update_game_data();
				g_gx_magicsushi_context.click_mode = 0;
				g_gx_magicsushi_context.check = 0;
			}
			else
			{
				g_gx_magicsushi_context.animation_mode = 0;
				g_gx_magicsushi_context.moving = 0;
				g_gx_magicsushi_context.click_mode = 0;
				g_gx_magicsushi_context.check = 0;
			}
		}
		break;
	case 6:
		/* dropping */
		g_gx_magicsushi_context.animation_step--;
		g_gx_magicsushi_context.tick = 0;
		tmp = (float)(g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
		tmpfloat = tmp / MMI_GX_MAGICSUSHI_ANIMATION_STEP;
		tmpfloat2 = (tmp + 1) / MMI_GX_MAGICSUSHI_ANIMATION_STEP;

		for (i = 0; i < 8; i++)
		{
			k = 0;
			tmp = (float)(g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
			for (j = 0; j < 8; j++)
			{
				if (j != g_gx_magicsushi_context.i_map[i][j])
				{
					x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
					y = (8 - g_gx_magicsushi_context.i_map[i][j]) * MMI_GX_MAGICSUSHI_SIZE
					        + MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32) (tmpfloat2 * MMI_GX_MAGICSUSHI_SIZE);
//					gdi_draw_solid_rect(
//					            x,
//					            y,
//					            x + MMI_GX_MAGICSUSHI_SIZE - 1,
//					            y + MMI_GX_MAGICSUSHI_SIZE - 1,
//					            GDI_COLOR_TRANSPARENT);

					y = (8 - g_gx_magicsushi_context.i_map[i][j]) * MMI_GX_MAGICSUSHI_SIZE
					        + MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE);
					gdi_image_draw_id(
					            x,
					            y,
					            (U16) (IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[i][j]));

					if (k != 0 && (g_gx_magicsushi_context.i_map[i][j] - k) > 1)
					{
						x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						y = (7 - k) * MMI_GX_MAGICSUSHI_SIZE
						        + MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32) (tmpfloat * MMI_GX_MAGICSUSHI_SIZE);
//						gdi_draw_solid_rect(
//						            x,
//						            y,
//						            x + MMI_GX_MAGICSUSHI_SIZE - 1,
//						            y + MMI_GX_MAGICSUSHI_SIZE - 1,
//						            GDI_COLOR_TRANSPARENT);
					}
					k = g_gx_magicsushi_context.i_map[i][j];

					if (tmp == 0)
					{
						g_gx_magicsushi_context.i_map[i][j]--;
						if (g_gx_magicsushi_context.i_map[i][j] == 7)
						{
							break;
						}
					}

					if (g_gx_magicsushi_context.i_map[i][j] == 8)
					{
						break;
					}
				}
			}
		}
		if (g_gx_magicsushi_context.animation_step == 0)
		{
			if (mmi_gx_magicsushi_check_all())
			{
				mmi_gx_magicsushi_update_game_data();
			}

			else if (g_gx_magicsushi_context.remainder <= 0)
			{
				g_gx_magicsushi_context.bonus = 1;
				g_gx_magicsushi_context.total_time -= 5;
				//g_gx_magicsushi_context.remain_time = g_gx_magicsushi_context.total_time / 2;
				g_gx_magicsushi_context.remainder = 50;
				g_gx_magicsushi_context.level++;
				g_gx_magicsushi_context.animation_step = 6;
				g_gx_magicsushi_context.animation_mode = 11;
				g_gx_magicsushi_context.special = 0;
				if (g_gx_magicsushi_context.level > 4)
				{
					g_gx_magicsushi_context.type_num = 8;
				}
			}
			else if (!mmi_gx_magicsushi_is_moremove())
			{
				g_gx_magicsushi_context.bonus = 1;
				g_gx_magicsushi_context.animation_step = 6;
				g_gx_magicsushi_context.animation_mode = 10;
				g_gx_magicsushi_context.special = 0;
			}
			else
			{
				g_gx_magicsushi_context.click_mode = 0;
				g_gx_magicsushi_context.animation_mode = 0;
				g_gx_magicsushi_context.moving = 0;
				g_gx_magicsushi_context.bonus = 1;
			}
		}
		break;
	case 8:
		/* update image for items being removed */
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (g_gx_magicsushi_context.map[i][j] == 255)
				{
					x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
					y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
					gdi_image_draw_id(x, y, IMG_ID_GX_MAGICSUSHI_TYPE_NULL);
				}
			}
		}
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

		GFX_PLAY_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
#else /* __MMI_GAME_MULTICHANNEL_SOUND__ */
//		GFX_PLAY_AUDIO_MIDI(MAGICSUSHIMove, MAGICSUSHIMOVE, DEVICE_AUDIO_PLAY_ONCE);
#endif /* __MMI_GAME_MULTICHANNEL_SOUND__ */

		g_gx_magicsushi_context.animation_mode = 9;
		break;
	case 9:
		/* clear image for items being removed */
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (g_gx_magicsushi_context.map[i][j] == 255)
				{
					x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
					y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
//					gdi_draw_solid_rect(
//					            x,
//					            y,
//					            x + MMI_GX_MAGICSUSHI_SIZE - 1,
//					            y + MMI_GX_MAGICSUSHI_SIZE - 1,
//					            GDI_COLOR_TRANSPARENT);

				}
			}
		}
		mmi_gx_magicsushi_update_map();
		break;
	case 10:
		/* show no more move */
		g_gx_magicsushi_context.animation_step--;

#ifndef __MMI_GAME_MULTI_LANGUAGE_SUPPORT__
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_HINT_POSITION_X, MMI_GX_MAGICSUSHI_HINT_POSITION_Y, IMG_ID_GX_MAGICSUSHI_NOMOREMOVE);
#else   /* __MMI_GAME_MULTI_LANGUAGE_SUPPORT__ */
		gdi_image_draw_id(
		            MMI_GX_MAGICSUSHI_HINT_POSITION_X,
		            MMI_GX_MAGICSUSHI_HINT_POSITION_Y,
		            mmi_gfx_get_multilanguage_image_ID(
		                mmi_magicsushi_language_index,
		                IMG_ID_GX_MAGICSUSHI_BACKGROUND_LANGUAGE0,
		                MMI_MAGICSUSHI_STRING_COUNT,
		                MMI_MAGICSUSHI_STRING_NOMOREMOVE));
#endif /* __MMI_GAME_MULTI_LANGUAGE_SUPPORT__ */

		if (g_gx_magicsushi_context.animation_step == 0)
		{
//			gdi_draw_solid_rect(
//			            MMI_GX_MAGICSUSHI_ORIGIN_X,
//			            MMI_GX_MAGICSUSHI_ORIGIN_Y,
//			            MMI_GX_MAGICSUSHI_ORIGIN_X + MMI_GX_MAGICSUSHI_FOOD_AREA_WIDTH - 1,
//			            MMI_GX_MAGICSUSHI_ORIGIN_Y + MMI_GX_MAGICSUSHI_FOOD_AREA_HEIGHT - 1,
//			            GDI_COLOR_TRANSPARENT);
			mmi_gx_magicsushi_gen_map();
		}
		break;
	case 11:
		/* show uplevel */
		g_gx_magicsushi_context.animation_step--;

#ifndef __MMI_GAME_MULTI_LANGUAGE_SUPPORT__
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_HINT_POSITION_X, MMI_GX_MAGICSUSHI_HINT_POSITION_Y, IMG_ID_GX_MAGICSUSHI_UPLEVEL);
#else
		gdi_image_draw_id(
		            MMI_GX_MAGICSUSHI_HINT_POSITION_X,
		            MMI_GX_MAGICSUSHI_HINT_POSITION_Y,
		            mmi_gfx_get_multilanguage_image_ID(
		                mmi_magicsushi_language_index,
		                IMG_ID_GX_MAGICSUSHI_BACKGROUND_LANGUAGE0,
		                MMI_MAGICSUSHI_STRING_COUNT,
		                MMI_MAGICSUSHI_STRING_UPLEVEL));
#endif

		if (g_gx_magicsushi_context.animation_step == 0)
		{
//			gdi_draw_solid_rect(
//			            MMI_GX_MAGICSUSHI_ORIGIN_X,
//			            MMI_GX_MAGICSUSHI_ORIGIN_Y,
//			            MMI_GX_MAGICSUSHI_ORIGIN_X + MMI_GX_MAGICSUSHI_FOOD_AREA_WIDTH - 1,
//			            MMI_GX_MAGICSUSHI_ORIGIN_Y + MMI_GX_MAGICSUSHI_FOOD_AREA_HEIGHT - 1,
//			            GDI_COLOR_TRANSPARENT);
			mmi_gx_magicsushi_gen_map();
		}
		break;
	}

	// mmi_gx_magicsushi_draw_dynamic_background();

//	gdi_layer_blt(
//	            g_gx_magicsushi_context.background, /* layer 0 */
//	            g_gx_magicsushi_context.foreground, /* layer 1 */
//	            0,                                  /* layer 2 */
//	            0,                                  /* layer 3 */
//	            0,                                  /* offset x */
//	            0,                                  /* offset y */
//	            MMI_GX_MAGICSUSHI_LCD_WIDTH - 1,    /* width */
//	            MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);  /* height */

//	gdi_layer_pop_clip();
//	gdi_layer_set_active(g_gx_magicsushi_context.background);

}

static void mmi_gx_magicsushi_cyclic_timer(void) {
	g_gx_magicsushi_context.tick++;
	if (g_gx_magicsushi_context.tick > 65530)
		g_gx_magicsushi_context.tick = 0;

//	gui_start_timer(g_gx_magicsushi_context.timer_elapse, mmi_gx_magicsushi_cyclic_timer);

	/* update TIME every 1 second */
	if (g_gx_magicsushi_context.tick % 5 == 0) {
		--g_gx_magicsushi_context.remain_time;

		if (g_gx_magicsushi_context.remain_time == 0)
			mmi_gx_magicsushi_gameover();
	}
	if (g_gx_magicsushi_context.is_gameover == FALSE) {
//		mmi_gx_magicsushi_framemove();
		mmi_gx_magicsushi_render();
	}
}

static void mmi_gx_magicsushi_enter_game(void) {
	Set_Active_Surface(screen);

	U32 blue = SDL_MapRGB(backscreen->format, 0, 0, 255); // GDI_COLOR_BLUE ??
	SDL_SetColorKey(backscreen, SDL_TRUE, blue);
	SDL_Rect r = { 0, 0, MMI_GX_MAGICSUSHI_LCD_WIDTH, MMI_GX_MAGICSUSHI_LCD_HEIGHT };
	SDL_FillRect(backscreen, &r, blue);

	if (g_gx_magicsushi_context.is_new_game == TRUE)
		mmi_gx_magicsushi_init_game();
	else
		mmi_gx_magicsushi_show_magicsushi();

	g_gx_magicsushi_context.is_new_game = FALSE;
	g_gx_magicsushi_context.is_gameover = FALSE;

	Set_Active_Surface(screen);
	Draw_Bitmap_At(0, 0, IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);

#if 0
	/* draw softkay image */
	gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X, MMI_GX_MAGICSUSHI_SOFTKEY_Y, IMG_ID_GX_MAGICSUSHI_UP);
	wgui_register_pen_down_handler(mmi_gx_magicsushi_pen_down_hdlr);    /* function to pen down event */
	wgui_register_pen_up_handler(mmi_gx_magicsushi_pen_up_hdlr);        /* function to pen up event */
	wgui_register_pen_move_handler(mmi_gx_magicsushi_pen_move_hdlr);    /* function to pen up event */

	SetKeyHandler(mmi_gx_magicsushi_key_5_release, KEY_5, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_magicsushi_key_5_release, KEY_IP, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_magicsushi_key_2_release, KEY_2, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_2_release, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_4_release, KEY_4, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_4_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_6_release, KEY_6, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_6_down,    KEY_6, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_magicsushi_key_6_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_6_down,    KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_magicsushi_key_8_release, KEY_8, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_magicsushi_key_8_release, KEY_DOWN_ARROW, KEY_EVENT_UP);

	SetKeyHandler(mmi_gx_magicsushi_rsk_press, KEY_RSK, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_magicsushi_rsk_release, KEY_RSK, KEY_EVENT_UP);

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	GFX_OPEN_BACKGROUND_SOUND(MAGICSUSHIBackground, MAGICSUSHIBACKGROUND, g_gx_magicsushi_context.background_midi);
	GFX_PLAY_BACKGROUND_SOUND(g_gx_magicsushi_context.background_midi);

	GFX_OPEN_DUMMY_BACKGROUND_SOUND();
	GFX_PLAY_DUMMY_BACKGROUND_SOUND();

	GFX_OPEN_SOUND_EFFECTS_MIDI(MAGICSUSHIMove, MAGICSUSHIMOVE, 1, g_gx_magicsushi_context.move_midi);
	GFX_OPEN_SOUND_EFFECTS_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, 1, g_gx_magicsushi_context.select_midi);
#endif /* __MMI_GAME_MULTICHANNEL_SOUND__ */
#endif
	/* start game loop */
	mmi_gx_magicsushi_cyclic_timer();
}

void load_bitmaps(void) {
	bitmaps[IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND] = SDL_LoadBMP("assets/gx_magicsushi_background.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_NULL] = SDL_LoadBMP("assets/gx_magicsushi_item_null.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_0] = SDL_LoadBMP("assets/gx_magicsushi_item_1.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_1] = SDL_LoadBMP("assets/gx_magicsushi_item_2.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_2] = SDL_LoadBMP("assets/gx_magicsushi_item_3.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_3] = SDL_LoadBMP("assets/gx_magicsushi_item_4.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_4] = SDL_LoadBMP("assets/gx_magicsushi_item_5.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_5] = SDL_LoadBMP("assets/gx_magicsushi_item_6.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_6] = SDL_LoadBMP("assets/gx_magicsushi_item_7.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_TYPE_7] = SDL_LoadBMP("assets/gx_magicsushi_item_8.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_CURSOR] = SDL_LoadBMP("assets/gx_magicsushi_select1.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_SELECTED] = SDL_LoadBMP("assets/gx_magicsushi_select.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_NOMOREMOVE] = SDL_LoadBMP("assets/gx_magicsushi_nomoremove.bmp");
	bitmaps[IMG_ID_GX_MAGICSUSHI_UPLEVEL] = SDL_LoadBMP("assets/gx_magicsushi_uplevel.bmp");
}

int main(int argc, char *argv[]) {
	srand(time(0));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "SDL_Init error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	screen = SDL_SetVideoMode(MMI_GX_MAGICSUSHI_LCD_WIDTH, MMI_GX_MAGICSUSHI_LCD_HEIGHT,
		MMI_GX_MAGICSUSHI_LCD_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "SDL_SetVideoMode error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	backscreen = SDL_CreateRGBSurface(SDL_HWSURFACE,
		MMI_GX_MAGICSUSHI_LCD_WIDTH, MMI_GX_MAGICSUSHI_LCD_HEIGHT, MMI_GX_MAGICSUSHI_LCD_BPP,
			0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (backscreen == NULL) {
		fprintf(stderr, "SDL_CreateRGBSurface error: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	load_bitmaps();
	mmi_gx_magicsushi_enter_game();

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

		mmi_gx_magicsushi_cyclic_timer();

		SDL_UpdateRect(screen, 0, 0, MMI_GX_MAGICSUSHI_LCD_WIDTH, MMI_GX_MAGICSUSHI_LCD_HEIGHT);
		SDL_Delay(100);
	}

	SDL_FreeSurface(backscreen);
	SDL_FreeSurface(screen);

	SDL_Quit();
	return EXIT_SUCCESS;
}

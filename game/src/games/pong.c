/*
 * pong.c
 *
 * Created: 2018-10-21 17:14:21
 *  Author: Magnus Sörensen
 */ 

#include "pong.h"
#include "pong_charts.h"

/******************************
* Globals
******************************/
int player_a=0, player_b=0;
int player_a_pos = 2, player_b_pos=2;
int i=0;
// ball position is an vector array of on an fiald that is
// 40 position wide and 8 position high.
#define FIELD_SIZE (40*8)/32
int ball_pos[FIELD_SIZE];
int ball_xpos = 0;
int ball_ypos = 0;

/******************************
* Macro for bit operations on 
* the field vector
******************************/
#define SetBit(A,k) (A[(k/32)] |= (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))         
#define TestBit(A,k) (A[(k/32)] & (1 << (k%32)))

// dx,dy
float ball_vector[] = {1.0, 0.0};

/******************************
* private function prototypes
******************************/
void player_a_left(int buttons);
void player_a_right(int buttons);
void player_b_left(int buttons);
void player_b_right(int buttons);
void shoot(int buttons);
void move_ball(float dx, float dy);
void pong_update(usec_t time);
void pong_draw(usec_t time);
void pong_init_screen(void);

void poing_init(void)
{
	// PID variables.
	int pong_button_pid, pong_update_pid, pong_draw_pid;
	// clear the field
	for (i = 0; i<FIELD_SIZE;i++)
	{
		ball_pos[i] = 0;
	}
	pong_init_screen();
	//dip204_set_cursor_position(1,1);
	lib_buttons_t pong_buttons = {
		.onButton0 = player_a_left,
		.onButton1 = player_a_right,
		.onLeft = player_b_left,
		.onRight = player_b_right,
	};
	pong_button_pid = lib_shedReg(lib_buttonsInit(&pong_buttons), PONG_GAME_SPEED%8);
	pong_update_pid = lib_shedReg(pong_update, PONG_GAME_SPEED%8);
	pong_draw_pid   = lib_shedReg(pong_draw, PONG_GAME_SPEED%8);
	
	lib_timeOn();
	while(pong_status); // Don't exit until game is finished.
	lib_timeOff();
	lib_shedRem(pong_button_pid);
	lib_shedRem(pong_update_pid);
	lib_shedRem(pong_draw_pid);
}

/* ===== Private functions ========== */
void player_a_left(int buttons)
{
	player_a_pos += 1;	
}
void player_a_right(int buttons)
{
	player_a_pos -= 1;	
}
void player_b_left(int buttons)
{
	
	player_b_pos += 1;	
}
void player_b_right(int buttons)
{
	player_b_pos -= 1;	
}
void shoot(int buttons)
{
	pong_status = PONG_START;	
}
void move_ball(float dx, float dy)
{
	if (pong_status == PONG_START)
	{
		// The ball is on a player board.
	}
	else if (pong_status == PONG_RUN)
	{
		// The ball is flying.	
	}
}
void pong_update(usec_t time)
{
	// Is the move a collision?	
	// if dependent on what:
	// --> screen edge:
	//		--> left top and dx = -1 => move ball to lower right
	//		--> left bottom => pong_status = GOAL
	//		--> right top => pong_tatus = GOAL
	//		--> right bottom and dx = +1 => move ball to upper right
	// --> wall => reflect
	// --> board => reflect
}
void pong_draw(usec_t time)
{
	// Draws the game on the screen.
	
}
void pong_init_screen(void)
{
	display_init();
	dip204_clear_display();
	// ball no wall
	dip204_create_char(disp_leftUp, charts_leftUp);
	dip204_create_char(disp_leftDown, charts_leftDown);
	dip204_create_char(disp_rightUp, charts_rightUp);
	dip204_create_char(disp_rightDown, charts_rightDown);
	// ball no wall
	dip204_create_char(disp_leftUp, charts_leftUp);
	dip204_create_char(disp_leftDown, charts_leftDown);
	dip204_create_char(disp_rightUp, charts_rightUp);
	dip204_create_char(disp_rightDown, charts_rightDown);
}

/*
 * pong.c
 *
 * Created: 2018-10-21 17:14:21
 *  Author: Magnus Sörensen
 */ 

#include "pong.h"
#include "../libs/lib_led.h"
//#include "../libs/lib_math.h"


#define E1 10
#define E2 11



struct pong_game_t {
	int game_status;
	int last_won;
	struct {
		int pos;
		int points;
		struct {
			int row;
			int start;
			int end;
		}goal;
	}playerA;
	struct {
		int pos;
		int points;
		struct {
			int row;
			int start;
			int end;
		}goal;
	}playerB;
	struct pong_ball_t {
		short x;
		short y;
		short dx;
		short dy;
	}pong_ball;
}pong_game;

/******************************
* Globals
******************************/
// ball position is an vector array of on an fiald that is
// 40 position wide and 8 position high.


/******************************
* private function prototypes
******************************/
void player_a_left(int buttons);
void player_a_right(int buttons);
void player_b_left(int buttons);
void player_b_right(int buttons);
void shoot(int buttons);
void pong_update(usec_t time);
void pong_draw(usec_t time);
void pong_init_screen(void);
void pong_reflect(int wx, int wy);

void poing_init(void)
{
	// PID variables.
	int pong_button_pid, pong_update_pid, pong_draw_pid;
	// Set game variables.
	pong_game.game_status = PONG_START; // locked to plate 
	pong_game.last_won = PONG_STARTA;
	pong_game.playerA.points=0;
	pong_game.playerB.points=0;
	pong_game.playerA.pos=2;
	pong_game.playerB.pos= 2;
	// Set up goal fieald.
	pong_game.playerA.goal.row = 4;
	pong_game.playerA.goal.start = 1;
	pong_game.playerA.goal.end = 9;
	pong_game.playerB.goal.row = 1;
	pong_game.playerB.goal.start = 11;
	pong_game.playerB.goal.end = 20;
	// Create the ball.	
	pong_game.pong_ball.x=2;
	pong_game.pong_ball.y=3;
	pong_game.pong_ball.dx=1;
	pong_game.pong_ball.dy=-1;
	int i = 0;	
	lib_ledInit();
	for (i = 0; i<6;i++)
	{
		// Turn off all ledds.
		lib_ledOff(i);
	}
		
	// clear the field
	pong_init_screen();
	//dip204_set_cursor_position(1,1);
	lib_buttons_t pong_buttons = {
		.onLeft = player_b_left,
		.onRight = player_b_right,
		.onButton0 = player_a_left,
		.onButton1 = player_a_right,
		.onButton2 = shoot,
	};
	pong_button_pid = lib_shedReg(lib_buttonsInit(&pong_buttons), PONG_GAME_SPEED );
	pong_draw_pid   = lib_shedReg(pong_draw, PONG_GAME_SPEED );
	pong_update_pid = lib_shedReg(pong_update, PONG_GAME_SPEED );
	
	if ((pong_button_pid == FAILED_TO_CREATE_TASK)||(pong_update_pid == FAILED_TO_CREATE_TASK )|| (pong_draw_pid == FAILED_TO_CREATE_TASK))
	{
		while(1);
	}
		
	lib_timeOn();
	while(pong_game.game_status != PONG_EXIT); // Don't exit until game is finished.
	lib_timeOff();
	lib_shedRem(pong_button_pid);
	lib_shedRem(pong_update_pid);
	lib_shedRem(pong_draw_pid);
}

/******** Private functions ************ 
* Player actions                       *
***************************************/
void player_a_left(int buttons)
{
	if (pong_game.playerA.pos > 1)
	{
		pong_game.playerA.pos--;
	}
}
void player_a_right(int buttons)
{
	if (pong_game.playerA.pos < E1 - 1)
	{
		pong_game.playerA.pos++;
	}
}
void player_b_left(int buttons)
{
	if (pong_game.playerB.pos + PONG_PLAYER_B_OFFSET > E2 + 1)
	{
	pong_game.playerB.pos--;
	}
}
void player_b_right(int buttons)
{
		
	if (pong_game.playerB.pos + PONG_PLAYER_B_OFFSET < 20)
	{
		pong_game.playerB.pos++;
	}
}
void shoot(int buttons)
{
	pong_game.game_status = PONG_RUN;
}

/********************************************
* Game mechanics
********************************************/
void pong_update(usec_t time)
{
	int x=0,y=0,dot=0,i = 0;
	int wall[2];
	int temp[2];

	if (pong_game.game_status == PONG_RUN )
	{
		// is the ball going to hit a wall?
		x = pong_game.pong_ball.x + pong_game.pong_ball.dx;
		y = pong_game.pong_ball.y + pong_game.pong_ball.dy;
		// the walls in the game are only vertical so i only need tho check the x axis.
		if ((x < 1)||(x == E1)||(x==E2)||(x > 19))
		{
			// yes the ball hit the wall reflect
			pong_reflect(1,0);
			//pong_game.pong_ball.x++;
		}
		else if ((x == pong_game.playerA.pos)&&(y == 4))
		{
			// the ball hists the board and is reflected. 	
			pong_reflect(0,1);
		}
		else if ((x == pong_game.playerB.pos + PONG_PLAYER_B_OFFSET )&&(y == 1))
		{
			// the ball hints the board and is reflected. 	
			pong_reflect(0,1);
		}
		else if ((0 < x )&&(x <= 9)&&(y < 1))
		{
			// hitts top of player A cort gets moved to the B cort
			pong_game.pong_ball.x +=E2;
			pong_game.pong_ball.y = 4;
		}
		else if ((E2<= x )&&(x <= 20)&&(y > 4))
		{
			// hitts bottom of player B cort gets moved to the A cort
			pong_game.pong_ball.x -=E1;
			pong_game.pong_ball.y = 1;
		}
		else if ((1 < x )&&(x < E1)&&(y > 4))
		{
			// GOOAL for player B
			pong_game.playerB.points += 1;
			if (pong_game.playerB.points <= 3)
			{
				// if so then show menu.
				pong_game.game_status=PONG_MENU;
			} 
			else
			{
				pong_game.game_status=PONG_STARTA;
				for (i=3; i < pong_game.playerB.points + 3;i++)
				{
					//tun on leds for score
					lib_ledOn(i);
				}
			}
		}
		else if ((E2 < x )&&(x < 20)&&(y < 1))
		{
			// GOOAL for player A
			pong_game.playerA.points += 1;
			if (pong_game.playerA.points <= 3)
			{
				// if so then show menu.
				pong_game.game_status=PONG_MENU;
			} 
			else
			{
				pong_game.game_status=PONG_STARTB;
				for (i=0; i < pong_game.playerB.points ;i++)
				{
					//tun on leds for score
					lib_ledOn(i);
				}
			}
		}
		else
		{
			// Didn't hitt any thing move ball dx dy
			pong_game.pong_ball.x += pong_game.pong_ball.dx;
			pong_game.pong_ball.y += pong_game.pong_ball.dy;
		}
	}
	else if (pong_game.game_status == PONG_STARTA)
	{
		// Ball is glued to player A
		pong_game.pong_ball.x = pong_game.playerA.pos;
		pong_game.pong_ball.y = PONG_PLAYER_A_OFFSET - 1;
		pong_game.pong_ball.dx = 1;
		pong_game.pong_ball.dy = -1;
	}
	else if (pong_game.game_status == PONG_STARTB)
	{
		// Bal is glued to player B
		pong_game.pong_ball.x = pong_game.playerB.pos + PONG_PLAYER_B_OFFSET;
		pong_game.pong_ball.y = 2;
		pong_game.pong_ball.dx = -1;
		pong_game.pong_ball.dy = 1;
	}
	else if (pong_game.game_status == PONG_MENU)
	{
		pong_game.game_status = PONG_STARTB;
	}
}
void pong_draw(usec_t time)
{
	// Draws the game on the screen.
	int i = 0;
	dip204_clear_display();
	// Show the field
	for (i = 1; i < 5;i++)
	{
		dip204_set_cursor_position(E1,i)	;
		dip204_write_string("||");
	}
	// show the players.
	dip204_set_cursor_position(pong_game.playerA.pos, PONG_PLAYER_A_OFFSET);
	dip204_write_data('-');
	dip204_set_cursor_position(pong_game.playerB.pos+PONG_PLAYER_B_OFFSET, 1);
	dip204_write_data('-');
	// show ball
	dip204_set_cursor_position(pong_game.pong_ball.x,pong_game.pong_ball.y);
	dip204_write_data('o');
}

void pong_reflect(int wx, int wy)
{
	// Reflection = Vector - 2 * <Vector|Flor>Flor
	int dot = wx*pong_game.pong_ball.dx;
	dot += wy*pong_game.pong_ball.dy;
	pong_game.pong_ball.dx = pong_game.pong_ball.dx - 2*dot*wx;
	pong_game.pong_ball.dy = pong_game.pong_ball.dy - 2*dot*wy;
}
void pong_init_screen(void)
{
	display_init();
	dip204_clear_display();
	// ball no wall
/*
	dip204_create_char(disp_leftUp, charts_leftUp);
	dip204_create_char(disp_leftDown, charts_leftDown);
	dip204_create_char(disp_rightUp, charts_rightUp);
	dip204_create_char(disp_rightDown, charts_rightDown);
	// ball no wall
	dip204_create_char(disp_leftUp, charts_leftUp);
	dip204_create_char(disp_leftDown, charts_leftDown);
	dip204_create_char(disp_rightUp, charts_rightUp);
	dip204_create_char(disp_rightDown, charts_rightDown);
	char Wall[8] = {
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001
	};
	dip204_create_char('|',Wall);
	char Board[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b00000,
		0b11111,
		0b11111,
		0b11111,
		0b11111
	};
	dip204_create_char('-',Board);
	char Ball[8] = {
		0b11111,
		0b11111,
		0b11011,
		0b10001,
		0b10001,
		0b11011,
		0b11111,
		0b11111
	};
	dip204_create_char('0',Ball);
	dip204_set_cursor_position(1,1);
	dip204_write_data('|');
	dip204_set_cursor_position(2,1);
	dip204_write_data('-');
	dip204_set_cursor_position(3,1);
	dip204_write_data('0');
 */
}

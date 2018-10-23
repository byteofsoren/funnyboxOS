/*
 * snake.c
 *
 * Created: 2018-10-01 10:56:35
 *  Author: Magnus sörensen.
 */ 

#include "snake.h"

/***********************************
* Private function prototypes.
***********************************/
void update_game(usec_t time);
void show_game(usec_t time);
void left(int buttons);
void right(int buttons);
void up(int buttons);
void down(int buttons);

/***********************************
* Structs.
***********************************/
typedef struct _player_t{
	struct _player_t *next;
	int pos_x;
	int pos_y;	
	//int old_x;
	//int old_y;
	int dx;
	int dy;
}player_t;

typedef struct _pill_t{
	int posX;
	int posY;
	int on;
}pill_t;

/***********************************
* Global variables.
***********************************/
pid_t button_pid = 0;
pid_t game_pid= 0;
pid_t disp_pid=0;
struct _player_t *current_part;
#define GAME_SPEED 155

struct _player_t *player;
int game_satus=0;
struct _pill_t my_pills;

void snake_init(void)
{
	player = (player_t*) calloc(1, sizeof(player_t));
	player->pos_x =  8;
	player->pos_y = 2;
	player->dx = 0;
	player->dy = 0;
	player->next = NULL;
	//player->old_x = 0;
	//player->old_y = 0;
	// First pill inited behind the snake.
	my_pills.posX = 3;
	my_pills.posY = 2;
	my_pills.on = 1;

	game_satus=SNAKE_ALIVE;
	
	lib_buttons_t snake_buttons = {
		.onLeft = left,
		.onRight = right,
		.onUp = up,
		.onDown = down,
	};
	display_init();
	dip204_clear_display();
	//dip204_set_cursor_position(1,1);
	//dip204_write_string("Start-Snake");
	button_pid = lib_shedReg(lib_buttonsInit(&snake_buttons), GAME_SPEED / 8 );	
	game_pid = lib_shedReg(update_game, GAME_SPEED/2);
	disp_pid = lib_shedReg(show_game, GAME_SPEED/24);
	lib_timeOn();
	while (1){
		if (game_satus == SNAKE_DEAD)
		{
			//lib_timeOff();
		}
	}	
}



void update_game(usec_t time)
{
	// Check if snake eats pill at update.
	if (((player->pos_x + player->dx) == my_pills.posX )&& ((player->pos_y + player->dy)== my_pills.posY) && (my_pills.on==1))
	{
		// if so then grow length by adding adding a new node as head.
		struct _player_t *new_head = (player_t*) calloc(1, sizeof(player_t));
		new_head->dx = player->dx;
		new_head->dy = player->dy;
		new_head->pos_x = my_pills.posX;
		new_head->pos_y = my_pills.posY;
		new_head->next = player;	
		player = new_head;
		my_pills.on=0;
		// Create a new pill.
		my_pills.posX = time%20+1;
		my_pills.posY = time%4+1;
		my_pills.on=1;
	} else {
		// No pills eaten.
		// move body.
		// first move the head
		int preX=0,preY=0, oldX=0, oldY=0;
		preX =  player->pos_x;
		preY =  player->pos_y;
		player->pos_x += player->dx;
		player->pos_y += player->dy;
		if (player->next != NULL)
		{
			current_part = player->next;
			while (current_part != NULL)
			{
				oldX = current_part->pos_x;
				oldY = current_part->pos_y;
				// Move the current part to a previous position.
				current_part->pos_x = preX;
				current_part->pos_y = preY;
				preX = oldX;
				preY = oldY;
				current_part = current_part->next;
				// Check if head collides with body
				if ((player->pos_x == current_part->pos_x) && (player->pos_y == current_part->pos_y))
				{
					game_satus = SNAKE_DEAD;
				}
			}
			
			
		}
		
	
		
	}
	// move all boddy parts to new positions.
	// Check if head collides with screen.
	// if ether snake is deed.
	if (player->pos_x > 20 || player->pos_x < 1 || player->pos_y > 4 || player->pos_y < 1)
	{
		// out of screen
		player->pos_x =  8;
		player->pos_y = 2;
		player->dx = 1;
		player->dy = 0;
		player->next = NULL;	
		//player->old_x = 0;
		//player->old_y = 0;
		game_satus = SNAKE_DEAD;
	}	
	
}


void show_game(usec_t time)
{
	dip204_clear_display();
	if (game_satus == SNAKE_ALIVE)
	{
		//dip204_set_cursor_position(player->old_x,player->old_y);
		//dip204_write_string(" ");
		
		current_part = player;
		if (current_part->next != NULL)
		{
			do 
			{
				dip204_set_cursor_position(current_part->pos_x,current_part->pos_y);
				dip204_write_string("X");
				current_part = current_part->next;
			} while (current_part != NULL);
		} 
		else
		{
			// Snake length is one.
			dip204_set_cursor_position(player->pos_x,player->pos_y);
			dip204_write_string("X");
		}
		//dip204_set_cursor_position(player->pos_x,player->pos_y);
		//dip204_write_string("X");
		if (my_pills.on == 1)
		{
			dip204_set_cursor_position(my_pills.posX,my_pills.posY);
			dip204_write_string("O");
		}
		
	} else
	{
		// Snake is dead.
		dip204_set_cursor_position(1,1);
		dip204_write_string("Snake is dead.");
	}
}


/***********************************
* Events that happens on button.
***********************************/

void left(int buttons)
{
	if ((player->dx == 0) ||((player->dx == 0) && (player->dy == 0)))
	{
		player->dx = -1;
		player->dy = 0;
	}
}
void right(int buttons)
{
	if ((player->dx == 0) ||((player->dx == 0) && (player->dy == 0)))
	{
		player->dx = 1;
		player->dy = 0;
	}
}
void up(int buttons)
{
	if ((player->dy == 0) ||((player->dx == 0) && (player->dy == 0)))
	{
		player->dx = 0;
		player->dy = -1;
	}
}
void down(int buttons)
{
	if ((player->dy == 0) ||((player->dx == 0) && (player->dy == 0)))
	{
		player->dx = 0;
		player->dy = 1;
	}
}

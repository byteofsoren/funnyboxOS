/*
 * lib_menu.c
 *
 * Created: 2018-10-31 12:58:54
 *  Author: Magnus Sörensen
 */ 


typedef struct _lib_menu_t 
{
	struct _lib_menu_t *next_taask;		// Next struct in linked list
	menu_run task;				// The pointer to the task
}lib_menu_t;
//struct _lib_task_t* lib_task_head = NULL;
struct _lib_menu_t* menu_head = NULL;
struct _lib_menu_t* itterator = NULL;

int lib_menuRegister(menu_run, const char *description)
{
	//lib_task_t* new_task = (lib_task_t*) calloc(1,sizeof(lib_task_t));
	lib_menu_t* new_task = (lib_task_t*1) calloc(1, sizeof(lib_menu_t));
	
	
}
void lib_menuRun(void)
{
	while (1)
	{
	}
}

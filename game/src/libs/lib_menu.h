/*
 * lib_menu.h
 *
 * Created: 2018-10-31 12:59:32
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_MENU_H_
#define LIB_MENU_H_

#include "lib_common.h"
/******************************
* lib_menuRegister
*
* in: menu_run: A function to
* be called on button press.
* in: description: The text
* to be displayed in the menu.
* out: returns adding state.
******************************/
int lib_menuRegister(menu_run, const char *description);
/******************************
* lib_menuRun
* in none:
* out none:
******************************/
void lib_menuRun(void); 

#endif /* LIB_MENU_H_ */
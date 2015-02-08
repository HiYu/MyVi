#include<ncurses.h>
#include<ctype.h>
#include<stdlib.h>
#include"main.h"
#include"edit.h"
void insert(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag)
{
	int y,x;
	getyx(editWin,y,x);
	if(*flag == INS_MODEL && isprint(ch))
	{
		winsch(editWin,ch);
		wmove(editWin,y,++x);
	}
	if(*flag == COM_MODEL)
	{
		if(isdigit(ch) && isdigit(*prev))
		{
			*prev =	(*prev - '0') * 10 + ( ch - '0');
			mvprintw(20,30,"%d",*prev);
		}
		else
			*prev = ch;
	}
	if(ch == VK_ESCAPE && *flag == INS_MODEL)
		*flag = COM_MODEL;
}


int lineIsEmpty(WINDOW *editWin)
{
	int y,x,i = 0;
	getyx(editWin,y,x);
	if(x == 0)
	{
		while(i < COLS && isspace(winch(editWin)))
			wmove(editWin,y,++i);
		if(i >= COLS)
		{
			wmove(editWin,y,x);
			return 1;
		}
	}
	wmove(editWin,y,x);
	return 0;
}/*判断光标所在行是否为空*/

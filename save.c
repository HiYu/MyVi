#include<ncurses.h>
#include<stdlib.h>
#include<ctype.h>
#include"main.h"
#include"save.h"
#include"commend.h"
#include"edit.h"

int getLastLine(WINDOW *editWin,int maxy,int maxx)
{
	int i,j;
	wmove(editWin,maxy - 1,maxx - 1);
	for(i = maxy - 1;i >= 0;i--)
	{
		for(j = maxx - 1;j >= 0;j--)
		{
			if(isspace(winch(editWin)))
				wmove(editWin,i,j);
			else
				return i;
		}
	}
	return 0;
}
int getLastChar(WINDOW *editWin,int maxx)
{
	int y,x;
	getyx(editWin,y,x);
	x = maxx - 1;
	wmove(editWin,y,x);
	while(isspace(winch(editWin)) && x > 0)
		wmove(editWin,y,--x);
	wmove(editWin,y,0);
	return x;
}
char* save(WINDOW *editWin,FILE *fp,int lastLine)
{
	int ch;
	int lastChar,temp;
	int maxy,maxx,y,x;
	int line = 0,col = 0;
	fseek(fp,0L,SEEK_SET);
	getyx(editWin,y,x);
	getmaxyx(editWin,maxy,maxx); 
	if(fp != NULL)
	{
	    temp = getLastLine(editWin,maxy,maxx);
	    if(temp > lastLine)
	    	lastLine = temp;
		wmove(editWin,0,0);
		while(line <= lastLine)
		{
			col = 0;
			lastChar = getLastChar(editWin,maxx);
			if(lastChar == 0)
			lastChar = maxx - 1;
			while( col <= lastChar)
			{
				ch = winch(editWin);
				fputc(ch,fp);
				wmove(editWin,line,++col);
			}
			fputc('\n',fp);
			wmove(editWin,++line,0);
		}
		wmove(editWin,y,x);

	}else
		return "SAVE FAILED";
	return "SAVE SUCCESS!";
}

int isPrint(WINDOW *editWin,WINDOW *comWin,FILE *fp,int *prev,int ch,int *flag,int lastLine)
{
	int y,x;
	char *str;
	int quit = false ;
	getyx(comWin,y,x);
	
	if(*flag == COM_MODEL)
	{
		*flag = SAVE_MODEL;
		wmove(comWin,0,0);
		wclrtoeol(comWin);
		wrefresh(comWin);
		mvwprintw(comWin,0,0,"%c",ch);//打印冒号:
		while((ch = wgetch(comWin)) != VK_ENTER && *flag == SAVE_MODEL && quit == false)
		{
			switch(ch)
			{
				case 'w' : wmove(comWin,0,1);
						   wclrtoeol(comWin);
						   wrefresh(comWin);
						   waddch(comWin,ch);
						   str = save(editWin,fp,lastLine);
						   *prev = ch;
						   break;
				case 'q' : wmove(comWin,0,1);
						   wclrtoeol(comWin);
						   wrefresh(comWin);
						   str = "good bye";
						   waddch(comWin,ch);
						   quit = true;
						   break; 
						  
				case VK_ESCAPE : *flag = COM_MODEL;
								  break;
				case VK_ENTER : enterKey(comWin,flag);break;
				case KEY_BACKSPACE : backspaceDel(comWin,flag);break;
				default : wmove(comWin,0,1);
						  wclrtoeol(comWin);
						  wrefresh(comWin);
						  mvwprintw(comWin,0,1,"%c is not a commend!",ch);
			}
		}
		mvwprintw(comWin,0,1,"%s",str);
		wrefresh(comWin);
		*flag = COM_MODEL;
	}
	return quit;
}


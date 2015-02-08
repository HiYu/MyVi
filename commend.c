#include<ncurses.h>
#include<ctype.h>
#include<stdlib.h>
#include"main.h"
#include"commend.h"
#include"edit.h"

void isPrintI(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag)
{
	if(*flag == INS_MODEL)
		insert(editWin,comWin,prev,ch,flag);
	else
		*flag = INS_MODEL;
}
void commendDisplay(WINDOW *editWin,WINDOW *comWin)
{
	int editY,editX,comY,comX;
	int maxy,maxx;
	getyx(editWin,editY,editX);
	getyx(comWin,comY,comX);
	getmaxyx(comWin,maxy,maxx);
	//wclrtoeol(comWin);
	wmove(comWin,0,0);
	wclrtobot(comWin);
	wrefresh(comWin);
	mvwprintw(comWin,0,maxx-20,"(%d,%d)",(editX + 1),(editY + 1));
	wrefresh(comWin);
	wmove(editWin,editY,editX);

}

void delX(WINDOW *editWin,int *flag,int *buf)
{
	if(*flag == COM_MODEL)
	{
		*buf = winch(editWin);
		if(lineIsEmpty(editWin))
			wdeleteln(editWin);
		else
			wdelch(editWin);
	}
}

void delWord(WINDOW *editWin,int *prev,int *flag,int *buf)
{
	int i = 0;
	int y,x;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL)
	{
		if(*prev == 'd')
		{
			if(lineIsEmpty(editWin))
				wdeleteln(editWin);
			else
			{
				while(!isspace(winch(editWin)) && x > 0)
					wmove(editWin,y,--x);/*移动到单词的第一个字母*/
				if(x > 0)
				wmove(editWin,y,++x);
				while(!isspace(winch(editWin)))
				{
					buf[i++] = winch(editWin); 
					wdelch(editWin);
				}
				buf[i] = '\0';
				if(isspace(winch(editWin)))
					wdelch(editWin);
			}
			*prev = EOF;
		}
	}
}

void delLine(WINDOW *editWin,int *prev,int *flag,int *buf)
{
	int y,x,i = 0;
	int ch;
	if(*flag == COM_MODEL && *prev == 'd')
	{
		getyx(editWin,y,x);
		wmove(editWin,y,0);
		while(i < COLS - 1)
		{ 
			buf[i] = winch(editWin);
			wmove(editWin,y,++i);
		}
		buf[i] = winch(editWin);
		buf[i + 1] = '\0';
		wdeleteln(editWin);
		wmove(editWin,y,0);
		*prev = EOF;
	}
}

void delToEnd(WINDOW *editWin,int *prev,int *flag)
{
	int y,x,i;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL && *prev == 'd')
	{
		i = COLS - x;
		if(lineIsEmpty(editWin))
			wdeleteln(editWin);
	    else{ 
		 	while(i >= 0)
			{
				wdelch(editWin);
				--i;
			}
		}
		*prev = EOF;
	}
}


void copyWord(WINDOW *editWin,int *prev,int *flag,int buf[])
{
	int count = 0;
	int tempCh,tempX;
	int y,x;
	getyx(editWin,y,x);
	tempX = x;
	if(*flag == COM_MODEL && *prev == 'c')
	{
		while(!isspace(tempCh = winch(editWin)))
		{
			buf[count++] = tempCh;
			wmove(editWin,y,++tempX);
		}
		buf[count] = '\0';
		wmove(editWin,y,x);
		*prev = EOF;
	}
}

void copyLine(WINDOW *editWin,int *prev,int *flag,int buf[])
{
	int count = 0,i = 0;
	int y,x,tempX,temp = COLS;
	getyx(editWin,y,x);
	tempX = x;
	if(*flag == COM_MODEL && *prev == 'c')
	{
		tempX = 0;
		wmove(editWin,y,tempX);
		while(tempX < COLS)
		{
			buf[count++] = winch(editWin);
			wmove(editWin,y,++tempX);
		}
		buf[count] = '\0';
		wmove(editWin,y,x);
		*prev = EOF;
	}
}

void pasteWord(WINDOW *editWin,int *prev,int *flag,int buf[])
{
	int i = 0;
	int prevY,prevX;
	getyx(editWin,prevY,prevX);
	if(*flag == COM_MODEL && *prev == 'p')
	{
		while(buf[i] != '\0')
		{
			if(isspace(winch(editWin)))
			{
				waddch(editWin,buf[i]);
				wmove(editWin,prevY,++prevX);
			}else{
				winsch(editWin,buf[i]);
				wmove(editWin,prevY,++prevX);
			}
			i++;
		}
		*prev = EOF;
	}
}

void pasteLine(WINDOW *editWin,int *prev,int *flag,int buf[])
{
	int i = 0;
	int y,x;
	if(*flag == COM_MODEL && *prev == 'p')
	{
		winsertln(editWin);
		getyx(editWin,y,x);
		wmove(editWin,y,0);
		while(buf[i] != '\0')
			waddch(editWin,buf[i++]);
		*prev = EOF;
	}
}

void moveWord(WINDOW *editWin,int *prev,int *flag)
{
	int y,x;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL && *prev == 'm' && x < COLS)
	{
		while(!isspace(winch(editWin)))
			wmove(editWin,y,++x);
		if(isspace(winch(editWin)))
			wmove(editWin,y,++x);
		*prev = EOF;
	}
}/*光标移动到下一个单词的开头*/

void moveLineEnd(WINDOW *editWin,int *prev,int *flag)
{
	int y,x;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL && *prev == 'm' && x < COLS)
	{
		wmove(editWin,y,COLS - 1);
		*prev = EOF;
	}
}

void moveNum(WINDOW *editWin,int *prev,int *flag)
{
	int maxy,maxx;
	getmaxyx(editWin,maxy,maxx);
	if(*flag == COM_MODEL && isdigit(*prev))
	{
		if(*prev - '0' >= maxy)
		{
			wmove(editWin,maxy - '1',0);
		}
		else
			wmove(editWin,*prev - '1',0);
		*prev = EOF;
	}
}

void replaceChar(WINDOW *editWin,int *flag)
{
	if(*flag == COM_MODEL)
		waddch(editWin,wgetch(editWin));
}
void replaceLine(WINDOW *editWin,int *flag)
{
	int ch;
	if(*flag == COM_MODEL)
	{
		while((ch = wgetch(editWin)) != VK_ESCAPE)
			waddch(editWin,ch);
	}
}

void setBoldChar(WINDOW *editWin,int *flag)
{
	if(*flag == COM_MODEL)
	{
		wattron(editWin,A_BOLD);
		waddch( editWin,winch(editWin));
		wattroff(editWin,A_BOLD);
	}

}
void setBlodAndReplace(WINDOW *editWin,int *prev,int *flag)
{
	int ch;
	if(*flag == COM_MODEL && *prev == 'B')
	{
		wattron(editWin,A_BOLD);
		while((ch = wgetch(editWin)) != VK_ESCAPE)
			waddch(editWin,ch);
		wattroff(editWin,A_BOLD);
		*prev = EOF;
	}
}
void setBoldWord(WINDOW *editWin,int *prev,int*flag)
{
	int ch;
	int y,x;
	getyx(editWin,y,x);
	if(*flag == COM_MODEL && *prev == 'b')
	{
		while(!isspace(winch(editWin)) && x > 0)
			wmove(editWin,y,--x);/*移动到单词的第一个字母*/
		if(x > 0)
			wmove(editWin,y,++x);
		wattron(editWin,A_BOLD);
		while(!isspace(ch = winch(editWin)))
			waddch(editWin,ch);	
		wattroff(editWin,A_BOLD);
		*prev = EOF;
	}
}
 void insertLineUnder(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag)
{
	int y,x;
	if(*flag == COM_MODEL)
	{
		getyx(editWin,y,x);
		wmove(editWin,y + 1,0);
		winsertln(editWin);
		*flag = INS_MODEL;
	   	/*因为此处将flag设置为INS_MODE模式，这样在main函数中insert将执行插入操作，所以会将命令显示出来 */
	}else
		insert(editWin,comWin,prev,ch,flag);
}
void insertLineAbove(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag)
{
	int y,x;
	if(*flag == COM_MODEL)
	{
		getyx(editWin,y,x);
		wmove(editWin,y - 1,0);
		winsertln(editWin);
		*flag = INS_MODEL;

	}else
		insert(editWin,comWin,prev,ch,flag);
}
void setHighLight(WINDOW *editWin,int *flag)
{
	if(*flag == COM_MODEL)
	{
		init_pair(1,COLOR_RED,COLOR_WHITE);
		wattron(editWin,COLOR_PAIR(1));
		waddch(editWin,winch(editWin));
		wattroff(editWin,COLOR_PAIR(1));
	}
}
void backspaceDel(WINDOW *win,int *flag)
{
	int ch;
	int count;
	int y,x;	
	int maxy,maxx;
	int buf[LINE_BUF_NUM];
	getmaxyx(win,maxy,maxx);
	getyx(win,y,x);
	if(*flag == COM_MODEL)
	{
		if(x > 0)
			wmove(win,y,--x);
		else{
			if(y - 1 >= 0)
			{
				wmove(win,--y,maxx - 1);
				getyx(win,y,x);
				while(isspace(winch(win)) && x > 0 )
					wmove(win,y,--x);/*从行末向前移动直到倒数第一个不为空格的字符*/		
			}else
				wmove(win,0,0);
		
		}
	}else if(*flag == INS_MODEL)
	{
		/*if(lineIsEmpty(win))
			wdeleteln(win);	*/
		if( x > 0)
		{
			wmove(win,y,x - 1);
			wdelch(win);
			wmove(win,y,--x);
		}else{
			if(y - 1 >= 0)
			{
				wmove(win,y,maxx - 1);
				getyx(win,y,x);
				while(isspace(winch(win)) && x > 0)
					wmove(win,y,--x);/*从行末向前移动直到倒数第一个不为空格的字符*/
				if(!isspace(winch(win)))
				{
					count = x;
					while(x >= 0)
					{
						buf[x] = winch(win);
						wmove(win,y,--x);
					}
					buf[count + 1] = '\0';
				}/*将该行数据读入到缓冲区*/

				wmove(win,--y,maxx - 1);
				getyx(win,y,x);
				while(isspace(winch(win)) && x > 0)
					wmove(win,y,--x);
				if(!isspace(winch(win)))
				wmove(win,y,++x);
				if(count > 0 && count < maxx - x)/*当上一行有足够的空间来存放下一行剩余的字符则将这些字符移动到上一行,否则不移动*/
				{ 
					int i = 0;
					while(buf[i] != '\0')
					{
						waddch(win,buf[i]);
						i++;
					}
					getyx(win,y,x);
					wmove(win,y + 1,0);
					wdeleteln(win);
					wmove(win,y,x - count - 1);
				}

			}else
				wmove(win,0,0);
		}
	}else{/*SAVE_MODEL*/
		wdelch(win);
		if(x > 0)
			wmove(win,y,--x);
		else
			*flag = COM_MODEL;
	}
}

void enterKey(WINDOW *win,int *flag)
{
	int buf[LINE_BUF_NUM];
	int y,x;
	getyx(win,y,x);
	if(*flag == COM_MODEL)
		wmove(win,++y,0);
	else if(*flag == INS_MODEL) 
	{
		int i = 0,j = 0,count,maxy,maxx;
		getmaxyx(win,maxy,maxx);
		count = maxx - x;
		while(count > 0)
		{
			buf[i++] = winch(win);
			wdelch(win);
			count--;
		}
		buf[i] = '\0';
		wmove(win,++y,0);
		winsertln(win);
		while(buf[j] != '\0')
		{
			waddch(win,buf[j]);
			j++;
		}
		wmove(win,y,0);
	}
	
}

void tabKey(WINDOW *editWin,int *flag)
{
	int i;
	int y,x;
	getyx(editWin,y,x);
	if(*flag == INS_MODEL)
		for(i = 0;i < 4;i++)
		{ 
			winsch(editWin,' ');
			wmove(editWin,y,++x);
		}
}


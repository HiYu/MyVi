/*保存退出模式*/
#ifndef Save_h
#define Save_h
int getLastLine(WINDOW *editWin,int maxy,int maxx);
int getLastChar(WINDOW *editWin,int maxx);
char* save(WINDOW *editWin,FILE *fp,int lastLine);
int isPrint(WINDOW *editWin,WINDOW *comWin,FILE *fp,int *prev,int ch,int *flag,int lastLine);
#endif

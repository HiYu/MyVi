/*本头文件包括编辑模式下的相关操作*/
#ifndef Edit_h
#define Edit_h
void insert(WINDOW *editWin,WINDOW *comWin,int *prev,int ch,int *flag);/*又命令模式切换到编辑模式，以及编辑模式的编辑操作*/
int lineIsEmpty(WINDOW *editWin);/*判断光标所在行是否为空，用于删除操作时，空行整体删除*/
#endif

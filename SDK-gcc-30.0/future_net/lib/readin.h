#include "stdio.h"
#include"stdlib.h"
#include"cstring"
#define MAX  8000
#define NUM 600
extern int d_num ;//要求必过点数量
extern int totalline;
extern int start;
extern int dest;
extern int Data[4800][4];
extern int d_Data[50];
int Get_Data(const char*filename);
int Get_D_Data(const char*filename);

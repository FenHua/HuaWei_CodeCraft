#include"iostream"
extern int **D_set;
extern int **sort_set;
extern bool *D_isvisited;// ����һ�����飬�洢���ʹ��ĵ�
extern bool *D_ismust;
extern bool **D_used;//�����ά����洢�������ı�
extern int T_path[50][50][600];
void Dijkstra(int n, int v, int *dist, int *prev, int**c);
void searchPath();
int Dijk_set(int d_set[], int d_num, int n, int *dist, int *prev, int**c);
void Dijk_state();
void Label_Sort();
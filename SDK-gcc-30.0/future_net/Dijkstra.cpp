#include"readin.h"
#include"graph_init.h"
#include"dijkstra.h"
using namespace std;
int **D_set;
int T_path[50][50][600];
int **sort_set;
bool *D_ismust;
bool **D_used;//定义二维数组存储遍历过的边
void Dijkstra(int node_num, int source, int *PathLength, int *pioneer,int **AdjacentMatrix)
{
	//标记顶点是否在集合S中，如果是，则值为1，否为0
	bool sign[NUM];
	for (int i = 0; i<node_num; i++)
	{
		PathLength[i] = AdjacentMatrix[source][i];//初始化源点到其他各个顶点的最短路径长度
		sign[i] = false;//标记是否被访问
		//满足条件，说明顶点i与源点不相邻
		if (PathLength[i] ==8000)
		{
			pioneer[i] = -1;
		}
		else
		{
			pioneer[i] = source;
		}
	}
	//初始化时，集合S中只有一个元素，源点
	PathLength[source] = 0;//起点的长度为0，标志为true
	sign[source] = true;
	for (int i = 0; i<node_num; i++)
	{
		int temp =8000;
		int t = source;
		for (int j = 0; j<node_num; j++)
		{
			if ((!sign[j]) && (PathLength[j]<temp))
			{
				t = j;
				temp = PathLength[j];
			}
		}
		if (t == source)
			break;
		sign[t] = true;
		for (int j = 0; j<node_num; j++)
		{
			if ((!sign[j]) && (AdjacentMatrix[t][j] !=8000))
			{
				if (PathLength[j]>(PathLength[t] + AdjacentMatrix[t][j]))
				{
					PathLength[j] = PathLength[t] + AdjacentMatrix[t][j];
					pioneer[j] = t;
				}
			}
		}
	}
}
int searchPath(int *pineer, int v, int i, int T_points[])
{
	int j = 0;
	int t=0;
	int temp[NUM];
	int dest = i;
	while (pineer[i] != -1)
	{
		//反向的
		temp[j] = pineer[i];
		j++;
		i = pineer[i];
	}
	for (int k = j - 1; k >=0; k--)
	{
		T_points[t] = temp[k];
		t++;
	}
	T_points[t] = dest;
	return t+1;
}
int Dijk_set(int d_set[], int d_num, int n,int *dist, int *prev, int**c)
{
	D_set = new int*[d_num];
	for (int i = 0; i < d_num; i++)
	{
		D_set[i] = new int[d_num];
	}
	for (int i = 0; i < d_num; i++)
	{
		Dijkstra(n, d_set[i], dist, prev, c);//下坐标对应的必须点为起点寻找到其它必须点的距离
		for (int j = 0; j < d_num; j++)
		{
			int k, T_points[600];
			D_set[i][j] = dist[d_set[j]];//记录两两必须点之间的距离
			k=searchPath(prev,d_set[i],d_set[j],T_points);
			T_path[i][j][0] = k;
			for (int s = 1; s <= k; s++)
			{
				T_path[i][j][s] = T_points[(s-1)];
			}
		}
	}
	return 0;
}
void Dijk_state()
{
	D_ismust = new bool[d_num + 2];
	for (int i = 0; i <d_num +2; i++)
	{
		D_ismust[i] =true;
	}
	D_ismust[0] = false;
	D_ismust[d_num + 1] = false;
	D_used = new bool*[d_num + 2];
	for (int i = 0; i <d_num + 2; i++)
	{
		D_used[i] = new bool[d_num + 2];
	}
	for (int i = 0; i <d_num + 2; i++)
	{
		for (int j = 0; j <d_num + 2; j++)
			D_used[i][j] = false;//记录遍历的边
	}
}
/***经调试速度测得**************/
void Label_Sort()
{
	int min_label = 0;
	sort_set = new int*[d_num + 2];
	for (int i = 0; i < d_num + 2; i++)
	{
		sort_set[i] = new int[d_num + 2];
	}
	int*a = new int[d_num + 2];
	for (int i = 0; i < d_num + 2; i++)
	{
		for (int k = 0; k < d_num + 2; k++)
		{
			a[k] = T_path[i][k][0]+D_set[i][k];
		}
		for (int s = 0; s < d_num + 2; s++)
		{
			int min =1008000;
			for (int j = 0; j<d_num + 2; j++)
			{
				if (min>=a[j])
				{
					min = a[j];
					min_label = j;
				}
			}
			a[min_label] =1008000;
			sort_set[i][s] = min_label;
		}
	}
}

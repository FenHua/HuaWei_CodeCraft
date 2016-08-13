#include"readin.h"
#include"graph_init.h"
#include"iostream"
int node_num = 0;
int a[NUM];
using namespace std;
void Init_Graph(Graph &G)
{
	/***********************计算得到顶点数***************/
	for (int i = 0; i < totalline; i++)
	{
		if (Data[i][1]>Data[i][2])
		{
			int temp_max = Data[i][1];
			if (node_num > temp_max)
			{
				node_num = node_num;
			}
			else
			{
				node_num = node_num;
			}
		}
		else
		{
			int temp_max = Data[i][2];
			if (node_num > temp_max)
			{
				node_num = node_num;
			}
			else
			{
				node_num = temp_max;
			}
		}
	}
	/***********初始化图信息**************/
	G.ledge = new int*[NUM];
	G.legde_cost = new int*[NUM];
	for (int i = 0; i <NUM; i++)
	{
		G.ledge[i] = new int[NUM];
		G.legde_cost[i] = new int[NUM];
	}
	for (int i = 0; i <NUM; i++)
	{
		for (int j = 0; j <NUM; j++)
		{
			G.ledge[i][j] = 5000;
			G.legde_cost[i][j] = 8000;
		}
	}
	for (int i = 0; i < totalline; i++)
	{
		G.ledge[Data[i][1]][Data[i][2]] = Data[i][0];
		G.legde_cost[Data[i][1]][Data[i][2]] = Data[i][3];
	}
	G.isvisited = new bool[node_num + 1];
	G.ismust = new bool[node_num + 1];
	for (int i = 0; i <node_num + 1; i++)
	{
		G.isvisited[i] = false;//记录被访问过的点
		G.ismust[i] = false;
	}
	for (int i = 0; i <d_num; i++)
	{
		G.ismust[d_Data[i]] = true;//必须经过的点标记为true
	}
	G.used = new bool*[node_num + 1];
	for (int i = 0; i < node_num + 1; i++)
	{
		G.used[i] = new bool[node_num + 1];
	}
	for (int i = 0; i <node_num + 1; i++)
	{
		for (int j = 0; j <node_num + 1; j++)
			G.used[i][j] = false;//记录遍历的边
	}
}
/**************图剪枝******************/
int prune(Graph&g,Prune_graph&P_g,int node_num)
{
	P_g.prune_cost = new int*[node_num];
	P_g.path_knum = new int*[node_num];
	for (int i = 0; i <node_num; i++)
	{
		P_g.prune_cost[i] = new int[node_num];
		P_g.path_knum[i] = new int[node_num];
	}
	for (int i = 0; i < node_num; i++)
	{
		for (int j = 0; j < node_num; j++)
		{
			P_g.prune_cost[i][j] = g.legde_cost[i][j];
			P_g.path_knum[i][j] = 0;
		}
	}
	P_g.prune_path = new int**[node_num];
	for (int i = 0; i < node_num; i++)
	{
		P_g.prune_path[i] = new int*[node_num];
		for (int j = 0; j < node_num; j++)
		{
			P_g.prune_path[i][j] = new int[node_num];
		}
	}
	int index = 0;
	while (index < node_num)
	{
		if ((g.ismust[index] == true)||(index==start)||(index==dest))
		{
			index++;
			continue;
		}
		else
		{
			for (int i = 0; i < node_num; i++)
			{
				if (P_g.prune_cost[i][index] != MAX)
				{
					for (int j = 0; j < node_num; j++)
					{
						if (P_g.prune_cost[index][j] != MAX)
						{
							if (P_g.prune_cost[i][j]>P_g.prune_cost[i][index] + P_g.prune_cost[index][j])
							{
								P_g.prune_cost[i][j] = P_g.prune_cost[i][index] + P_g.prune_cost[index][j];
								P_g.path_knum[i][j] = P_g.path_knum[i][index] + P_g.path_knum[index][j] + 1;
								int k;
								for (k = 0; k<P_g.path_knum[i][index]; k++)
								{
									P_g.prune_path[i][j][k] = P_g.prune_path[i][index][k];
								}
								P_g.prune_path[i][j][k] = index;
								for (k = P_g.path_knum[i][index] + 1; k <P_g.path_knum[i][j]; k++)
								{
									P_g.prune_path[i][j][k] = P_g.prune_path[index][j][(k - P_g.path_knum[i][index] - 1)];
								}
							}
						}
					}
				}
				P_g.prune_cost[i][index] = MAX;
				P_g.path_knum[i][index] = 0;
			}
			for (int j = 0; j < node_num; j++)
			{
				if (P_g.prune_cost[index][j] != MAX)
				{
					P_g.prune_cost[index][j] = MAX;
					P_g.path_knum[index][j] = 0;
				}
			}
			index++;
		}
	}
	return 0;
}

void P_Label_Sort(Prune_graph&P_g, int node_num)
{
	int min_label = 0;
	P_g.sort_set = new int*[node_num];
	for (int i = 0; i <node_num; i++)
	{
		P_g.sort_set[i] = new int[node_num];
	}
	for (int i = 0; i <node_num; i++)
	{
		for (int k = 0; k <node_num; k++)
		{
			a[k] = P_g.path_knum[i][k]*10000+P_g.prune_cost[i][k];
		}
		for (int s = 0; s <node_num; s++)
		{
			int min = 1008000;
			for (int j = 0; j<node_num; j++)
			{
				if (min>a[j])
				{
					min = a[j];
					min_label = j;
				}
			}
			a[min_label] = 1008000;
			P_g.sort_set[i][s] = min_label;
		}
	}
}
void optimal_sort(int node_num, Prune_graph&P_g)
{
	
	int min_label;
	for (int k = 0; k <node_num; k++)
	{
		a[k] =P_g.path_knum[start][k]*10000+P_g.prune_cost[start][k];
	}
	for (int s = 0; s <node_num; s++)
	{
		int min = 1008000;
		for (int j = 0; j<node_num; j++)
		{
			if (min>a[j])
			{
				min = a[j];
				min_label = j;
			}
		}
		a[min_label] = 1008000;
		P_g.sort_set[start][s] = min_label;
	}
	for (int i = 0; i < (node_num)/2;i++)
	{
		int temp =P_g.sort_set[start][i];
		P_g.sort_set[start][i] =P_g.sort_set[start][node_num-1- i];
		P_g.sort_set[start][node_num-1- i] = temp;
	}
}


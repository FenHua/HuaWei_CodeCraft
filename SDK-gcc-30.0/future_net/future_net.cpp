#include<string>
#include"readin.h"
#include"dijkstra.h"
#include"graph_init.h"
#include"iostream"
using namespace std;
string str_route;
int total_cost = 0;
inline bool h_visited(int i,int j,Graph&g)
{
	for (int k =2; k <=T_path[i][j][0]; k++)
	{
		if (g.isvisited[T_path[i][j][k]] == true)
		{
			return true;
			break;
		}
	}
	return false;
}
inline bool p_visited(int i, int j, Graph&g,Prune_graph&P_g)
{
	if (g.isvisited[j] == true)
		return true;
	for (int k = 0; k <P_g.path_knum[i][j]; k++)
	{
		if (g.isvisited[P_g.prune_path[i][j][k]] == true)
		{
			return true;
			break;
		}
	}
	return false;
}
inline void write_visited(int i, int j, Graph&g)
{
	for (int k =1; k <=T_path[i][j][0]; k++)
	{
		g.isvisited[T_path[i][j][k]] = true;
	}
}
inline void P_write_visited(int i, int j, Graph&g, Prune_graph&P_g)
{
	g.isvisited[j] = true;
	g.isvisited[i] = true;
	for (int k = 0; k < P_g.path_knum[i][j]; k++)
	{
		g.isvisited[P_g.prune_path[i][j][k]] = true;
	}
}
inline void off_visited(int i, int j, Graph&g)
{
	for (int k =1; k <=T_path[i][j][0]; k++)
	{
		g.isvisited[T_path[i][j][k]] = false;
	}
}
inline void P_off_visited(int i, int j, Graph&g, Prune_graph&P_g)
{
	for (int s = 0; s < P_g.path_knum[i][j]; s++)
	{
		g.isvisited[P_g.prune_path[i][j][s]] =false;
	}
	g.isvisited[j] =false;
}
inline string print_route(int i, int j,Graph&g)
{
	string temp;
	char T[12];
	for (int k = 1; k <T_path[i][j][0]; k++)
	{
		sprintf(T, "%d", g.ledge[T_path[i][j][k]][T_path[i][j][k+1]]);
		temp.append(T);
		if (k != T_path[i][j][0] - 1)
		{
			temp.append("|");
		}
	}
	return temp;
}
inline string P_print_route(int i, int j, Graph&g, Prune_graph&P_g)
{
	string temp;
	char T[12];
	int *seq = new int[P_g.path_knum[i][j] + 2];
	seq[0] = i;
	for (int k = 1; k < P_g.path_knum[i][j] +1; k++)
	{
		seq[k] = P_g.prune_path[i][j][k -1];
	}
	seq[P_g.path_knum[i][j] + 1] = j;
	for (int k = 0; k < P_g.path_knum[i][j] + 1; k++)
	{
		sprintf(T, "%d", g.ledge[seq[k]][seq[k+1]]);
		temp.append(T);
		if (k != P_g.path_knum[i][j])
		{
			temp.append("|");
		}
	}
	return temp;
}
inline void write_result(const char * const filename)
{
	if (str_route.length() == 0)//保存的路径
		return;
	else
	{
		FILE *fp = fopen(filename, "w");
		if (fp == NULL)
		{
			cout << "writing failing!" << endl;
			return;
		}
		const char *ch = str_route.c_str();
		fputs(ch, fp);
		fputs("\n", fp);
		fclose(fp);
	}
}
string Search_Route1(int start, int end, Graph&g, int num, char*save_path)
{
	int k = 1;
	int graph_num = num + 1;
	int *path_node = new int[graph_num + 1];//定义数组存储遍历路径
	//之所以加1，因为从k=1开始，否则会发生写覆盖，导致heap corruption detected error，释放错误
	string optimal_path;//最佳路径
	total_cost = MAX;//记录长度
	*(path_node + k) = start;//从起点开始
	g.isvisited[start] = true;
	int count = 0;//用来记录是否遍历了所有必须点
	while (k)
	{
		int i = *(path_node + k);
		int j = 0;
		while (j <graph_num)
		{
			if (g.legde_cost[i][j] <8000 && !g.isvisited[j] && !g.used[i][j])//如果节点未被访问，相应边存在，且没有被访问
			{
				g.isvisited[j] = true;
				g.used[i][j] = true;
				k++;
				*(path_node + k) = j;//将此点放入记录路径
				if (g.ismust[j] == true)
				{
					count++;
				}
				break;
			}
			else
			{
				j++;
			}
		}
		if (j == graph_num || *(path_node + k) == end)// 如果没有路可走的时候，或者走到结束点的时候，要弹栈
		{
			if ((*(path_node + k) == end) && (count == d_num))//满足要求
			{
				string temp;
				char T[12];
				int tempWeight = 0;
				for (int z = 1; z <k; z++)
				{
					sprintf(T, "%d", g.ledge[*(path_node + z)][*(path_node + z + 1)]);
					temp.append(T);
					tempWeight += g.legde_cost[*(path_node + z)][*(path_node + z + 1)];
					if (z != k - 1)
					{
						temp.append("|");
					}
				}
				if (tempWeight <total_cost)//得最优解
				{
					total_cost = tempWeight;
					optimal_path = temp;
					str_route = optimal_path;
					write_result(save_path);
					cout << str_route << endl;
					cout << total_cost << endl;
				}
			}
			int topNode = *(path_node + k);//如果遍历结束未满足条件
			k--;
			g.isvisited[topNode] = false;
			for (int s = 0; s <graph_num; s++)
			{
				g.used[topNode][s] = false;
			}
			if (g.ismust[topNode] == true)
				count--;
		}
	}
	delete path_node;
	return optimal_path;
}
void Search_Route2(int start, int end, Graph&g, int **edge_cost, int mustdata[], int num, char*save_path)
{
	int k = 1;
	int counter=0;
	int graph_num = num + 1;
	int *path_node = new int[graph_num + 1];//定义数组存储遍历路径
	//之所以加1，因为从k=1开始，否则会发生写覆盖，导致heap corruption detected error，释放错误
	string optimal_path;//最佳路径
	total_cost = MAX;//记录长度
	*(path_node + k) = 0;//从起点开始
	g.isvisited[mustdata[0]] = true;
	int count = 0;//用来记录是否遍历了所有必须点
	while (k)
	{
		int i = *(path_node + k);
		int j = 0;
		while (j < graph_num)
		{
			bool temp_visited = h_visited(i, (sort_set[i][j]), g);
			if ((!temp_visited) && (!g.isvisited[mustdata[sort_set[i][j]]]) && (edge_cost[i][(sort_set[i][j])]>0 && edge_cost[i][(sort_set[i][j])] < 8000) && (!D_used[i][(sort_set[i][j])]))//如果节点未被访问，相应边存在，且没有被访问
			{
				g.isvisited[mustdata[sort_set[i][j]]] = true;
				D_used[i][(sort_set[i][j])] = true;
				write_visited(i, (sort_set[i][j]), g);
				k++;
				*(path_node + k) = (sort_set[i][j]);//将此点放入记录路径
				if (D_ismust[(sort_set[i][j])] == true)
					count++;
				break;
			}
			else
			{
				j++;
			}
		}
		if (j == graph_num || (mustdata[*(path_node + k)] == end))// 如果没有路可走的时候，或者走到结束点的时候，要弹栈
		{
			if ((mustdata[*(path_node + k)] == end) && count == d_num)//满足要求
			{
				string temp;
				int tempWeight = 0;
				for (int z = 1; z < k; z++)
				{
					temp.append(print_route(*(path_node + z), *(path_node + z + 1), g));
					tempWeight += D_set[*(path_node + z)][*(path_node + z + 1)];
					if (z != k - 1)
					{
						temp.append("|");
					}
				}
				if (tempWeight < total_cost)//得最优解
				{
					total_cost = tempWeight;
					optimal_path = temp;
					str_route = optimal_path;
					write_result(save_path);
					cout << str_route << endl;
					cout << total_cost << endl;
				}
			}
			if (k==17)
			{
				counter++;
			}
			if (counter==5)
			{
				counter = 0;//回溯计数标志置0
				//全部出栈—————请教师兄
				while (*(path_node+k)!= 0)
				{
					// 弹栈，将标志位复位
					int topNode=*(path_node + k);
					k--;
					g.isvisited[mustdata[topNode]] = false;
					if (D_ismust[topNode] == true)
					{
						count--;
					}
					for (int s=0;s<graph_num;s++)
					{
						D_used[topNode][k] = false;
					}
					if (k>0)
					{
						off_visited(*(path_node + k), topNode, g);
					}
				}
			}
			else
			{
				int topNode = *(path_node + k);//如果遍历结束未满足条件
				k--;
				g.isvisited[mustdata[topNode]] = false;
				for (int s = 0; s < graph_num; s++)
				{
					D_used[topNode][s] = false;
				}
				if (k>0)
				{
					off_visited(*(path_node + k), topNode, g);
				}
				if (D_ismust[topNode] == true)
				{
					count--;
				}
			}	
		}
	}
	delete path_node;
}
void Search_Route3(int start, int end, Graph&g,Prune_graph&P_g,int num, char*save_path)
{
	int k = 1;
	int graph_num = num + 1;
	int *path_node = new int[graph_num + 1];//定义数组存储遍历路径
	//之所以加1，因为从k=1开始，否则会发生写覆盖，导致heap corruption detected error，释放错误
	string optimal_path;//最佳路径
	total_cost = MAX;//记录长度
	*(path_node + k) =start;//从起点开始
	g.isvisited[start] = true;
	int count = 0;//用来记录是否遍历了所有必须点
	while (k)
	{
		int i = *(path_node + k);
		int j = 0;
		while (j < graph_num)
		{
			bool P_temp = p_visited(i,(P_g.sort_set[i][j]), g, P_g);
			if ((!P_temp) && (P_g.prune_cost[i][(P_g.sort_set[i][j])]>0 && P_g.prune_cost[i][(P_g.sort_set[i][j])] < 8000) && !g.used[i][(P_g.sort_set[i][j])])//如果节点未被访问，相应边存在，且没有被访问
			{
				g.isvisited[i] = true;
				g.used[i][(P_g.sort_set[i][j])] = true;
				P_write_visited(i, (P_g.sort_set[i][j]), g, P_g);
				k++;
				*(path_node + k) = (P_g.sort_set[i][j]);//将此点放入记录路径
				if (g.ismust[(P_g.sort_set[i][j])] == true)
					count++;
				break;
			}
			else
			{
				j++;
			}
		}
		if (j == graph_num || (*(path_node + k) == end))// 如果没有路可走的时候，或者走到结束点的时候，要弹栈
		{
			if ((*(path_node + k)== end) && count == d_num)//满足要求
			{
				string temp;
				int tempWeight = 0;
				for (int z = 1; z <k; z++)
				{
					temp.append(P_print_route(*(path_node + z), *(path_node + z + 1), g,P_g));
					tempWeight += P_g.prune_cost[*(path_node + z)][*(path_node + z+1)];
					if (z != k - 1)
					{
						temp.append("|");
					}
				}
				if (tempWeight < total_cost)//得最优解
				{
					total_cost = tempWeight;
					optimal_path = temp;
					str_route = optimal_path;
					write_result(save_path);
					cout << str_route << endl;
					cout << total_cost << endl;
				}
			}
			int topNode = *(path_node + k);//如果遍历结束未满足条件
			g.isvisited[topNode] = false;
			k--;
			for (int s = 0; s < graph_num; s++)
			{
				g.used[topNode][s] = false;
			}
			if (k>0)
			{
				P_off_visited(*(path_node + k), topNode, g,P_g);
			}
			if (g.ismust[topNode] == true)
			{
				count--;
			}
		}
	}
	delete path_node;
}
int main(int argc, char *argv[])
{
	char*load_path = argv[1];
	char*demand_path = argv[2];
	char*save_path = argv[3];
	int dist[NUM] = { 0 };// 表示当前点到源点的最短路径长度
	int prev[NUM] = { 0 };// 记录当前点的前一个结点
	Get_D_Data(demand_path);
    	Get_Data(load_path);
	Graph g;
	Prune_graph PG;
	Init_Graph(g);
	int mustdata[52];
	mustdata[0] = start;
	mustdata[d_num + 1] = dest;
	for (int i = 0; i <d_num; i++)
	{
		mustdata[i + 1] = d_Data[i];
	}
	Dijk_set(mustdata, (d_num + 2), (node_num + 1), dist, prev, g.legde_cost);
	Dijk_state();
	Label_Sort();
	Search_Route2(start, dest, g, D_set, mustdata, (d_num + 1), save_path);
	delete D_ismust;
	delete D_set;
	delete sort_set;
	/*
	if (totalline <= 100)
	{
		str_route = Search_Route1(start, dest, g, node_num, save_path);
	}
	else if ((1500>=totalline)&&(totalline>300))
	{
		int mustdata[52];
		mustdata[0] = start;
		mustdata[d_num + 1] = dest;
		for (int i = 0; i <d_num; i++)
		{
			mustdata[i + 1] = d_Data[i];
		}
		Dijk_set(mustdata, (d_num + 2), (node_num + 1), dist, prev, g.legde_cost);
		Dijk_state();
		Label_Sort();
		Search_Route2(start, dest, g, D_set, mustdata, (d_num + 1), save_path);
		delete D_ismust;
		delete D_isvisited;
		delete D_set;
		delete D_used;
		delete sort_set;
	}
	else
	{
		prune(g, PG, node_num + 1);
		P_Label_Sort(PG, node_num + 1);
		optimal_sort(node_num + 1, PG);
		if ((d_num<23) && (totalline<2200))
		{
			for (int i = 0; i < (node_num + 1) / 2; i++)

			{

				int temp = PG.sort_set[start][i];
				PG.sort_set[start][i] = PG.sort_set[start][node_num - i];
				PG.sort_set[start][node_num - i] = temp;
			}
			Search_Route3(start, dest, g, PG, node_num, save_path);
			delete PG.path_knum;
			delete PG.prune_cost;
			delete PG.prune_path;
			delete PG.sort_set;

		}
		else
		{
			Search_Route3(start, dest, g, PG, node_num, save_path);
			delete PG.path_knum;
			delete PG.prune_cost;
			delete PG.prune_path;
			delete PG.sort_set;
		}
	}
	*/
	delete g.isvisited;
	delete g.used;
	delete g.ledge;
	delete g.legde_cost;
	return 0;
}

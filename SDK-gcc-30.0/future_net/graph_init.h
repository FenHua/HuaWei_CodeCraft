extern int node_num;
struct Graph
{
	bool *isvisited;// 定义一个数组，存储访问过的点
	bool *ismust;
	int **ledge;//存储边序号
	int **legde_cost;
	bool **used;//定义二维数组存储遍历过的边
};
struct Prune_graph
{
	int **sort_set;
	int**prune_cost;
	int***prune_path;
	int**path_knum;
};
int prune(Graph&g,Prune_graph&P_g,int node_num);
void Init_Graph(Graph &G);
void P_Label_Sort(Prune_graph&P_g, int node_num);
void optimal_sort(int node_num, Prune_graph&P_g);
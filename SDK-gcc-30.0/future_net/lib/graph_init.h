extern int node_num;
struct Graph
{
	bool *isvisited;// ����һ�����飬�洢���ʹ��ĵ�
	bool *ismust;
	int **ledge;//�洢�����
	int **legde_cost;
	bool **used;//�����ά����洢�������ı�
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
/**
 * 实现代码文件
 * 
 * @author XXX
 * @since 2016-3-4
 * @version V1.0
 */
package com.routesearch.route;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Stack;

import com.filetool.util.FileUtil;

public final class Route
{
    public static String searchRoute(String filePath, String graphContent, String condition)
    {
    	int startNode = 0;                     // 起始点
    	int endNode = 0;                       // 结束点
    	// 图的顶点最大编号
    	int MAX_VERTEX = 0;
    	// 标记最大边
    	int MAX_PATH = 0;
    	
    	/***********************************************************/
    	// 按"/n"拆分字符串
    	String[] arr= graphContent.split("\n");
    	// 取出图顶点的最大编号
    	for (int i = 0; i < arr.length; i++)
    	{
    		String[] temp_str = arr[i].split(",");
    		int temp_pathNumber = Integer.parseInt(temp_str[0]);
    		int temp_start = Integer.parseInt(temp_str[1]);
    		int temp_end = Integer.parseInt(temp_str[2]);
    		if (temp_start > MAX_VERTEX)
			{
				MAX_VERTEX = temp_start;
			}
    		if (temp_end > MAX_VERTEX)
			{
    			MAX_VERTEX = temp_end;
			}
    		if (temp_pathNumber >= MAX_PATH )
			{
				MAX_PATH = temp_pathNumber;
			}
    	}
    	
//    	System.out.println("打印最大编号："+MAX_VERTEX);
    	
    	// 图的邻接矩阵
    	int[][] graph = new int[MAX_VERTEX+1][MAX_VERTEX+1];  
    	// 图的路径编号
    	int[][] pathNumber = new int[MAX_VERTEX+1][MAX_VERTEX+1];
    	
    	/***********************************************************/
    	// 给数组赋初值，都为最大值
    	for (int i = 0; i < graph.length; i++)
		{
			for (int j = 0; j < graph.length; j++)
			{
				graph[i][j] = Integer.MAX_VALUE;   
			}
		}
    	
    	// 填充邻接表和路径编号
    	for (int i = 0; i < arr.length; i++)
		{
    		String[] tempArr = arr[i].split(",");
    		int edgeNum = Integer.parseInt(tempArr[0]);
    		int start = Integer.parseInt(tempArr[1]);
    		int end = Integer.parseInt(tempArr[2]);
    		int weight  = Integer.parseInt(tempArr[3]);
    		
			graph[start][end] = weight;
			pathNumber[start][end] = edgeNum;
		}
    	
    	/*************************************************************/
    	String[] NecessStr= condition.split(",");
    	startNode = Integer.parseInt(NecessStr[0]);
    	endNode = Integer.parseInt(NecessStr[1]);
    	
    	String[] necessNode = NecessStr[2].substring(0,NecessStr[2].length()-1).split("\\|");
    	// 必经点数组
    	int[] necessityNode = new int[necessNode.length];     
    	// 存储必经点的数组
    	for (int i = 0; i < necessNode.length; i++)
		{
			necessityNode[i] = Integer.parseInt(necessNode[i]);
		}
    	// 必经点表
    	boolean[] necessTable = new boolean[MAX_VERTEX+1];
    	for (int i = 0; i < necessityNode.length; i ++)
    	{
    		necessTable[necessityNode[i]] = true;
    	}
    	/************************************************************/
    	String answer;
    	if (MAX_PATH < 100)
		{
    		answer = DFS(startNode, endNode, graph, necessityNode, pathNumber, filePath, necessTable);
		} else {
			answer = combinationArgo(startNode, endNode, necessityNode, graph, necessTable, filePath, pathNumber);
		}
    	System.out.println(answer);
        return answer;
    }
    
    /**
     * 深度优先算法
     * @param start  起始点
     * @param end    结束点
     * @param graphArr  图的邻接矩阵数组
     * @param NecessArr 图的必经节点数组
     * @param path 图的路径编号数组
     * @param filePath 文件路径
     * @param necessTable 必经点表
     */
    public static String DFS(int start, int end, int[][] graphArr, int[] NecessArr, int[][] path, String filePath, boolean[] necessTable)
    {
    	// 图的顶点数
    	int graphNum = graphArr.length;
    	// 定义一个栈，存储分支节点
    	Stack<Integer> nodeStack = new Stack<Integer>();
    	// 定义一个数组，存储访问过的点
    	boolean[] isVisited = new boolean[graphNum];
    	// 对于一个顶点，是否走过其分支的标志位
    	boolean[][] map = new boolean[graphNum][graphNum];
    	// 返回的字符串
    	StringBuilder answer = new StringBuilder();
    	// 记录路径权重
    	int pathWeight = Integer.MAX_VALUE;
    	
    	// 首先，将开始顶点压栈
    	nodeStack.push(start);
    	// 起始点标记为已访问
    	isVisited[start] = true;
    	// 记录必经点的数目
    	int count = 0;
    	
    	// 栈不为空，表示所有的路没有搜索完    	
    	while (!nodeStack.empty())
		{
    		int i = nodeStack.peek();
    		int j = 0;
    			
    		while (j < graphNum)
			{
				// 如果j是i的子节点，并且j没有被访问，并且该子节点没有被访问，并且没有环
				if (graphArr[i][j] < 30 && !isVisited[j] && !map[i][j])
				{
					// 设置标志位为:已访问
					isVisited[j] = true;
					map[i][j] = true;
					// 将j压入栈
					nodeStack.push(j);
					
					if (necessTable[j] == true)
					{
						count++;
					}
					break;
				}  else{
					j++;
				}				
			}
    		    		
    		// 如果没有路可走的时候，或者走到结束点的时候，要弹栈
    		if (j == graphNum || nodeStack.peek() == end)
			{
    			// 如果走到结束点的时候，要判断是否是满足要求的路径
    			if (nodeStack.peek() == end && count == NecessArr.length)
				{
    				StringBuilder sBuilder = new StringBuilder();
    				int tempWeight = 0;
    				
					for (int z = 1; z < nodeStack.size(); z++)
					{
						sBuilder.append(path[nodeStack.get(z-1)][nodeStack.get(z)]);
						tempWeight += graphArr[nodeStack.get(z-1)][nodeStack.get(z)];
						
						if (z != (nodeStack.size()-1))
						{
							sBuilder.append("|");
						}
					}
					
					// 判断得到的解是否是最优解
					if (tempWeight < pathWeight)
					{
						pathWeight = tempWeight;
						answer = sBuilder;
					}
					FileUtil.write(filePath, sBuilder.toString(), false);
				}
    			
    			// 弹栈，将标志位复位
				int popNode = nodeStack.pop();
				isVisited[popNode] = false;
				for (int k = 0; k < graphNum; k++)
				{
					map[popNode][k] = false;
				}
				if (necessTable[popNode] == true)
				{
					count--;
				}
			}
		}
    	
    	return answer.toString();
    }
    
    public static String switchVertextToEdge(int[][] path, List<Integer> pathList)
    {
    	StringBuilder sBuilder = new StringBuilder();
    	for (int z = 1; z < pathList.size(); z++)
		{
			sBuilder.append(path[pathList.get(z-1)][pathList.get(z)]);
			
			if (z != (pathList.size()-1))
			{
				sBuilder.append("|");
			}
		}
    	
    	return sBuilder.toString();
    }
    /**
     * 利用Dijkstra算法求出所有必经点之间的两两组合路径，然后深度优先
     * @param start 开始顶点
     * @param end 结束顶点
     * @param necessNode[] 必经点数组
     * @param graph[][] 图的邻接矩阵
     * @param necessTable 必经点表
     * @return
     */
    public static String combinationArgo(int start, int end, int[] necessNode, int[][] graph, boolean[] necessTable,String filePath, int[][] path)
    {
    	String result = "";
    	int pathWeight = Integer.MAX_VALUE;
    	/**********************************************************************************************/
    	/*                                 【迪杰斯特拉求两两之间最短路径】                                                                                                      */
    	/**********************************************************************************************/
    	// 起始点+必经点+结束点
    	LinkedList<Integer> twobyto[][] = new LinkedList[necessNode.length+2][necessNode.length+2];
    	// 由于不求结束点到其他必经点的路径，所以少建一行;不求其他点到开始点的路径，所以也要少建立一列
    	for (int i = 0; i < twobyto.length-1; i++)
    	{
    		for (int j = 1; j < twobyto.length; j++)
			{
    			twobyto[i][j] = new LinkedList<Integer>();
			}
    	}
    	// 必经点和数组下标对应数组
    	int[] num = new int[graph.length];
    	int count = 1;
    	num[start] = 0;
    	for (int i = 0; i < necessNode.length; i++)
		{
    		num[necessNode[i]] = count++;
		}
    	num[end] = count;
    	
        int dist[]=new int[graph.length];
        int prve[]=new int[graph.length];
    	
        // 利用迪杰斯特拉算法，求出两两必经点之间的路径
    	for (int i = 0; i < necessNode.length; i++)
    	{
        	dijkstra(necessNode[i], end, necessNode, graph, dist, prve, necessTable);
        	outPath(necessNode[i], end, prve, dist, twobyto, num, start, necessTable);
    	}
    	// 利用迪杰斯特拉算法，求出开始点到其他点的路径
    	dijkstra(start, end, necessNode, graph, dist, prve, necessTable);
    	outPath(start, end, prve, dist, twobyto, num, start, necessTable);
    	
    	/*********************************************************************************************/
    	/*                                     【深度优先搞起】                                                                                                                      */
    	/*********************************************************************************************/
    	Stack<Integer> stack = new Stack<Integer>();
    	// 被访问过节点
    	boolean[] isVisitedVertex = new boolean[graph.length];
    	// 必经点+start+end 
    	int ArrLenth = necessNode.length + 2;
    	// 定义一个数组，存储访问过的点
    	boolean[] isVisited = new boolean[ArrLenth];
    	// 对于一个顶点，是否走过其分支的标志位
    	boolean[][] map = new boolean[ArrLenth][ArrLenth];
    	// 路由顺序表
    	int[][] sortMap = new int[ArrLenth][ArrLenth];
    	// 是否被比较
    	boolean[] isCompared = new boolean[ArrLenth];
    	
    	// 计算路由顺序表的排序
    	for (int i = 0; i < sortMap.length-1; i++)
		{
    		int m_count = 0;
    		for (int k = 0; k < isCompared.length; k++)
			{
				isCompared[k] = false;
			}
			for (int j = 1; j < sortMap.length; j++)
			{
				int min = Integer.MAX_VALUE;
				int flag = 0;
				boolean flag2 = false;
				for (int k = 1; k < sortMap.length; k++)
				{
					// 是否已经被比较，size==0的时候表示此路不通，找到最小值
					if (!isCompared[k] && twobyto[i][k].size()!=0 && min > twobyto[i][k].size())
					{
						min = twobyto[i][k].size();
						flag = k;
						flag2 = true;
					}
				}
				// 找到最小值才给数组添加值
				if (flag2)
				{
					// 表示该值已经被比较
					isCompared[flag] = true;
					
					boolean isContainNecess = false;
					for (int k = 2; k < twobyto[i][flag].size()-1; k++)
					{
//						System.out.println("i="+i+",k="+k+",值："+twobyto[i][flag].get(k));
						if (necessTable[twobyto[i][flag].get(k)] || twobyto[i][flag].get(k) == start)
						{
							isContainNecess = true;
							break;
						}
					}
					
					if (isContainNecess == false)
					{
						// 填充路由顺序表
						sortMap[i][m_count++] = flag;
						
						// 如果要添加的值和前一个顶点的长度相等，那么判断他们的权重
						if (m_count >= 2 && twobyto[i][sortMap[i][m_count-1]].size() == 
								twobyto[i][sortMap[i][m_count-2]].size())
						{
							for (int k = m_count-1; k > 0; k--)
							{
								// 如果前一个的权重小，那么互换位置
								if (twobyto[i][sortMap[i][k]].size() == twobyto[i][sortMap[i][k-1]].size() && 
										(twobyto[i][sortMap[i][k]].peek() <	twobyto[i][sortMap[i][k-1]].peek()))
								{
									int temp = sortMap[i][k];
									sortMap[i][k] = sortMap[i][k-1];
									sortMap[i][k-1] = temp;
								} else {
									break;
								}
							}
						}
					}
				}
			}
		}
    	
//    	System.out.println("打印路径开始");
    	// 打印函数
//    	for (int j = 0; j < sortMap.length-1; j++)
//		{
//			for (int j2 = 0; j2 < sortMap.length; j2++)
//			{
//				System.out.print(sortMap[j][j2]+",");
//			}
//			System.out.println();
//			System.out.println("得到的");
//			for (int j2 = 0; j2 < sortMap.length; j2++)
//			{
//				if (twobyto[j][j2]!=null)
//				System.out.print(twobyto[j][j2].size()+",");
//			}
//			System.out.println();
//		}
//    	System.out.println("打印路径结束");
    	
    	// 将第一个点压栈
    	stack.push(0);
    	isVisited[0] = true;
    	
    	while (!stack.empty())
		{
    		int i = stack.peek();
    		int j = 0;
    		int next = Integer.MAX_VALUE;;
//    		System.out.println("i="+i);
    		
    		// next等于0的时候，就表示没有可以选择的通路了
    		while (j < ArrLenth && next != 0)
			{
    			// 获取优先要查找的节点
    			next = sortMap[i][j];
//    			System.out.println("j="+j+"--"+isVisited[next]+","+map[i][next]+","+isContainLoop(isVisitedVertex, twobyto[i][next]));
    			if (!isVisited[next] && !map[i][next] && !isContainLoop(isVisitedVertex, twobyto[i][next]))
				{
//    				System.out.println("压人next="+next+"进入了if");
					isVisited[next] = true;
					map[i][next] = true;
					stack.push(next);
					// 设置访问点
					setVisited(isVisitedVertex, twobyto[i][next], true);
    				break;
				} else {
					j++;
					next = sortMap[i][j];
				}
			}
    		
    		// 没有路可以走或者找到了结束点的时候
    		if ((next == 0) || j == ArrLenth || stack.peek() == (ArrLenth-1))
			{
//    			System.out.println(stack.size()+":大小");
    			// 如果搜索到终点
				if (stack.size() == ArrLenth)
				{
//					System.out.println("弹栈");
					int tempWeight = 0;
					StringBuilder tempBulider = new StringBuilder();
					List<Integer> pathList = new ArrayList<>();
					
					for (int z = 0; z < stack.size()-1; z++)
					{
						LinkedList<Integer> tempList = twobyto[stack.get(z)][stack.get(z+1)];
						int size = tempList.size();
						
						for (int k = size-1; k >= 2; k--)
						{
							tempBulider.append(tempList.get(k));
							tempBulider.append("|");
							pathList.add(tempList.get(k));
						}
						tempWeight += tempList.get(0);
					}
					
					tempBulider.append(end);
					pathList.add(end);
					
					// 判断得到的解是否是最优解
					if (tempWeight < pathWeight)
					{
						pathWeight = tempWeight;
						System.out.println(switchVertextToEdge(path, pathList));
						result = switchVertextToEdge(path, pathList);
						FileUtil.write(filePath, result, false);
					}
				}
    			
    			// 弹栈，将标志位复位
				int popNode = stack.pop();
				isVisited[popNode] = false;
				for (int k = 0; k < ArrLenth; k++)
				{
					map[popNode][k] = false;
				}
				// 已经设置为访问的点要撤回
//				System.out.println("弹出："+popNode);
				if (popNode != 0)
				{
//					System.out.println("i="+stack.peek()+",popName="+popNode);
					setVisited(isVisitedVertex, twobyto[stack.peek()][popNode], false);
				}
			}
		}
    	return result;
    }
    
    /**
     * 判断要加入的必经点线段是否包含有环路
     * @param isVisitedVertex 
     * @param linkedList 
     * @param next 表示行数
     * @return
     */
    public static boolean isContainLoop(boolean[] isVisitedVertex, LinkedList<Integer> linkedList)
    {
    	boolean answer = false;
    	// 不判断起点，故-1
    	for (int i = 1; i < linkedList.size()-1; i++)
		{
//    		System.out.print(linkedList.get(i)+",");
    		if (isVisitedVertex[linkedList.get(i)])
			{
				answer = true;
				break;
			}
		}
    	return answer;
    }
    
    /**
     * 顶点的入栈或者弹栈设置
     * @param isVisitedVertex
     * @param linkedList
     */
    public static void setVisited(boolean[] isVisitedVertex, LinkedList<Integer> linkedList, boolean flag)
    {
//    	for (int i = 0; i < isVisitedVertex.length; i++)
//		{
//			System.out.print(isVisitedVertex[i]+",");
//		}
//    	System.out.println("linkedlist:");
//    	for (int i = 0; i < linkedList.size(); i++)
//		{
//			System.out.print(linkedList.get(i)+",");
//		}
//    	System.out.println();
    	if (flag == true)
		{
    		for (int i = 1; i < linkedList.size(); i++)
    		{
        		isVisitedVertex[linkedList.get(i)] = flag;
    		}
		} else {
			for (int i = 1; i < linkedList.size()-1; i++)
    		{
        		isVisitedVertex[linkedList.get(i)] = flag;
    		}
		}
//    	System.out.println("设置后");
//    	for (int i = 0; i < isVisitedVertex.length; i++)
//		{
//			System.out.print(isVisitedVertex[i]+",");
//		}
//    	System.out.println();
    }
    
    /**
     * Dijkstra算法
     * @param start 开始顶点
     * @param end 结束顶点
     * @param necessNode 所有必经点数组
     * @param graph 图的邻接矩阵数组
     * @param dist 存储最短路径长度的数组
     * @param prve 存储当前顶点的前驱顶点
     * @param necessTable 必经点表
     */
  	 public static void dijkstra(int start, int end, int[] necessNode, int[][] graph, int dist[], int prve[], boolean[] necessTable)
  	 {
  		 // 图有多少个顶点
  		 int n = graph.length;
  		 // s[]存储已经找到最短路径的顶点，false为未求得
  		 boolean[] s = new boolean[n];
  		 // 计数：统计有几个必经点被加入到了s集合中了
  		 int count = 0;
  		   
  		 for(int i = 0; i < n; i++)
  		 {
  		   // 初始化dist[]数组
  		   dist[i] = graph[start][i];
  		   s[i] = false; 
  		   
  		   /*
  		    * prve[]数组存储源点到顶点vi之间的最短路径上该顶点的前驱顶点,
  		    * 若从源点到顶点vi之间无法到达，则前驱顶点为-1
  		    */
  		   if(dist[i] < Integer.MAX_VALUE)   
  		    prve[i] = start;
  		   else 
  		    prve[i] = -1;
  		  }
  		  
  		  // 初始化v0源点属于s集
  		  dist[start] = 0;
  		  // 表示v0源点已经求得最短路径
  		  s[start] = true;   
  		  
  		  for(int i = 0;i < n;i++)
  		  {
  			  // temp暂存v0源点到vi顶点的最短路径
  			  int temp = Integer.MAX_VALUE;   
  			  int u = start;
  			  for(int j = 0; j < n; j++)
  			  {
  				  // 顶点vi不属于s集当前顶点不属于s集(未求得最短路径),并且距离start更近
  				  if((!s[j]) && dist[j] < temp) 
  				  {                 
  				     u = j;                   // 更新当前源点,当前vi作为下一个路径的源点
  				     temp = dist[j];          // 更新当前最短路径
  				  }
  			  }
  			  s[u] = true;                    // 顶点vi进s集
  			  // 如果u为必经点，则cout+1
  			  if (necessTable[u])
  			  {
				count++;
  			  }
  			  // 更新当前最短路径以及路径长度
  			  for(int j = 0; j < n;j++)                          
  			  {     
  				  	// 当前顶点不属于s集(未求得最短路径)并且当前顶点有前驱顶点
  				    if((!s[j]) && graph[u][j]<Integer.MAX_VALUE)   
  				    {   
  					     int newDist=dist[u]+graph[u][j];        // 累加更新最短路径
  					     if (newDist<dist[j])
  					     {
  						      dist[j]=newDist;               // 更新后的最短路径
  						      prve[j]=u;                     // 当前顶点加入前驱顶点集
  					     }
  				    }
  			   }
  			  
  			  if (s[end] && (count == necessNode.length))
  			  {
				break;
  			  }
  		  }
  	 }
  	  
  	 /**
  	  * 结果输出方法
  	  * @param start 开始顶点
  	  * @param end 结束点
  	  * @param p 更新结果后的前驱顶点集
  	  * @param d 更新结果后的最短路径集
  	  * @param twoBytwo 存储两两必经点之间的路径和权重，权重存储在第一位
  	  * @param num 必经点和下标的对应关系
  	  * @param startNode 总路径的起始点（题目要求的起始点）
  	  * @param necessTable 必经点表
  	  */
  	 public static void outPath(int start, int end, int[] p, int[] d, LinkedList<Integer>[][] twoByto, int[] num, int startNode, boolean[] necessTable)
  	 {
  		  for(int i = 0; i < d.length; i++)
  		  {
  			   // 首先判断，是否是必经点,或者结束点，其次判断当前顶点已求得最短路径并且当前顶点不等于源点，最后判断当前顶点不等于总路径的起始点
  			   if((necessTable[i] || i==end )&& d[i] < Integer.MAX_VALUE && i!=start && i!=startNode)  
  			   {
//  					System.out.print("v"+i+"<--");
  					twoByto[num[start]][num[i]].add(i);
  					// 设置当前顶点的前驱顶点
  					int next = p[i];               
  					
  					// 若前驱顶点不为一个，循环求得剩余前驱顶点
  					while(next != start)              
  					{ 
//  						System.out.print("v"+next+"<--");
  						twoByto[num[start]][num[i]].add(next);
  						next=p[next];
  					}
  					twoByto[num[start]][num[i]].add(start);
  					
  					// 将权重保存在第一位
  					twoByto[num[start]][num[i]].addFirst(d[i]);
//  					System.out.println("v"+start+":"+d[i]);
  			   }
  		   }
  	 }
}
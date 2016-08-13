#include"stdio.h"
#include"readin.h"
#include"iostream"
#include"string"
#include"fstream"
#include<sstream>
using namespace std;
int d_num = 0;//要求必过点数量
int totalline = 0;
int start;
int dest;
int Data[4800][4];
int d_Data[50];
int Get_D_Data(const char*filename)
{
	/*******************demand数据集要求******************/
	char*Inf;
	string value;
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		cout << "This file can't open !";
		return 0;
	}
	char line[5000];
	fgets(line,5000, fp);//读入数据流
	Inf = strtok(line, ",");
	if (Inf != NULL)
		start = atoi(Inf);
	Inf = strtok(NULL, ",");
	if (Inf != NULL)
		dest = atoi(Inf);
	Inf = strtok(NULL, "|");
	while (Inf != NULL)
	{
		d_Data[d_num] = atoi(Inf);
		Inf = strtok(NULL, "|");
		d_num++;
	}
	fclose(fp);
	return 0;
}
/*************数据读取**********/
int Get_Data(const char*filename)
{
	ifstream file;
	file.open(filename);
	string  file_line, number; //读取的一行文本
	if (file.fail() || file.eof())//判断输入文件是否合格
	{
		cout << "can't open this file" << endl;
		return 0;
	}
	int v;
	while (std::getline(file, file_line))
	{
		int num = 0;
		istringstream is(file_line);
		while (std::getline(is, number, ','))
		{

			v = atoi(number.c_str());
			Data[totalline][num] = v;
			num++;
		}
		totalline++;
	}
	file.clear();
	file.close();
	return totalline;
}

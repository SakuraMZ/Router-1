#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
using namespace std;
const int DefaultNumOfPort=8;
#define DefaultVertices 8;
const int MaxVertices = 100;	//ͼ�е���󶥵���Ŀ
const int maxWeight = 10000;	//������Ȩֵ
const string invalid = "0.0.0.0";//��Ч�����
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;
	E cost;
	Edge<T, E> *link;
	Edge(int i_dest, string s_netNum, E i_cost)
	{
		dest = i_dest;
		netNum = s_netNum;
		cost = i_cost;
	}
	Edge() { cost = maxWeight; }
	bool operator != (Edge<T, E>& R)const
	{
		return (dest != R.dest) ? true : false;
	}
};
struct port {
	int num;		//�˿ں�
	string netNum;  //�˿����������
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //·�ɱ��
	T nameRouter;				 //·��������
	port *por;					//�ӿ�����
	int numofports;				//��ǰ�ӿ���
	Edge<T, E> *adj;		//�������ͷָ��
	Vertex(){ 
	por = new port[DefaultNumOfPort];
	numofports = 0;
	}
};
template<class T, class E>
class Graphlnk
{
public:
	Graphlnk(int sz = DefaultVertices);
	~Graphlnk();
	T getValue(int i)					//ȡλ��Ϊi�Ķ����е�ֵ
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].data : 0;
	}
	E getWeight(int v1, int v2);			//���ر�(v1,v2)��Ȩֵ
	bool insertVertex(int num, const T vertex);
	bool removeVertex(int v);
	bool insertEdge(string netNu, int v1, int v2, E cost);
	bool removeEdge(int v1, int v2);
	int NumberOfVertices()
	{
		return this->numVertices;
	}
	
	int getPort(int v1,int v2) {					//�õ������
		if (v1 != -1 && v2 != -1)
		{
			Edge<T, E> *p = NodeTable[v1].adj;
			while (p != NULL && p->dest != v2)
			{
				p = p->link;
			}
			if (p != NULL)
				return p->netNum;
			for (int i = 0; i < 20; i++)
			{
				if (NodeTable[v1].por[i].netNum == p->netNum)
					return i;
			}
			else return -1;
		}
	}
	int getVertexPos(const T vertex)
	{
		for (int i = 0; i< numVertices; i++)
			if (NodeTable[i].data == vertex)
				return i;
		return -1;
	}
	void ShortestPath(int v);
	void printPath(int v, E dist[], int path[]);
private:
	Vertex<T, E> * NodeTable;
	int maxVertices;
	int numEdges;
	int numVertices;
};
template<class T, class E>
Graphlnk<T, E>::Graphlnk(int sz /* = DeafultVertices */)
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = NULL;

	try
	{
		NodeTable = new Vertex<T, E>[maxVertices];
	}
	catch (bad_alloc & memExp)
	{
		cerr << memExp.what() << endl;
	}
	for (int i = 0; i<maxVertices; i++)
	{
		NodeTable[i].adj = NULL;
	}
}
template<class T, class E>
Graphlnk<T, E>::~Graphlnk()
{
	for (int i = 0; i<maxVertices; i++)
	{
		Edge<T, E> * p = NodeTable[i].adj;
		while (p != NULL)
		{
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}
	delete[] NodeTable;
}
template<class T, class E>
void Graphlnk<T, E>::ShortestPath(int v)
{//Graph��һ����Ȩ����ͼ�����㷨����һ�����飬dist[j],0<=j<n;�ǵ�ǰ�󵽵ĴӶ���v������j�����·�����ȣ�ͬʱ������path����󵽵����·��
	E *dis = new E[n];
	int *path = new int[n];
	int n = numVertices;
	bool *S = new bool[n];
	int i, j, k;
	E w, min;
	for (i = 0; i<n; i++)
	{
		dist[i] = getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] <maxValue)
			path[i] = v;
		else
		{
			path[i] = -1;
		}
	}
	S[v] = true;
	dist[v] = 0;
	for (i = 0; i<n - 1; i++)
	{
		min = maxValue;
		int u = v;
		for (j = 0; j<n; j++)
		{
			if (S[j] == false && dist[j] < min)
			{
				u = j;
				min = dist[j];
			}
		}
		S[u] = true;
		for (k = 0; k<n; k++)
		{
			w = G.getWeight(u, k);
			if (S[k] == false && w <maxValue && dist[u] + w<dist[k])
			{
				dist[k] = dist[u] + w;
				path[k] = u;
			}
		}
	}
	printPath(v, dist, path);
}

template<class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2)
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL)
			return p->cost;
		else return maxWeight;
	}
}
template<class T, class E>
bool Graphlnk<T, E>::insertVertex(int num, const T vertex)
{
	if (numVertices == maxVertices) return false;
	NodeTable[numVertices].nameRouter = vertex;
	numVertices++;
	return true;
}
template<class T, class E>
bool Graphlnk<T, E>::removeVertex(int v)
{
	if (numVertices == 1 || v<0 || v >= numVertices)return false;
	Edge<T, E> *p, *s, *t;
	int i, k;
	while (NodeTable[v].adj != NULL)
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;
		t = NULL;
		while (s != NULL && s->dest != v)
		{
			t = s;
			s = s->link;
		}
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;
			else t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;
		delete p;
		numEdges--;
	}
	numVertices--;
	NodeTable[v].data = NodeTable[numVertices].data;
	p = NodeTable[v].adj = NodeTable[numVertices].adj;
	while (p != NULL)
	{
		s = NodeTable[p->dest].adj;
		while (s != NULL)
			if (s->dest == numVertices) { s->dest = v; break; }
			else s = s->link;
	}
	return true;
}
template<class T, class E>
void Graphlnk<T, E>::printPath(int v, E dist[], int path[])
{//���path�����д洢��ͼG�Ӷ���v������������·���;���
	cout << "�Ӷ���" << getValue(v) << "����������������·��Ϊ��" << endl;
	int i, j, k, n = numVertices;
	int m = 0;
	string netN;
	int * d = new int[n];
	for (i = 0; i<n; i++)
	{
		if (i != v)
		{
			j = i;
			k = 0;
			while (j != v)
			{
				d[k++] = j;
				j = path[j];
			}
			cout << "·����" << getValue(i);
			if (k == 1)
			{
				m= getNetNum(v, d[0]);				//�õ��Ľӿں�
				cout << getValue(d[0]) << " ";			//d[0]����k�����һ��
				cout << "ֱ�ӽ���" << endl;
				cout <<NodeTable[0].p[m].num<< endl;
			}
			else if (k > 1)
			{
				
				cout << "��һ��Ϊ��" << endl;
				cout << getValue(d[--k]) << " ";

			}
			cout << "���·������Ϊ��" << dist[i] << endl;
		}
	}
	delete[] d;
}

template<class T, class E>
bool Graphlnk<T, E>::insertEdge(string netNu, int v1, int v2, E cost)
{
	int n;
	if (v1 >= 0 && v1< numVertices && v2 >= 0 && v2<numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL) return false;
		p = new Edge<T, E>;
		q = new Edge<T, E>;
		p->dest = v2;
		p->cost = cost;
		p->link = NodeTable[v1].adj;
		p->netNum = netNu;								//������Ÿ����
		n=NodeTable[v1].numofports;
		NodeTable[v1].por[n].num = n;						//���ν���ӿ�
		NodeTable[v1].por[n].netNum = netNu;				//������Ŵ��뵽�ӿڵ������
		NodeTable[v1].numofports++;
		NodeTable[v1].adj = p;
		q->dest = v1;
		q->cost = cost;
		q->netNum = netNu;
		n = NodeTable[v2].numofports;
		NodeTable[v2].por[n].num = n;						//���ν���ӿ�
		NodeTable[v2].por[n].netNum = netNu;				//������Ŵ��뵽�ӿڵ������
		NodeTable[v2].numofports++;						//��ǰ�ӿ���Ŀ��һ
		q->link = NodeTable[v2].adj;
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}
template<class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj, *q = NULL, *s = p;
		while (p != NULL && p->dest != v2)
		{
			q = p;
			p = p->link;
		}
		if (p != NULL)
		{
			if (p == s)
				NodeTable[v1].adj = p->link;
			else
			{
				q->link = p->link;
			}
			delete p;
		}
		else
		{
			return false;
		}
		p = NodeTable[v2].adj;
		q = NULL;
		s = p;
		while (p->dest != v1)
		{
			q = NULL;
			s = p;
		}
		if (p == s)
			NodeTable[v2].adj = p->link;
		else
		{
			q->link = p->link;
		}
		delete p;
		return true;
	}
	return false;
}
#endif
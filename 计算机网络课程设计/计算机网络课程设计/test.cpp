#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;
#include "Graphlnk.h"
int main()
{
	Graphlnk<string, int> g;
	g.NumberOfVertices();
	g.insertVertex(1,"ss");
	g.insertVertex(1, "ss");
	g.insertVertex(1, "ss");
	g.insertEdge("ss", 1, 2, 3);
	return 0;
}
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <list>
using namespace std;
#define MAX 100001

ifstream in("bfs.in");
ofstream out("bfs.out");


class Graph {
private:
    int nrV;    //number of vertiges
    int nrE;    //number of edges
    bool oriented; // True if the graph is oriented
    vector <int> edges[MAX]; //adjacency list
public:
    Graph() { cout<<"\nconstructor\n";}
    Graph(int x, int y, bool z) {nrV=x; nrE=y; oriented=z;}
    void read_o_graph();
    void BFS( int s ); // s = start node
};


 void Graph :: read_o_graph () {
    for(int i = 1; i <= nrE; i++)
    {
        int x, y;
        in >> x >> y;
        edges[x].push_back(y);

    }
}

void Graph :: BFS (int s) {
    vector <bool> visited (nrV+1, false);
    vector <int> distance (nrV+1, -1);
    queue <int> queue;  // coada pt bfs

    visited[s] = true;  // marchez nodul de start ca vizitat si il pun in coada
    distance[s] = 0;
    queue.push(s);

    while(!queue.empty())
    {
        int cv = queue.front();  //cv = current vertige
        queue.pop();
        // iau toate nodurile adiacente cu cv si, daca nu au fost vizitate, le marghez ca vizitate, le adaug in coada si calculez distanta de la s
        for( int i : edges[cv] )
            if(!visited[i])
            {
                visited[i] = true;
                
                queue.push(i);
                distance[i] = distance[cv] + 1;
            }
    }

    // pt fiecare nod afisez distanta de la s pana la el (ramane -1 daca nu a fost vizitat)
    for(int i = 1; i <= nrV; i++)
        out << distance[i] << " ";

}




int main()
{

     int n; int m; int s;
     in>>n>>m>>s;

     Graph g2(n,m,s);
     g2.read_o_graph();
     g2.BFS(s);

     in.close();
     out.close();

return 0;
}

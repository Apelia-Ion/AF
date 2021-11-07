#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <list>
using namespace std;

#define MAX 100001
bool visited_DFS[MAX];

ifstream in("dfs.in");
ofstream out("dfs.out");


class Graph {
private:

    int nrV;    //number of vertiges
    int nrE;    //number of edges
    bool oriented; // True if the graph is oriented
    vector <int> edges[MAX]; //adjacency list
public:

    Graph(int x, int y, bool z) {nrV=x; nrE=y; oriented=z;}
    void read_graph();  // read and make the actual graph
    void BFS( int s ); // s = start node
    void DFS( int s );
    void DFS_conex_comp();
};


 void Graph :: read_graph () {
    if (oriented==true)
    {
        for(int i = 1; i <= nrE; i++)
        {
            int x, y;
            in >> x >> y;
            edges[x].push_back(y);

        }
    }
    else
    {
            for(int i = 1; i <= nrE; i++)
        {
            int x, y;
            in >> x >> y;
            edges[x].push_back(y);
            edges[y].push_back(x);

        }
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

void Graph :: DFS(int s){
    // functie apelata de DFS_conex_comp
    visited_DFS[s] = true;
    for( int i : edges[s] )
            if(!visited_DFS[i])
                DFS(i);
    
}

void Graph :: DFS_conex_comp(){
    int nr = 0;
    for(int i=1; i<= nrV; i++)
        if(!visited_DFS[i])
        {
            DFS(i);
            nr++;
        }
    out << nr;
}


int main()
{

    int n; int m;
    in>>n>>m;

    Graph g2(n,m,0);
    g2.read_graph();

    //g2.BFS(s);
    g2.DFS_conex_comp();


    in.close();
    out.close();

return 0;
}

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <list>
#include <stack>
using namespace std;

#define MAX 100001

// for DFS
bool visited_DFS[MAX];

//for SCC (CTC)
// stack <int> s;
// vector <int> edges_transp[MAX], comp[MAX];
// bool visited_DFS_transp_graph[MAX];

// files
ifstream in("ctc.in");
ofstream out("ctc.out");


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
    void DFS( int s ); // used for DFS_conex_comp
    void DFS_conex_comp();

    void get_transposed_graph(vector<int> edges_transp[]); //transposed graph used for SCC
    void DFS_SCC(int v, vector <int> edges_transp[], int nr, vector<int> comp); 
    void order_SCC(int v, bool visited_SCC[], stack<int> s);
    void SCC(); // Strongly Connected Components (CTC)
    


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

void Graph :: get_transposed_graph(vector<int> edges_transp[]){
    for(int i = 1 ; i <= nrV ; i++)
        for(int j : edges[i])
            edges_transp[j].push_back(i);
    
}

void Graph :: order_SCC(int v, bool visited_SCC[], stack<int> s){
     visited_SCC[v] = true;
      for(int i : edges[v])
        if(!visited_SCC[i])
            order_SCC(i, visited_SCC, s);
    s.push(v);
}

void Graph :: DFS_SCC(int v, vector <int> edges_transp[], int nr, vector<int> comp){
    visited_DFS[v] = true;
    comp[nr].push_back(v);
    for(int i : edges_transp[v])
        if(!visited_DFS[i])
            DFS_SCC(i, edges_transp, nr, comp);

}

void Graph :: SCC(){
    //  Kosaraju's Algorithm -- O(V+E)
    int nr = 0;  //nr de componente tare conexe
    stack <int> s;
    vector <int> edges_transp[MAX], comp[MAX];
    bool visited_SCC [MAX] = {false}; 

    for(int i = 0; i < nrV; i++)
        if(visited_SCC[i] == false)
            order_SCC(i, visited_SCC, s);
    
    get_transposed_graph(edges_transp);

    for(int i = 0; i < nrV; i++)
        visited_SCC[i] = false;

    while (!s.empty())
    {
        int v = s.top();
        s.pop();
        if(!visited_SCC[v])
        {
            nr = nr+1;
            DFS_SCC(v, edges_transp, nr, comp);
            
        }
    }





}



int main()
{

    int n; int m;
    in>>n>>m;

    Graph g2(n,m,0);
    g2.read_graph();

    //g2.BFS(s);
    //g2.DFS_conex_comp();


    in.close();
    out.close();

return 0;
}

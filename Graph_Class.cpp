#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <list>
#include <stack>
#include <tuple>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

#define MAX 100001

// for DFS
bool visited_DFS[MAX];

//for SCC (CTC)
// stack <int> s;
// vector <int> edges_transp[MAX], comp[MAX];
// bool visited_DFS_transp_graph[MAX];

// files
ifstream in("royfloyd.in");
ofstream out("royfloyd.out");


class Graph {
private:

    int nrV;    //number of vertiges
    int nrE;    //number of edges
    bool oriented; // True if the graph is oriented
    vector <int> edges[MAX]; //adjacency list
    vector < vector < pair <int, int> > > weighted_edges; //adjacency list for weighted graph 
    vector < vector <int> > weighted_marix;
    
public:
    
    Graph(int x, int y, bool z) {nrV=x; nrE=y; oriented=z;}
    Graph(int x, bool z){nrV=x; oriented=z;} // constructor 2 pentru cerintele la care nu avem nrE dat

    ///Tema 1
    void read_graph();  // read and make the actual graph

    void BFS( int s ); // s = start node

    void DFS( int s ); // used for DFS_conex_comp
    void DFS_conex_comp();



    ///Tema 2
    void read_weighted_graph(); 

    int repr(int v, vector <int> &root);  //returneaza reprezentantul nodului v
    void link(int v1, int v2, vector <int> &height, vector <int> &root); //leaga arborele mai mic de cel mai mare
    void APM();

    void disj(); //Paduri de multimi disjuncte

    void Bellman_Ford(int s); //s-start node

    ///Tema 3

    vector<vector<int>> Roy_Floyd();
    void afisare_Roy_Floyd(); //afiseaza matricea drumurilor minime
    void read_weighted_matrix(); //matricea ponderilor din cerinta
    

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

void Graph :: read_weighted_graph(){

    weighted_edges.resize(nrV+1);

    if (oriented==true)
    {
        for(int i = 1; i <= nrE; i++)
        {
            int x, y, c;
            in >> x >> y >> c;
            weighted_edges[x].push_back(make_pair(y,c));
        }
    }
    else
    {
            for(int i = 1; i <= nrE; i++)
        {
            int x, y, c;
            in >> x >> y >> c;
            weighted_edges[x].push_back(make_pair(y,c));
            weighted_edges[y].push_back(make_pair(x,c));

        }
    }


}

void Graph :: read_weighted_matrix(){
    int edges=0;
    int elem;
    weighted_marix.resize(nrV);

    for(int i=0; i<nrV; i++)
        for (int j=0; j<nrV; j++)
            {
                in >> elem;
                weighted_marix[i].push_back(elem);
                if(weighted_marix[i][j])
                    edges++;
            }
    nrE=edges;

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
    int nr = 0;   // nr comp conexe
    for(int i=1; i<= nrV; i++)
        if(!visited_DFS[i])
        {
            DFS(i);
            nr++;
        }
    out << nr;
}

int Graph :: repr(int v, vector <int> &root){
    while (root[v] != v)
        v=root[v];
    return v;
}

void Graph :: link(int v1, int v2, vector <int> &height, vector <int> &root){
    int rv1=repr(v1, root);
    int rv2=repr(v2, root);
    
    if(height[rv1]>height[rv2])
        root[rv2]=rv1;
    else
        {
            root[rv1]=rv2;
            if(height[rv1]==height[rv2])
                height[rv2]++;
        }
}

void Graph :: APM (){
    //Kruskal
    int x, y, c;
    vector < tuple <int,int,int> > edges_weights_list; //vector (E1,E2,W)
    int cost_apm = 0;
    int nr = 0;  //numarul de muchii din APM la un moment dat (pt stop condition)
    vector <int> height(nrV+1, 1);  //inaltimea arborelui 
    vector <int> root(nrV+1);  //radacina arborelui
    vector <bool> edges(nrE+1,0);  //folosit pt a scrie la final muchiile din apm
    
    // formez vectorul de muchii si costuri
    for(int i=0; i<nrE; ++i)
    {
        in>>x>>y>>c;
        edges_weights_list.push_back(make_tuple(x,y,c));
    }

    for(int i=1; i<=nrV; ++i)
        root[i]=i;

    // sortare muchii dupa cost
    sort (edges_weights_list.begin(), edges_weights_list.end(),
            [](const tuple<int, int, int> &c1, const tuple<int, int, int> &c2) { return get<2>(c1) < get<2>(c2); });

    //Iau pe rand muchiile din vector si verific daca formeaza un ciclu cu apm-ul format
    //pana in momentul i; Daca nu formeaza un ciclu includ muchia in APM. 
    // Repet pasul anterior pana am nrV-1 muchii in apm

    for(int i=0; i<nrE && nr<nrV-1; i++)
    {
        int v1=get<0>(edges_weights_list[i]);
        int v2=get<1>(edges_weights_list[i]);
        int cost=get<2>(edges_weights_list[i]);

        int rv1 = repr(v1, root);
        int rv2 = repr(v2, root);

        if(rv1 != rv2)  //daca nu fac parte din aceeasi comp conexa
        {
            nr++;
            link(v1,v2,height,root);
            cost_apm=cost_apm+cost;
            edges[i]=1;     
        }


    }

    out<< cost_apm<<'\n'<<nr <<'\n';
    for (int i=0; i<nrE; i++)
        if(edges[i])
            out<<get<0>(edges_weights_list[i])<<' '<< get<1>(edges_weights_list[i])<<'\n';



}

void Graph :: disj(){
    //rezolvare cu arbori

    //OBS!!! n si m, nr de multimi si nr de operatii au fost citite in main 
    // si folosite de constructor pt a initializa graful, deci sunt nrV si nrE
    int cod, x, y;
    vector <int> height(nrV+1, 1), root(nrV+1);

    //in>>n>>m;
    //cout<<nrV<<' '<<nrE;

    for(int i=0; i<nrV; i++)
        root[i]=i;


    for(int i=0; i<nrE; i++)
    {
        in>>cod>>x>>y;
        if(cod==1)
            link(x,y,height,root); //refolosesc functia de la apm
        else
        {
            int reprx=repr(x,root); //reprezentantul lui x (vezi functia de la APM)
            int repry=repr(y,root);
            if (reprx == repry)
                out<<"DA\n";
            else out<<"NU\n";
        }

    }
    

}

void Graph :: Bellman_Ford(int s){

    const int inf = INT_MAX;  
    bool negative_cycle = false; 
    vector <int> distance (nrV + 1, inf); //distante
    vector <bool> in_queue (nrV + 1, false); // nodul x e sau nu in coada
    vector <int> passes (nrV + 1, 0); //retine de cate ori am trecut printr-un nod
    queue <int> queue; //nodurile ale caror distanta s-a modificat (pt optimizare)
    
    distance[s]=0;
    queue.push(s);
    in_queue[s] = true;

    while(queue.empty() == false && negative_cycle == false)
    {
        int v1=queue.front();
        queue.pop();
        in_queue[v1]=0;
        for(int i=0; i<weighted_edges[v1].size(); i++)
        {
            int v2 = get <0> (weighted_edges[v1][i]);
            int c = get <1> (weighted_edges[v1][i]);
            if(distance[v2]>distance[v1]+c) //relaxez muchia
            {
                distance[v2]=distance[v1]+c;
                passes[v2]++;
                if(!in_queue[v2])
                {
                    queue.push(v2);
                    in_queue[v2]=true;

                }

                if(passes[v2]>=nrV)
                negative_cycle=true;
            }


        }

    }

    if(negative_cycle == true)
        out<<"Ciclu negativ!";
    else
        for(int i=1; i<nrV+1; i++)
            if(i != s)
                out<<distance[i]<<' ';
    


}

 vector<vector<int>> Graph :: Roy_Floyd(){
    vector< vector <int> > result; //matricea de drumuri minime
    const int inf = INT_MAX;
    result.resize(nrV);

    for(int i=0;i<nrV;i++)
        for(int j=0; j<nrV; j++)
            {
                if(weighted_marix[i][j])
                    result[i].push_back(weighted_marix[i][j]);
                else
                    result[i].push_back(inf);
            }


    for(int k=0; k<nrV; k++)  //varfuri intermediare
        for(int i=0;i<nrV;i++)
            for(int j=0; j<nrV; j++)
                if(result[i][j]>result[i][k]+result[k][j])
                    result[i][j]=result[i][k]+result[k][j];
    return result;
}


void Graph :: afisare_Roy_Floyd(){

for (int i=0; i<nrV; i++)
{
    vector< vector <int> > result;
    const int inf = INT_MAX;
    result=Roy_Floyd();
    
    for(int j=0;j<nrV;j++)
        if (result[i][j]!=inf && i!=j)
            out<<result[i][j]<<' ';
        else 
            out<<0<<' ';
    out<<'\n';
}
}


int main()
{

    int n;
    int m;
    in>>n;
    in>>m;

    Graph g2(n,m,1);
    g2.read_graph();

    //g2.BFS(s);
    g2.DFS_conex_comp();

    //g2.APM();
    //g2.disj();

    //g2.read_weighted_graph();
    //g2.Bellman_Ford(1);

    //Graph g(n,1);
    //g.read_weighted_matrix();
   //g.afisare_Roy_Floyd();

    in.close();
    out.close();

return 0;
}

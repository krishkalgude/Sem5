#include<bits/stdc++.h>
#include<time.h>
#include<chrono>
#include<pthread.h>
#include<algorithm>
#include<mutex>
using namespace std;
using namespace std::chrono;
#define endl "\n"
#define pb push_back
#define F first
#define S second
#define pi pair<int,int>
#define mp make_pair
typedef long long int ll;
typedef unsigned long long int ull;
typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<ll> vll;
typedef vector<ull> vull;


#define INF INT_MAX  
int n;
vector<vector<pair<int, int>>> adj;     
vector<vector<int>> shortestPaths;      
vector<vector<int>> optimizedPaths;     
vector<vector<int>> dist;               
vector<vector<int>> matrix;             
vector<vector<int>> mstWeightMatrix;
vector<vector<int>> threadedMatrix;

int readMatrixFromFile(const string& filename) {
    ifstream inputFile(filename);
    
    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 0;
    }

    vector<vector<int>> tempMatrix;
    string line;
    
    while (getline(inputFile, line)) {
        stringstream ss(line);
        vector<int> row;
        int value;
        while (ss >> value) {
            row.push_back(value);
        }
        tempMatrix.push_back(row);
    }

    inputFile.close();

    int rows = tempMatrix.size();
    int cols = rows > 0 ? tempMatrix[0].size() : 0;

    matrix.resize(rows, vector<int>(cols, -1));  
    adj.resize(rows);                            
    shortestPaths.resize(rows, vector<int>(cols, INF));
    optimizedPaths.resize(rows + 1, vector<int>(cols + 1, INF));
    dist.resize(rows + 1, vector<int>(cols + 1, INF));
    mstWeightMatrix.resize(rows, vector<int>(cols, 0)); 

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = tempMatrix[i][j];
           
        }
    }

    return rows;
}

struct Edge {
    int u, v, weight;
    bool operator<(Edge const& other) const {
        return weight < other.weight; 
    }
};

struct TreeNode {
    int val;
    int Ltag, Rtag;  
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;  
    TreeNode(int x) : val(x), Ltag(0), Rtag(0), left(nullptr), right(nullptr), parent(nullptr) {}
};
TreeNode *Root,*Dummy,*leftmost,*rightmost;
class DisjointSet {
public:
    vector<int> parent, rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }
    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u != v) {
            if (rank[u] < rank[v])
                swap(u, v);
            parent[v] = u;
            if (rank[u] == rank[v])
                rank[u]++;
        }
    }
};
vector<Edge> MST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());  
    DisjointSet ds(n);
    vector<Edge> mst;

    for (Edge& e : edges) {
        if (ds.find(e.u) != ds.find(e.v)) {
            mst.push_back(e); 
            ds.unite(e.u, e.v);

            mstWeightMatrix[e.u][e.v] = e.weight;
            mstWeightMatrix[e.v][e.u] = e.weight;  
        }
    }

    return mst;
}

vector<int> countDegrees(const vector<Edge>& mst, int n) {
    vector<int> degree(n, 0);
    for (const Edge& e : mst) {
        degree[e.u]++;
        degree[e.v]++;
    }
    return degree;
}

TreeNode* buildTree(int node, const vector<Edge>& mst, vector<bool>& visited, TreeNode* parent = nullptr) {
    TreeNode* root = new TreeNode(node);
    visited[node] = true;
    root->parent = parent; 

    for (const Edge& e : mst) {
        if (e.u == node && !visited[e.v]) {
            if (!root->left) {
                root->left = buildTree(e.v, mst, visited, root);
                root->Ltag = 1;  
            } else {
                root->right = buildTree(e.v, mst, visited, root);
                root->Rtag = 1;  
            }
        } else if (e.v == node && !visited[e.u]) {
            if (!root->left) {
                root->left = buildTree(e.u, mst, visited, root);
                root->Ltag = 1; 
            } else {
                root->right = buildTree(e.u, mst, visited, root);
                root->Rtag = 1; 
            }
        }
    }

    return root;
}

TreeNode* convertToThreaded(TreeNode* Root)
{
    TreeNode* dummy=new TreeNode(INT_MAX);
    
    TreeNode *temp=Root,*parent=NULL,*ntemp,*root=Root;
    
    while(temp->Ltag==1)
        temp=temp->left;
    
    temp->left=dummy;
    temp=Root;
    
    while(temp->Rtag==1)
        temp=temp->right;
    
    temp->right=dummy;
    dummy->Ltag=1;
    dummy->Rtag=1;
    dummy->left=Root;
    dummy->right=dummy;
    return dummy;
 }
 void SetupSuccessor(TreeNode* temp)
{
    TreeNode *trav,*store=temp;
    if(temp->Ltag==1)
    {
    trav=temp->left;
    if(trav->Rtag==1)
    {
        while(trav->Rtag==1)
            trav=trav->right;
    }   
    trav->right=store;
    }
    if(store->Ltag==1)
            SetupSuccessor(store->left);
        
    if(store->Rtag==1)
            SetupSuccessor(store->right);
    
}
void SetupPredecessor(TreeNode* temp)
{
    TreeNode *trav,*store=temp;
    if(temp->Rtag==1)
    {
    trav=temp->right;
    if(trav->Ltag==1)
    {
        while(trav->Ltag==1)
            trav=trav->left;
    }   
    trav->left=store;
    }
    if(store->Ltag==1)
            SetupPredecessor(store->left);
    if(store->Rtag==1)
        SetupPredecessor(store->right);
        
}
TreeNode* FindSuccessor(TreeNode* temp)
{
    TreeNode *position,*Ntrav;
    if(temp->Rtag==0)
        return temp->right;
    else
    {
        position=temp->right;
        while(position->Ltag==1)
            position=position->left;
        return position;
    }
    
}
TreeNode* inorderSuccessorTraversal(TreeNode* temp,int Key,int mode)
{
    TreeNode* p=temp;
    while(1)
    {
        p=FindSuccessor(p);
        
        if(p->val!=INT_MAX  && mode==1){
        cout<<p->val<<" ";
        }
        if(p->val==Key)
           return p;
    }
        
}
TreeNode* FindPredecessor(TreeNode* temp)
{
    TreeNode *position,*Ntrav;
    if(temp->Ltag==1)
        return temp->left;
    else
    {
        position=temp;
        while(position->Rtag==0){
            position=position->right;
        }
        return position->right;
    }
    
}

TreeNode* inorderPredecessorTraversal(TreeNode* temp,int Key,int mode)
{
    TreeNode* p=temp;
    while(1)
    {
        p=FindPredecessor(p);
        if(p->val!=INT_MAX && mode==1){
       
        cout<<p->val<<" ";
        }
        if(p->val==Key)
            return p;    
    }
}
TreeNode* inSucc(TreeNode* temp)
{
    if(temp->Rtag==0)
        return temp->right;
    temp=temp->right;
    while(temp->Ltag==1)
        temp=temp->left;
    
    return temp;
}
TreeNode* inPred(TreeNode* temp)
{
    
    if(temp->Ltag==0)
        return temp->left;
    temp=temp->left;
    while(temp->Rtag==1)
        temp=temp->right;
    
    return temp;
}
vector<int> dijkstra(int src, int n, vector<vector<pair<int, int>>> &adj) {
    vector<int> dist(n, INF);  
    dist[src] = 0;  
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int currDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currDist > dist[u]) continue;

        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist; 
}

void linkStateRouting(int n) {
     auto start= high_resolution_clock::now();
  
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] != -1 && i != j) {
                adj[i].push_back({j, matrix[i][j]});  
            }
        }
    }
    
    for (int i = 0; i < n; ++i) {
        shortestPaths[i] = dijkstra(i, n, adj);
    }
    

    
    auto end = high_resolution_clock::now();
    auto exec_time = duration_cast<microseconds>(end - start);
    cout << "Execution time in Link state routing: " << exec_time.count() << " ms" << endl;
}

void threadopt(TreeNode *temp)
{
    if(temp->Ltag==0){
        if((temp->val)!=(leftmost->val)){
        optimizedPaths[temp->val+1][temp->left->val+1]=0;
    }
}
    else
        threadopt(temp->left);
    if(temp->Rtag==0){
        if((temp->val)!=(rightmost->val)){
        optimizedPaths[temp->val+1][temp->right->val+1]=0;
    }
}
     else
        threadopt(temp->right);

}

 vector<vector<int>> threadMatrix(int n )
{
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            if((i-1)==(j-1))
                optimizedPaths[i][j]=0;
            if (mstWeightMatrix[i-1][j-1] != 0)
                optimizedPaths[i][j]=mstWeightMatrix[i-1][j-1]; 
            
        }
    }
    optimizedPaths[0][0]=0;
    optimizedPaths[0][Dummy->left->val+1]=0;
    
    rightmost=inPred(Dummy);
    optimizedPaths[rightmost->val+1][0]=0;
    if(rightmost->Ltag==0)   
    optimizedPaths[rightmost->val+1][rightmost->left->val+1]=0;
    
    leftmost=inSucc(Dummy);
    optimizedPaths[leftmost->val+1][0]=0;
    if(leftmost->Rtag==0)
    optimizedPaths[leftmost->val+1][leftmost->right->val+1]=0;
    
 
    threadopt(Root);
     
       
    return optimizedPaths;
}

void lengthwiseTraversal(vector<vector<int>>& matrix, int n) {
    
    dist = matrix; 
    
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
               if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
   
}
void threadedAlgo (int n)
{
      auto start= high_resolution_clock::now();
   
    vector<Edge> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (matrix[i][j] != -1) {
                edges.push_back({i, j, matrix[i][j]});
            }
        }
    }
    vector<Edge> mst = MST(n, edges);

    // cout << "Edges in the Tree:" << endl;
    // for (Edge& e : mst) {
    //     cout << e.u << " -- " << e.v << " with weight " << e.weight << endl;
    // }

    Edge maxEdge = *max_element(mst.begin(), mst.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;
    });

    //cout << "\nEdge with maximum weight: " << maxEdge.u << " -- " << maxEdge.v << " with weight " << maxEdge.weight << endl;

    vector<int> degree = countDegrees(mst, n);
    
    int rootNode = -1;
    if (degree[maxEdge.u] <= 2) {
        rootNode = maxEdge.u;
    } else if (degree[maxEdge.v] <= 2) {
        rootNode = maxEdge.v;
    } else {
        
        Edge minEdge = *min_element(mst.begin(), mst.end(), [](Edge a, Edge b) {
            return a.weight < b.weight;
        });

        if (degree[minEdge.u] <= 2) {
            rootNode = minEdge.u;
        } 
        else if (degree[minEdge.v] <= 2) {
            rootNode = minEdge.v;
        } 
    }
    
    vector<bool> visited(n, false);
    Root = buildTree(rootNode, mst, visited);
    
    Dummy=convertToThreaded(Root);
    SetupSuccessor(Root);
    SetupPredecessor(Root);
    // cout<<"* Inorder using Successor : "<<endl;
    // inorderSuccessorTraversal(Dummy,INT_MAX,1);
    // cout<<endl;
    // cout<<endl<<"* Preorder using Predecessor : "<<endl;
    // inorderPredecessorTraversal(Dummy,INT_MAX,1);
    // cout<<endl<<endl;
   
    threadedMatrix=threadMatrix(n+1);

    
    //   for (int i = 0; i < n+1; ++i) {
    //     for (int j = 0; j < n+1; ++j) {
    //         if (threadedMatrix[i][j] == INF)
    //             cout << " INF\t";  
    //         else
    //            cout << threadedMatrix[i][j] << "\t";  
    //     }
    //     cout << endl;
    // }
    lengthwiseTraversal(threadedMatrix, n+1);
      auto end = high_resolution_clock::now();
    auto exec_time = duration_cast<microseconds>(end - start);
    cout << "Execution time in threaded algo: " << exec_time.count() << " ms" << endl;
}


int main() {
    int reduced=0,increased=0,unitless=0,unitmore=0;
    
    string filename = "inputf.in";  
    n=readMatrixFromFile(filename);  
     thread t1(linkStateRouting,n);
    t1.join();
    
    thread t2(threadedAlgo,n);
    t2.join();
    
    cout << "\nmstweightmatrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << (mstWeightMatrix[i][j] > 0 ? to_string(mstWeightMatrix[i][j]) : "0") << "\t";
        }
        cout << endl;
    }
    cout << "Shortest paths (Link State Routing)\n";
    
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (shortestPaths[i][j] == INF)
                cout << " INF\t";  
            else
                cout << shortestPaths[i][j] << "\t";  
                
        }
        cout << endl;
    }
     
    cout << "Threaded Algo Paths\n";

    for (int i = 1; i < n+1; ++i) {
        for (int j = 1; j < n+1; ++j) {
            if (dist[i][j] == INF)
                cout << "INF\t";
            else
                cout << dist[i][j] << "\t";
        }
        cout<<endl;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (shortestPaths[i][j]>dist[i+1][j+1]){
                reduced++;
                unitless+=(shortestPaths[i][j]-dist[i+1][j+1]);
            }
            else if(shortestPaths[i][j]<dist[i+1][j+1]){
                increased++;
                unitmore+=(dist[i+1][j+1]-shortestPaths[i][j]);
            }
        }
    }
    cout<<"Total entries :"<<n*n<<endl<<"Optimized :"<<reduced<<endl<<"Increased entries :"<<increased;
    cout<<endl<<"saved total = "<<unitless<<" units"<<endl<<"taken more time = "<<unitmore<<" units";
   

return 0;
}
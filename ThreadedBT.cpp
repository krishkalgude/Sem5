#include<bits/stdc++.h>
using namespace std;
#define endl "\n"
#define pb push_back
#define F first
#define S second
#define pi pair<int,int>
#define mp make_pair;
typedef long long int ll;
typedef unsigned long long int ull;
typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<ll> vll;
typedef vector<ull> vull;
const int mod=1e9+7;

typedef struct node {
    struct node* left;
    bool Ltag;
    int key;
    bool Rtag;
    struct node* right;
}TreeNode;

typedef struct check{
    struct check* left;
    int key;
    struct check* right;
    check(int x) : key(x), left(nullptr), right(nullptr) {}
    
}checkTree;
vector<pair<int,pi>> Matrix;
vector<pair<int,pi>> Insert;
vector<int> ToBeDeleted;
vi preorder;
vi inorder;
TreeNode *Root,*Dummy;

TreeNode* create(TreeNode* temp,int index)
{
    TreeNode* tempL,*tempR,*root;
    if(index==1){
    temp=new TreeNode;
    root=temp;
    }
    
    int Key=Matrix[index].F,indexL=Matrix[index].S.F,indexR=Matrix[index].S.S;
    
    temp->key=Key;
    if(indexL!=0)
    {
        tempL=new TreeNode;
        temp->left=tempL;
        temp->Ltag=true;
        
    }
    else
    {
        temp->left=NULL;
        temp->Ltag=false;
    }
    if(indexR!=0)
    {
        tempR=new TreeNode;
        temp->right=tempR;
        temp->Rtag=true;
        
    }
    else
    {
        temp->right=NULL;
        temp->Rtag=false;
    }
    if(indexL!=0)
    {
        create(tempL,indexL);
    }
    if(indexR!=0)
    {
    create(tempR,indexR);
    }
    return root;
}

TreeNode* convertToThreaded(TreeNode* Root)
{
    TreeNode* dummy=new TreeNode;
    dummy->key=INT_MAX;
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
        
        if(p->key!=INT_MAX  && mode==1){
        inorder.pb(p->key);
        cout<<p->key<<" ";
        }
        if(p->key==Key)
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
        if(p->key!=INT_MAX && mode==1){
        preorder.pb(p->key);
        cout<<p->key<<" ";
        }
        if(p->key==Key)
            return p;    
    }
}

void Insertion(TreeNode* root)
{
    
    for (int i = 0; i < Insert.size(); ++i)
    {
    int k=Insert[i].F,find=Insert[i].S.F,child=Insert[i].S.S;
    TreeNode* a=inorderPredecessorTraversal(Dummy,find,0);
    TreeNode* temp=new TreeNode;
    temp->key=k;
    temp->Ltag=0;
    temp->Rtag=0;
    if(child==0)
    {
        temp->left=a->left;
        temp->right=a;
        a->Ltag=1;
        a->left=temp;
    }
    else
    {
        temp->right=a->right;
        temp->left=a;
        a->Rtag=1;
        a->right=temp;
    }
    
     
    } 
}
checkTree* buildTreeHelper(vector<int>& preorder,int preStart,int preEnd,vector<int>& inorder,int inStart,int inEnd,unordered_map<int,int>& inMap) {
    if(preStart>preEnd||inStart>inEnd)
        return NULL;

    int rootVal=preorder[preStart];
    checkTree* root=new checkTree(rootVal);

    int inRoot=inMap[rootVal];
    int numsLeft=inRoot-inStart;

    root->left=buildTreeHelper(preorder,preStart+1,preStart+numsLeft,inorder,inStart,inRoot-1,inMap);
    root->right=buildTreeHelper(preorder,preStart+numsLeft+1,preEnd,inorder,inRoot+1,inEnd,inMap);

    return root;
}

checkTree* build(vector<int>& preorder,vector<int>& inorder){
    unordered_map<int,int> inMap;
    for (int i=0;i<inorder.size();i++)
        inMap[inorder[i]]=i;
    return buildTreeHelper(preorder,0,preorder.size()-1,inorder,0,inorder.size()-1,inMap);
}
void fillMatrix(checkTree* root,vector<vector<int>>& newmatrix){
    if (!root) 
        return;
    int leftKey=(root->left)?root->left->key:-1;
    int rightKey=(root->right)?root->right->key:-1;
    
    newmatrix.push_back({root->key,leftKey,rightKey});
    fillMatrix(root->left,newmatrix);
    fillMatrix(root->right,newmatrix);
}

void printMatrix(vector<vector<int>>& newmatrix){
    for (auto& row:newmatrix)
        cout<<row[0]<<"    "<<row[1]<<"    "<<row[2]<<endl;
}     
TreeNode* FindParentAndNode(TreeNode* temp, int KEY) {
    if(Root->key==KEY)
        return Dummy;
    if(!temp) 
        return nullptr;
    
    if(temp->Ltag == 1 ) 
    {
        if(temp->left->key == KEY) 
            return temp;
        else 
        {
            TreeNode* leftSearch = FindParentAndNode(temp->left, KEY);
            if (leftSearch) 
                return leftSearch;
        }
    }
    if (temp->Rtag == 1 )
    {
        if (temp->right->key == KEY)
            return temp;
        else 
        {
            TreeNode* rightSearch = FindParentAndNode(temp->right, KEY);
            if (rightSearch) 
                return rightSearch;
        }
    }

    return NULL;
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

void deletion(int x)
{
    TreeNode *parent,*required;

    parent=FindParentAndNode(Root,x);
  
    if(parent!=NULL)
    {
        if(parent->left->key==x)
        {
            required=parent->left;
        
            if(required->Ltag==0 && required->Rtag==0)
            {
            parent->left=required->left;
            parent->Ltag=0;
            delete required;
            return;
            }
            
            if(required->Ltag==1 && required->Rtag==0)
            {
            TreeNode * predecessor=inPred(required);
            predecessor->right=required->right;
            parent->left=required->left;
            delete required;
            return;
            }
            
            if(required->Ltag==0 && required->Rtag==1)
            {
            TreeNode * successor=inSucc(required);
            successor->left=required->left;
            parent->left=required->right;
            delete required;
            return;
            }
            
            if(required->Ltag==1 && required->Rtag==1)
            {
            TreeNode * successor=inSucc(required);
            int replace=successor->key;
            deletion(successor->key);
            required->key=replace;
            return;
            }
            
        }
        else    
        {
            required=parent->right;
            if(required->Ltag==0 && required->Rtag==0)
            {
            parent->right=required->right;
            parent->Rtag=0;
            delete required;
            return;
            }
            
            if(required->Ltag==1 && required->Rtag==0)
            {
            TreeNode * predecessor=inPred(required);
            predecessor->right=required->right;
            parent->right=required->left;
            delete required;
            return;
            }
        
            if(required->Ltag==0 && required->Rtag==1)
            {
            TreeNode * successor=inSucc(required);
            successor->left=required->left;
            parent->right=required->right;
            delete required;
            return;
            }
            
            if(required->Ltag==1 && required->Rtag==1)
            {
            TreeNode * successor=inSucc(required);
            int replace=successor->key;
            deletion(successor->key);
            required->key=replace;
            return;
            }
            
        }
    }
    else
    {
        cout<<"cannot be deleted as ELEMENT not found";
    }
    
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int row,index,key,left,right;
    Matrix.pb({0,{0,0}});
    ifstream infile("inputf.in");
    char ch;
    while (infile>>key>>left>>right>>ch)
    {
        if(ch=='c')
        Matrix.pb({key,{left,right}});
        if(ch=='i')
        Insert.pb({key,{left,right}});
        if(ch=='d')
        ToBeDeleted.pb(key);
    }
    Root=create(NULL,1);
    Dummy=convertToThreaded(Root);
    
    SetupSuccessor(Root);
    SetupPredecessor(Root);
    cout<<"* Inorder using Successor :"<<endl;
    inorderSuccessorTraversal(Dummy,INT_MAX,1);
    cout<<endl;
    cout<<endl<<"* Preorder using Predecessor :"<<endl;
    inorderPredecessorTraversal(Dummy,INT_MAX,1);
    cout<<endl<<endl;
    
    cout<<"----------------Insertion-------------------"<<endl;
    
    Insertion(Root);
        
    inorder.clear();
    preorder.clear();
     
    cout<<"* Inorder using Successor after insertion:"<<endl;
    inorderSuccessorTraversal(Dummy,INT_MAX,1);
    cout<<endl;
    cout<<endl<<"* Preorder using Predecessor after insertion :"<<endl;
    inorderPredecessorTraversal(Dummy,INT_MAX,1);
    
    cout<<endl<<"---------------Tree in a Matrix format--------------"<<endl;
    
    checkTree* TestRoot=build(preorder,inorder);
    vector<vector<int>> newmatrix;
    fillMatrix(TestRoot, newmatrix);
    cout<<endl<<endl;
    cout << "New Matrix"<< endl;
    printMatrix(newmatrix);
    
    cout<<"----------------------Deletion------------------"<<endl;
    cout<<"Inorder traversal before deletion:"<<endl;
    inorderSuccessorTraversal(Dummy,INT_MAX,1);
    cout<<endl;
    for(int i=0;i<ToBeDeleted.size();i++)
    {
        deletion(ToBeDeleted[i]);
    } 
    cout<<"* Inorder using Successor after deletion :"<<endl;
    inorderSuccessorTraversal(Dummy,INT_MAX,1);
    
}
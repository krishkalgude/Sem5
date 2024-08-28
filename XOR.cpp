#include<bits/stdc++.h>
using namespace std;
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
const int mod=1e9+7;

typedef struct node {
	struct node* left;
	int key;
	struct node* right;
}TreeNode;

vector<pair<int,pi>> Matrix;
TreeNode* Root;
TreeNode* XOR(TreeNode* a,TreeNode* b)
{
	return (TreeNode*)(((unsigned int )(a)^((unsigned int)(b))));
}
TreeNode* create(TreeNode* parent,TreeNode* temp,int index)
{
	TreeNode* tempL,*tempR,*root;
	if(parent==NULL){
 	temp=new TreeNode;
 	cout<<"root="<<temp<<" "<<endl;
 	root=temp;
 	}
 	
 	int Key=Matrix[index].F,indexL=Matrix[index].S.F,indexR=Matrix[index].S.S;
    
 	temp->key=Key;
 	cout<<Key<<" "<<endl;
 	if(indexL!=0)
 	{
 		
 		tempL=new TreeNode;
 		cout<<"tempL"<<tempL<<" ";
 		temp->left=XOR(parent,tempL);
 		
 	}
 	else{
 		temp->left=XOR(NULL,parent);
 	}
 	cout<<"temp->left"<<temp->left<<" ";
 	if(indexR!=0)
 	{
 		tempR=new TreeNode;
 		cout<<"tempR"<<tempR<<" ";
 		temp->right=XOR(parent,tempR);
 	}
 	else{
 		temp->right=XOR(NULL,parent);
 	}
 	cout<<"temp->right"<<temp->right<<" ";
 	cout<<endl;
 	parent=temp;
 	if(indexL!=0)
 	{
 		create(parent,tempL,indexL);
 	}
 	if(indexR!=0)
 	{
 	create(parent,tempR,indexR);
 	}
 	
 	
 	return root;
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int row,index,key,left,right;
    Matrix.pb({0,{0,0}});
    ifstream infile("inputf.in");
    
    while (infile>>key>>left>>right)
	{
    	Matrix.pb({key,{left,right}});
	}
	for(auto e:Matrix)
	{
		cout<<e.F<<" "<<e.S.F<<" "<<e.S.S<<endl;
	}
	Root=create(NULL,NULL,1);
	
    
    
    return 0;
}
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
using namespace std;

//模板区
#pragma region 
template <class T> class vecTor
{
  public:
    int _size = 0;
    int _capacity = 0;
    T *_elem = 0;
    vecTor()
    {
        _elem = new T[10]; //存储单元
        _capacity = 20;    //容量
        _size = 0;         //实际存入的数据大小
    }
    ~vecTor() { delete[] _elem; } //析构函数
    int size() const { return _size; }
    bool empty() const { return !_size; }
    vecTor<T> &operator=(vecTor<T> const &);
    T &operator[](int r) const //重定义[]
    {
        return _elem[r];
    }
    void expand();
    void shrink();
    int find(T const &a, int ptr, int end); //无序状态下的查找函数，O（n）
    int insert(int r, T const &a);
    int remove(int fir, int end);
    T remove(int r);
    int search(T const &a); //有序状态下的查找函数，O（logn）
    void push(T const &a) { insert(size(), a); } //相当于vector中的pushback
    void clear()                                 //置空
    {
        _size = 0;
        for (int i = 0; i < _capacity; i++)
        {
            _elem[i] = 0;
        }
    }
};

template <class T> vecTor<T> &vecTor<T>::operator=(vecTor<T> const &a)
{
    if (_elem)
        delete[] _elem;
    _capacity = 2 * a.size();
    _elem = new T[_capacity];
    _size = 0;
    int ptr = 0;
    while (ptr < a.size())
    {
        _elem[_size++] = a._elem[ptr++];
    }
}

template <class T> void vecTor<T>::expand()
{
    if (_size < _capacity)
        return;
    T *oldElem = _elem;
    _capacity = max(10, _capacity * 2);
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i];
    }
    delete[] oldElem;
}

template <class T> void vecTor<T>::shrink()
{
    if (_size > _capacity / 4)
        return;
    T *oldElem = _elem;
    _capacity /= 2;
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i];
    }
    delete[] oldElem;
}

template <class T> int vecTor<T>::find(T const &a, int fir, int ptr)
{
    while (fir < ptr)
    {
        if (_elem[ptr] == a)
        {
            break;
        }
        ptr--;
    }
    return ptr;
}

template <class T> int vecTor<T>::insert(int r, T const &a)
{
    expand();
    for (int i = _size; i > r; i--)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = a;
    _size++;
    return r;
}

template <class T> int vecTor<T>::remove(int fir, int end)
{
    int type = fir;
    if (fir >= end)
        return -1;
    while (end < _size)
    {
        _elem[fir++] = _elem[end++];
    }
    _size = fir;
    shrink();
    return type;
}

template <class T> T vecTor<T>::remove(int r)
{
    T temp = _elem[r];
    remove(r, r + 1);
    return temp;
}

template <class T> int vecTor<T>::search(T const &a)
{
    int front = 0, rear = _size, mid = -1;
    while (front < rear)
    {
        mid = (front + rear) / 2;
        if ((a < _elem[mid]))
            rear = mid;
        else if (a > _elem[mid])
            front = mid + 1;
        else
            return mid;
    }
    return -1;
}



#define BTNodePosi(T) BTNode<T>*

template <typename T> struct BTNode
{
  public:
   BTNodePosi(T) parent;
   vecTor<T> key;
   vecTor<BTNodePosi(T)> child;

   BTNode(){parent=NULL;child.insert(0,NULL);}
   BTNode(T e,BTNodePosi(T) lc=NULL,BTNodePosi(T) rc=NULL)
   {
     parent=NULL;
     key.insert(0,e);
     child.insert(0,lc);
     child.insert(1,rc);
     if(lc) 
       lc->parent=this;
     if(rc)
       rc->parent=this;
   }
};

template <typename T> class BTree 
{ 
  protected:    
    int _size;
    int _order;
    BTNodePosi(T) _root;
    BTNodePosi(T) _hot;
  public:
    BTree(int order=3):_order(order),_size(0)
    { _root=new BTNode<T>();}
    ~BTree() {if(_root) delete _root;}
    int const order() { return _order;}
    int const size() { return _size;}
    BTNodePosi(T) & root() { return _root;}
    bool empty() const { return !_root;}
    BTNodePosi(T) search(const T& e);
    bool insert(const T& e);
    bool remove(const T& e);
    void solveOverflow(BTNodePosi(T));
    void solveUnderflow(BTNodePosi(T));
 };

template <typename T> BTNodePosi(T) BTree<T>::search(const T& e) 
{ 
  BTNodePosi(T) v=_root; 
  _hot=_root;
  while (v) 
  {
    int r=v->key.search(e);
    if ((0<= r)&&(e==v->key[r])) 
      return v;
    _hot=v;
    v=v->child[r+1]; 
  }
  return NULL;
}

template <typename T> bool BTree<T>::insert(const T& e)
{ 
  BTNodePosi(T) v=search(e); 
  /*if (v) return false;*/
  int r=_hot->key.search(e);
  _hot->key.insert(r+1,e);
  _hot->child.insert(r+2,NULL);
  _size++;
  solveOverflow(_hot);
  return true;
}

template <typename T> void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
  if(_order >= v->child.size()) return;
  int s=_order/2;
  BTNodePosi(T) u=new BTNode<T>();
  for(int j=0;j<_order-s-1;j++) 
  {
    u->child.insert(j,v->child.remove(s+1));
    u->key.insert(j, v->key.remove(s+1));
  }
  u->child[_order-s-1]=v->child.remove(s+1);
  if(u->child[0])
  {
    for(int j=0;j<_order-s;j++)
    {
      u->child[j]->parent=u;
    }
  }
  BTNodePosi(T) p=v->parent;
  if(!p) 
  { 
    p=new BTNode<T>();
    _root=p;
    p->child[0]=v; 
    v->parent=p;
  }
  int r=1+p->key.search(v->key[0]);
  p->key.insert(r,v->key.remove(s));
  p->child.insert(r+1,u);
  u->parent=p;
  solveOverflow(p);
}

template <typename T> bool BTree<T>::remove(const T& e) 
{
  BTNodePosi(T) v=search(e); 
  if (!v) return false;
  int r=v->key.search(e);
  if(v->child[0]) 
  {
    BTNodePosi(T) u=v->child[r+1];
    while(u->child[0]) u=u->child[0];
    v->key[r]=u->key[0];
    v=u;
    r=0;
  }
  v->key.remove(r);
  v->child.remove(r+1);
  _size--;
  solveUnderflow(v);
  return true;
}

template <typename T> void BTree<T>::solveUnderflow(BTNodePosi(T) v) 
{
  if((_order+1)/2<=v->child.size()) 
    return;
  BTNodePosi(T) p=v->parent;
  if(!p) 
  {
    if(!v->key.size()&&v->child[0])
    {
      _root=v->child[0];
      _root->parent=NULL;
      v->child[0]=NULL;
      delete v;
    }
    return;
  }
  int r=0;
  while(p->child[r]!=v) r++;
   //情况1：向左兄弟借关键码
   if(r>0) 
  {
    BTNodePosi(T) ls=p->child[r-1];
    if ((_order + 1)/2<ls->child.size())
    {
      v->key.insert(0,p->key[r-1]);
      p->key[r-1]=ls->key.remove(ls->key.size()-1);
      v->child.insert(0,ls->child.remove(ls->child.size()-1));
      if (v->child[0]) 
        v->child[0]->parent=v;
      return;
    }
   }
   //情况2：向右兄弟借关键码
   if(p->child.size()-1>r)
   {
     BTNodePosi(T) rs=p->child[r+1];
     if ((_order+1)/2<rs->child.size()) 
     {
       v->key.insert(v->key.size(),p->key[r]);
       p->key[r]=rs->key.remove(0);
       v->child.insert(v->child.size(),rs->child.remove(0));
       if (v->child[v->child.size()-1])
       v->child[v->child.size()-1]->parent=v;
       return;
     }
    }
   //情况3：左右兄弟要么为空（但不可能同时），要么都太瘦——合并
   if(0<r) 
   {
     BTNodePosi(T) ls=p->child[r-1];
     ls->key.insert(ls->key.size(),p->key.remove(r-1)); 
     p->child.remove(r);
     ls->child.insert(ls->child.size(),v->child.remove(0));
     if(ls->child[ls->child.size()-1])
       ls->child[ls->child.size()-1]->parent=ls;
     while(!v->key.empty()) 
     {
       ls->key.insert(ls->key.size(),v->key.remove(0));
       ls->child.insert(ls->child.size(),v->child.remove(0));
       if(ls->child[ls->child.size()-1]) 
         ls->child[ls->child.size()-1]->parent=ls;
     }
     delete v;
   } 
   else 
   {
     BTNodePosi(T) rs=p->child[r+1];
     rs->key.insert(0,p->key.remove(r)); 
     p->child.remove(r);
     rs->child.insert(0,v->child.remove(v->child.size()-1));
     if(rs->child[0]) 
       rs->child[0]->parent=rs;
     while (!v->key.empty()) 
     {
       rs->key.insert(0,v->key.remove(v->key.size()-1));
       rs->child.insert(0,v->child.remove(v->child.size()-1));
       if (rs->child[0]) 
         rs->child[0]->parent=rs;
     }
     delete v;
   }
   solveUnderflow(p);
   return;
}
#pragma endregion

//变量区
BTree<int> tree(3);

//函数区
#pragma region
void travel(BTNodePosi(int) root)
{
  if(root==NULL)
  {
     return ;
  } 
  int final=root->key.size();
  int i=0;
  for(;i<final;i++)
  {
    travel(root->child[i]);
    cout<<root->key[i]<<endl;
  }
  travel(root->child[i]);
}
void action_1(int object)
{
   tree.insert(object);
}
void action_2(int object)
{
  /*if(!tree.remove(object))
    cout<<"Delete failed!"<<endl;*/
    tree.remove(object);
}
void action_3(int object)
{
   stack<BTNodePosi(int)> temp;
   int final_answer=0;
   temp.push(tree.root());
   while(!temp.empty())
   {
     BTNodePosi(int) temp_check=temp.top();
     temp.pop();
     for(int i=0;i<temp_check->key.size();i++)
     {
       if(temp_check->key[i]<object)
       {
         final_answer++;
       }
     }
     for(int i=0;i<temp_check->child.size();i++)
      {
        if(temp_check->child[i]!=NULL)
        {
          temp.push(temp_check->child[i]);
        }
      }
     
   }
   cout<<final_answer+1<<endl;
   //travel(tree.root());
}
void action_4(int object)
{
   if(object>tree.size())
   {
     cout<<"Lookup failed!"<<endl;
     return;
   }
   stack<BTNodePosi(int)> temp;
   priority_queue<int> que;
   temp.push(tree.root());
   while(!temp.empty())
   {
     BTNodePosi(int) temp_check=temp.top();
     temp.pop();
     for(int i=0;i<temp_check->key.size();i++)
     {
       que.push(temp_check->key[i]);
     }
     for(int i=0;i<temp_check->child.size();i++)
      {
        if(temp_check->child[i]!=NULL)
        {
          temp.push(temp_check->child[i]);
        }
      }
   }
   int i=1;
   int cmp=tree.size()-object+1;
   while(i<cmp)
   {
     que.pop();
     i++;
   }
   cout<<que.top()<<endl;
}
void action_5(int object)
{
   stack<BTNodePosi(int)> temp;
   priority_queue<int> que;
   temp.push(tree.root());
   while(!temp.empty())
   {
     BTNodePosi(int) temp_check=temp.top();
     temp.pop();
     for(int i=0;i<temp_check->key.size();i++)
     {
       que.push(temp_check->key[i]);
     }
     for(int i=0;i<temp_check->child.size();i++)
      {
        if(temp_check->child[i]!=NULL)
        {
          temp.push(temp_check->child[i]);
        }
      }
   }
   vector<int> temp_num;
   while(!que.empty())
   {
     temp_num.push_back(que.top());
     que.pop();
   }
   if(temp_num[temp_num.size()-1]>=object)
   {
    cout<<"Lookup failed!"<<endl;
    return;
   }
   else if(temp_num[0]<=object)
   {
     cout<<temp_num[0]<<endl;
     return;
   }
   for(int i=1;i<temp_num.size()-1;i++)
   {
     if((temp_num[i-1]>=object)&&(temp_num[i]<object))
     {
       cout<<temp_num[i]<<endl;
       return;
     }
   }
   cout<<"Lookup failed!"<<endl;
   return;
}
void action_6(int object)
{
   stack<BTNodePosi(int)> temp;
   priority_queue<int> que;
   temp.push(tree.root());
   while(!temp.empty())
   {
     BTNodePosi(int) temp_check=temp.top();
     temp.pop();
     for(int i=0;i<temp_check->key.size();i++)
     {
       que.push(temp_check->key[i]);
     }
     for(int i=0;i<temp_check->child.size();i++)
      {
        if(temp_check->child[i]!=NULL)
        {
          temp.push(temp_check->child[i]);
        }
      }
   }
   vector<int> temp_num;
   while(!que.empty())
   {
     temp_num.push_back(que.top());
     que.pop();
   }
   if(temp_num[temp_num.size()-1]>=object)
   {
    cout<<temp_num[temp_num.size()-1]<<endl;
    return;
   }
   else if(temp_num[0]<=object)
   {
     cout<<"Lookup failed!"<<endl;
     return;
   }
   for(int i=1;i<temp_num.size()-1;i++)
   {
     if((temp_num[i-1]>object)&&(temp_num[i]<=object))
     {
       cout<<temp_num[i-1]<<endl;
       return;
     }
   }
   cout<<"Lookup failed!"<<endl;
   return;
}
#pragma endregion

//Splay
#pragma region 
#define N 100005
int n,m; 
int fa[N],ch[N][2],size[N],rev[N],rt;
inline void pushup(int x)
{
    size[x]=size[ch[x][0]]+size[ch[x][1]]+1;
}
void pushdown(int x)
{
    if(rev[x])
    {
      swap(ch[x][0],ch[x][1]);
      rev[ch[x][0]]^=1;rev[ch[x][1]]^=1;rev[x]=0;
    }
}
void rotate(int x,int &k)
{
    int y=fa[x],z=fa[y],kind;
    if(ch[y][0]==x)kind=1;else kind=0;
    if(y==k)
      k=x;
    else
    {
      if(ch[z][0]==y)
        ch[z][0]=x;
      else 
        ch[z][1]=x;
    }
    ch[y][kind^1]=ch[x][kind];fa[ch[y][kind^1]]=y;
    ch[x][kind]=y;fa[y]=x;fa[x]=z;
    pushup(x);pushup(y);
}
void splay(int x,int &k)
{
    while(x!=k)
    {
      int y=fa[x],z=fa[y];
      if(y!=k)
      {
        if((ch[y][0]==x)^(ch[z][0]==y))
          rotate(x,k);
        else 
          rotate(y,k);
      }
      rotate(x,k);
    }
}
void build(int l,int r,int f)
{
    if(l>r) return;
    int mid=(l+r)/2;
    if(mid<f)
      ch[f][0]=mid;
    else 
      ch[f][1]=mid;
    fa[mid]=f;
    size[mid]=1;
    if(l==r) return;
    build(l,mid-1,mid);
    build(mid+1,r,mid);
    pushup(mid);
}
int find(int x,int k)
{
    pushdown(x);int s=size[ch[x][0]];
    if(k==s+1)return x;
    if(k<=s)return find(ch[x][0],k);
    else return find(ch[x][1],k-s-1);
}
void rever(int l,int r)
{
    int x=find(rt,l),y=find(rt,r+2);
    splay(x,rt);splay(y,ch[x][1]);int z=ch[y][0];
    rev[z]^=1;
}
void Splay()
{
    scanf("%d%d",&n,&m);
    rt=(n+3)/2;build(1,n+2,rt);
    for(int i=1;i<=m;i++)
    {
      int L,R;scanf("%d%d",&L,&R);
      rever(L,R);
    }
    for(int i=2;i<=n+1;i++)
      printf("%d ",find(rt,i)-1);
}
#pragma endregion

int main()
{
  int num_action;
  int temp_action;
  int num_func;
  //操作对象
  int action_object;
  
  cin>>num_func;
  if(num_func==1)
  {
    cin>>num_action;
    for(int i=0;i<num_action;i++)
    {
      cin>>temp_action>>action_object;
      switch(temp_action)
      {
        case 1:
          action_1(action_object);
          break;
        case 2:
          action_2(action_object);
          break;
        case 3:
          action_3(action_object);
          break;
        case 4:
          action_4(action_object);
          break;
        case 5:
          action_5(action_object);
          break;
        case 6:
          action_6(action_object);
          break;
        default:
          cout<<"Command error!"<<endl;
          break;
      }
    }
  }
  else 
  {
     Splay();
  }
  return 0;
}
#include<iostream>
#include<stack>
#include<queue>
#include<string>
using namespace std;

#pragma region 
#define IsRoot(x) (!((x).parent))
#define IsLChlid(x) (!IsRoot(x)&&(&(x)==(x).parent->lChild))
#define IsRChlid(x) (!IsRoot(x)&&(&(x)==(x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x)||HasRChild(x))
#define HasBothChild(x) (HasLChild(x)&&HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) (IsLChlid(*(p))?(p)->parent->rChild:(p)->parent->lChild)
#define uncle(x) (IsLChlid(*((x)->parent))?(x)->parent->parent->rChild:(x)->parent->parent->lChild)
#define FromParentTo(x) (IsRoot(x) ?_root:(IsLChlid(x)?(x).parent->lChild:(x).parent->rChild))
#pragma endregion

#pragma region 
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height:-1)
typedef enum {RB_RED,RB_BLACK} RBColor;

template <typename T> struct BinNode
{
   public:
   T data;
   BinNodePosi(T) parent;
   BinNodePosi(T) lChild;
   BinNodePosi(T) rChild;
   int height;             //高度
   RBColor color;

   //构造函数
   BinNode():parent(NULL),lChild(NULL),rChild(NULL),height(0),color(RB_RED){}
   BinNode(T e,BinNodePosi(T) p=NULL,BinNodePosi(T) lc=NULL,BinNodePosi(T) rc=NULL,
   int h=0,RBColor c=RB_RED):data(e),parent(p),lChild(lc),rChild(rc),height(h),color(c){}
   //操作接口
   int size();                                   //后代总数
   BinNodePosi(T) insertAsLC(T const&);          //插入左孩子
   BinNodePosi(T) insertAsRC(T const&);          //插入右孩子
   BinNodePosi(T) succ();                        //取当前节点的直接后继
   template<typename VST> void travLevel(VST&);  //子树层次遍历
   template<typename VST> void travPre(VST&,int num);    //子树先序遍历
   template<typename VST> void travIn(VST&,int num);     //子树中序遍历
   template<typename VST> void travPost(VST&,int num);   //子树后序遍历
};

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e){ return lChild=new BinNode(e,this);}
template<typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e){ return rChild=new BinNode(e,this);}
template<typename T> template<typename VST>
void BinNode<T>::travPre(VST& visit,int num)
{
    switch(num)
    {
      case 1:travPre_I(this,visit);break;
      case 2:travPre_R(this,visit);break;
      default:break;
    }
}
template<typename T> template<typename VST>
void BinNode<T>::travIn(VST& visit,int num)
{
    switch(num)
    {
      case 1:travIn_I(this,visit);break;
      case 2:travIn_R(this,visit);break;
      default:break;
    }
}
template<typename T> template<typename VST>
void BinNode<T>::travPost(VST& visit,int num)
{
    switch(num)
    {
      case 1:travPost_I(this,visit);break;
      case 2:travPost_R(this,visit);break;
      default:break;
    }
}
#pragma endregion

#pragma region 
template<typename T>class BinTree{
protected:
  int _size;
  BinNodePosi(T) _root;
  virtual int updateHeight(BinNodePosi(T) x);
  void updateHeightAbove(BinNodePosi(T) x);
public:
  BinTree(): _size(0),_root(NULL){}
  ~BinTree(){ if(0<_size) remove(_root);}
  int size() const {return _size;}
  bool empty() const {return !_root;}
  BinNodePosi(T) root() const {return _root;}
  BinNodePosi(T) insertAsRoot(T const&e);
  BinNodePosi(T) insertAsLC(BinNodePosi(T) x,T const& e);
  BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const& e);
  BinNodePosi(T) attachAsLC(BinNodePosi(T) x,BinTree<T>* &S);
  BinNodePosi(T) attachAsRC(BinNodePosi(T) x,BinTree<T>* &S);
  int remove(BinNodePosi(T) x);
  BinTree<T>* secede(BinNodePosi(T) x);
  template <typename VST>
  void travLevel(VST& visit){if(_root) _root->travLevel(visit);}
  template <typename VST>
  void travPre(VST& visit){if(_root) _root->travPre(visit);}
  template <typename VST>
  void travIn(VST& visit){if(_root) _root->travIn(visit);}
  template <typename VST>
  void tarvPost(VST& visit){if(_root) _root->travPost(visit);}

};
//高度更新
template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x)
{return x->height=1+max(stature(x->lChild),stature(x->rChild));}
template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{while(x) {updateHeight(x);x=x->parent;}}
//节点插入
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{_size=1; return _root=new BinNode<T>(e);}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x,T const& e)
{_size++;x->insertAsLC(e);updateHeightAbove(x);return x->lChild;}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x,T const& e)
{_size++;x->insertAsRC(e);updateHeightAbove(x);return x->rChild;}
//子树接入
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x,BinTree<T>* &S)
{
    if(x->lChild=S->_root) x->lChild->parent=x;
    _size+=S->_size;updateHeightAbove(x);
    S->_root=NULL;S->_size=0;release(S);S=NULL;return x;
}
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x,BinTree<T>* &S)
{
    if(x->rChild=S->_root) x->rChild->parent=x;
    _size+=S->_size;updateHeightAbove(x);
    S->_root=NULL;S->_size=0;release(S);S=NULL;return x;
}
//子树删除
template <typename T> 
int BinTree<T>::remove(BinNodePosi(T) x) 
{ 
   FromParentTo(*x) = NULL; 
   updateHeightAbove(x->parent);
   int n = removeAt(x); _size -= n; return n;
}
template <typename T>
static int removeAt(BinNodePosi(T) x) 
{ 
   if (!x) return 0;
   int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
   delete x; return n;
}
//子树分离
template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) 
{
   FromParentTo(*x) = NULL;
   updateHeightAbove(x->parent);
   BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL;
   S->_size = x->size(); _size -= S->_size; return S;
 }
//先序遍历
//递归版
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) 
{
   if (!x) return;
   visit(x->data);
   travPre_R(x->lChild, visit);
   travPre_R(x->rChild, visit);
}
//迭代版
template <typename T, typename VST> 
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, stack<BinNodePosi(T)>& S) 
{
   while (x) 
   {
     visit(x->data);
     S.push(x->rChild);
     x=x->lChild;
   }
 }
template <typename T, typename VST>
void travPre_I(BinNodePosi(T) x, VST& visit) 
{
   stack<BinNodePosi(T)> S;
   while (true) 
   {
     visitAlongLeftBranch(x, visit, S);
     if (S.empty()) break;
     x=S.top();
     S.pop();
   }
}
//后序遍历
//递归版
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit) 
{ 
   if (!x) return;
   travPost_R(x->lChild, visit);
   travPost_R(x->rChild, visit);
   visit(x->data);
}
//迭代版
template <typename T>
static void gotoHLVFL(stack<BinNodePosi(T)>& S) 
{ 
   while (BinNodePosi(T) x = S.top())
   if (HasLChild(*x)) 
   {
     if (HasRChild(*x)) S.push(x->rChild);
     S.push(x->lChild);
   } 
   else
     S.push(x->rChild);
   S.pop();
 }
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) 
{ 
  stack<BinNodePosi(T)> S;
  if (x) S.push(x);
  while (!S.empty()) 
  {
    if (S.top() != x->parent)
    gotoHLVFL(S);
    x=S.top();S.pop(); visit(x->data);
  }
}

//中序遍历
//递归版
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) 
{
   if (!x) return;
   travIn_R(x->lChild, visit);
   visit(x->data);
   travIn_R(x->rChild, visit);
}
//迭代版
template <typename T> 
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)>& S) {while(x) {S.push(x);x=x->lChild;}}
template <typename T, typename VST>
void travIn_I(BinNodePosi(T) x, VST& visit) 
{ 
  stack<BinNodePosi(T)> S;
  while (true) 
  {
    goAlongLeftBranch(x, S);
    if (S.empty()) break;
    x=S.top(); 
    S.pop();
    visit(x->data);
    x=x->rChild;
  }
}
//层次遍历
template <typename T> template <typename VST>
void BinNode<T>::travLevel(VST& visit) 
{ 
   queue<BinNodePosi(T)> Q;
   Q.push(this);
   while (!Q.empty()) 
   {
      BinNodePosi(T) x=Q.front(); Q.pop(); visit(x->data);
      if(HasLChild(*x)) Q.push(x->lChild);
      if(HasRChild(*x)) Q.push(x->rChild);
   }
}
#pragma endregion

#pragma region 
struct All{
  BinTree<int> tree;
  string name;
}all[100];
int tree_num;

int num_jiedian;

void look(int x)
{
   cout<<"data:"<<x<<endl;
}

void count(int x)
{
   num_jiedian++;
}

void pre_pri()
{
   cout<<"********欢迎您使用二叉树!*********"<<endl;
   cout<<"*      请选择您想进行的操作      *"<<endl;
   cout<<"*         1.建立二叉树           *"<<endl;
   cout<<"*         2.建立节点             *"<<endl;
   cout<<"*         3.查找二叉树           *"<<endl;
   cout<<"*         4.遍历二叉树           *"<<endl;
   cout<<"*         5.其他功能             *"<<endl;
   cout<<"*         6.退出                 *"<<endl;
   cout<<"**********************************"<<endl;
   cout<<"请输入您的选择:"<<endl;
}

void Tree_1()
{
  bool is_end=true;
  int temp_root;

  cout<<"******欢迎您使用建立二叉树功能*******"<<endl;
  cout<<"请输入您想建立的二叉树的名字:"<<endl;
  cin>>all[tree_num].name;
  cout<<"请输入您的二叉树的根节点的值:"<<endl;
  cin>>temp_root;
  all[tree_num].tree.insertAsRoot(temp_root);
  cout<<"提示:您已成功建立二叉树!执行返回操作"<<endl;
  cout<<"----------------------------------"<<endl;
  tree_num++;
}

void Tree_2()
{
   int temp_action;
   bool symbol=false;
   int temp_shunxu;
   int temp_data;
   string temp_name;
   BinNodePosi(int) temp_x;
   BinTree<int>* temp_tree;
   cout<<"******欢迎使用建立节点功能!*********"<<endl;
   cout<<"*    请选择您想建立节点的二叉树    *"<<endl;
   cout<<"*          1.按名称选择           *"<<endl;
   cout<<"*          2.按编号查找           *"<<endl;
   cout<<"***********************************"<<endl;
   cout<<"请输入您的选择:"<<endl;
   cin>>temp_action;
   switch(temp_action)
   {
     case 1:
       cout<<"请输入您所查找的二叉树名称:"<<endl;
       cin>>temp_name;
       for(int i=0;i<tree_num;i++)
        {
          if(all[i].name==temp_name)
          {
             symbol=true;
             cout<<"您所查找的二叉树名称为:"<<all[i].name<<endl;
             cout<<"该二叉树的规模为:"<<all[i].tree.size()<<endl;
             temp_x=all[i].tree.root();
             temp_tree=&all[i].tree;
          }
        }
        if(!symbol)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        break;
     case 2:
        cout<<"请输入您所查找的二叉树序号:"<<endl;
        cin>>temp_shunxu;
        if(temp_shunxu>tree_num||temp_shunxu<=0)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        else 
        {
          symbol=true;
          cout<<"您所查找的二叉树名称为:"<<all[temp_shunxu-1].name<<endl;
          cout<<"该二叉树的规模为:"<<all[temp_shunxu-1].tree.size()<<endl;
          temp_x=all[temp_shunxu-1].tree.root();
          temp_tree=&all[temp_shunxu-1].tree;
        }
        break;
     default:
       cout<<"警告:检测到格式错误!执行返回操作"<<endl;
       break;
   }
   if(!symbol) {return;}
   else 
   {
     cout<<"提示:查找成功!"<<endl;

     while(symbol)
     {
        #pragma region //相关信息
        cout<<"当前节点高度为:"<<temp_x->height<<endl;
        cout<<"当前节点信息为:"<<temp_x->data<<endl;
        if(HasLChild(*temp_x))
        {
          cout<<"当前节点已有左孩子."<<endl;
        }
        else 
        {
          cout<<"当前节点没有左孩子."<<endl;
        }
        if(HasRChild(*temp_x))
        {
          cout<<"当前节点已有右孩子."<<endl;
        }
        else
        {
          cout<<"当前节点没有右孩子."<<endl;
        }
        cout<<"----------------------------------"<<endl;
        #pragma endregion
        cout<<"*******您可以进行以下操作*********"<<endl;
        cout<<"*        1.建立左节点           *"<<endl;
        cout<<"*        2.建立右节点           *"<<endl;
        cout<<"*        3.查看左节点           *"<<endl;
        cout<<"*        4.查看右节点           *"<<endl;
        cout<<"*        5.查看兄弟节点         *"<<endl;
        cout<<"*        6.查看父亲节点         *"<<endl;
        cout<<"*        7.返回父亲节点         *"<<endl;
        cout<<"*        8.退出                 *"<<endl;
        cout<<"*********************************"<<endl;
        cout<<"请输入您的选择:"<<endl;
        cin>>temp_action;

        switch(temp_action)
        {
           case 1:
             if(HasLChild(*temp_x))
             {
               cout<<"警告:检测到非法操作!执行重置操作"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             else 
             {
               cout<<"请输入您想建立的左节点的信息:"<<endl;
               cin>>temp_data;
               temp_x=temp_tree->insertAsLC(temp_x,temp_data);
               cout<<"提示:建立成功!"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             break;
           case 2:
             if(HasRChild(*temp_x))
             {
               cout<<"警告:检测到非法操作!执行重置操作"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             else 
             {
               cout<<"请输入您想建立的右节点的信息:"<<endl;
               cin>>temp_data;
               temp_x=temp_tree->insertAsRC(temp_x,temp_data);
               cout<<"提示:建立成功!"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             break;
           case 3:
             if(HasLChild(*temp_x))
             {
                cout<<"当前节点左节点的高度为:"<<temp_x->lChild->height<<endl;
                cout<<"当前节点左节点的信息为:"<<temp_x->lChild->data<<endl;
                temp_x=temp_x->lChild;
                cout<<"提示:查看成功!执行重置操作"<<endl;
                cout<<"提示:当前控制权已转移到左节点."<<endl;
                cout<<"----------------------------------"<<endl;
             }
             else 
             {
                cout<<"警告:检测到非法操作!执行重置操作"<<endl; 
                cout<<"----------------------------------"<<endl;
             }
             break;
           case 4:
             if(HasRChild(*temp_x))
             {
                cout<<"当前节点右节点的高度为:"<<temp_x->rChild->height<<endl;
                cout<<"当前节点右节点的信息为:"<<temp_x->rChild->data<<endl;
                temp_x=temp_x->rChild;
                cout<<"提示:查看成功!执行重置操作"<<endl;
                cout<<"提示:当前控制权已转移到右节点."<<endl;
                cout<<"----------------------------------"<<endl;
             }
             else 
             {
                cout<<"警告:检测到非法操作!执行重置操作"<<endl;
                cout<<"----------------------------------"<<endl;
             }
             break;
           case 5:
             if(!temp_x->parent)
             {
               cout<<"警告:查询失败!"<<endl;
               cout<<"当前节点为根节点"<<endl;
             }
             else if(temp_x->parent->lChild==temp_x)
             {
               if(!temp_x->parent->rChild)
                 cout<<"当前节点无兄弟节点"<<endl;
               else 
                 cout<<"当前节点有兄弟节点,且兄弟节点的值为:"<<temp_x->parent->rChild->data<<endl;
             }
             else if(temp_x->parent->rChild==temp_x)
             {
               if(!temp_x->parent->lChild)
                 cout<<"当前节点无兄弟节点"<<endl;
               else 
                 cout<<"当前节点有兄弟节点,且兄弟节点的值为:"<<temp_x->parent->lChild->data<<endl;
             }
             cout<<"----------------------------------"<<endl;
             break;
           case 6:
             if(HasParent(*temp_x))
             {
               cout<<"当前节点父亲节点的高度为:"<<temp_x->parent->height<<endl;
               cout<<"当前节点父亲节点的信息为:"<<temp_x->parent->data<<endl;
               cout<<"提示:查看成功!"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             else 
             {
               cout<<"警告:检测到格式错误!执行重置操作"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             break;
           case 7:
             if(HasParent(*temp_x))
             {
               temp_x=temp_x->parent;
               cout<<"提示:返回成功!"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             else 
             {
               cout<<"警告:检测到格式错误!执行重置操作"<<endl;
               cout<<"----------------------------------"<<endl;
             }
             break;
           case 8:
             symbol=false;
             cout<<"欢迎您再次使用建立节点功能!"<<endl;
             cout<<"----------------------------------"<<endl;
             break;
           default:
             symbol=false;
             cout<<"警告:检测到非法输入!执行返回操作"<<endl;
             cout<<"----------------------------------"<<endl;
             break;
        }
     }
   }
}

void Tree_3()
{
    int temp_action;
    int temp_shunxu;
    string temp_name; 
    bool symbol=false;

    cout<<"******欢迎使用查找二叉树功能!******"<<endl;
    cout<<"*      请选择您查找的方式         *"<<endl;
    cout<<"*        1.按名称查找             *"<<endl;
    cout<<"*        2.按顺序查找             *"<<endl;
    cout<<"***********************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;
    switch(temp_action)
    {
      case 1:
        cout<<"请输入您所查找的二叉树名称:"<<endl;
        cin>>temp_name;
        for(int i=0;i<tree_num;i++)
        {
          if(all[i].name==temp_name)
          {
             symbol=true;
             cout<<"您所查找的二叉树名称为:"<<all[i].name<<endl;
             cout<<"该二叉树的规模为:"<<all[i].tree.size()<<endl;
             cout<<"提示:查找成功!执行返回操作"<<endl;
          }
        }
        if(!symbol)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        break;
      case 2:
        cout<<"请输入您所查找的二叉树序号:"<<endl;
        cin>>temp_shunxu;
        if(temp_shunxu>tree_num||temp_shunxu<=0)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        else 
        {
          cout<<"您所查找的二叉树名称为:"<<all[temp_shunxu-1].name<<endl;
          cout<<"该二叉树的规模为:"<<all[temp_shunxu-1].tree.size()<<endl;
          cout<<"提示:查找成功!执行返回操作"<<endl;
        }
        break;
      default:
        cout<<"警告:检测到非法输入!执行返回操作"<<endl;
        break;
    }
}

void Tree_4()
{
   int temp_action;
   int temp_action_1;
   int temp_action_2;
   int temp_action_3;
   int temp_action_4;
   int temp_shunxu;
   bool symbol=false;
   string temp_name;
   BinNodePosi(int) temp_x;
   BinTree<int>* temp_tree;
   
   cout<<"******欢迎您使用遍历二叉树功能*****"<<endl;
   cout<<"*    请选择您想建立节点的二叉树   *"<<endl;
   cout<<"*          1.按名称选择           *"<<endl;
   cout<<"*          2.按编号查找           *"<<endl;
   cout<<"***********************************"<<endl;
   cout<<"请输入您的选择:"<<endl;
   cin>>temp_action;
   switch(temp_action)
   {
     case 1:
       cout<<"请输入您所查找的二叉树名称:"<<endl;
       cin>>temp_name;
       for(int i=0;i<tree_num;i++)
        {
          if(all[i].name==temp_name)
          {
             symbol=true;
             cout<<"您所查找的二叉树名称为:"<<all[i].name<<endl;
             cout<<"该二叉树的规模为:"<<all[i].tree.size()<<endl;
             temp_x=all[i].tree.root();
             temp_tree=&all[i].tree;
          }
        }
        if(!symbol)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        break;
     case 2:
        cout<<"请输入您所查找的二叉树序号:"<<endl;
        cin>>temp_shunxu;
        if(temp_shunxu>tree_num||temp_shunxu<=0)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        else 
        {
          symbol=true;
          cout<<"您所查找的二叉树名称为:"<<all[temp_shunxu-1].name<<endl;
          cout<<"该二叉树的规模为:"<<all[temp_shunxu-1].tree.size()<<endl;
          temp_x=all[temp_shunxu-1].tree.root();
          temp_tree=&all[temp_shunxu-1].tree;
        }
        break;
     default:
       cout<<"警告:检测到格式错误!执行返回操作"<<endl;
       break;
   }
   if(!symbol) {return;}
   else 
   {
     cout<<"提示:查找成功!"<<endl;
     cout<<"----------------------------------"<<endl;
   }
   cout<<"******欢迎您使用遍历二叉树功能*****"<<endl;
   cout<<"*     请选择您想选择的遍历方式    *"<<endl;
   cout<<"*         1.先序遍历              *"<<endl;
   cout<<"*         2.中序遍历              *"<<endl;
   cout<<"*         3.后序遍历              *"<<endl;
   cout<<"*         4.层次遍历              *"<<endl;
   cout<<"*         5.退出                  *"<<endl;
   cout<<"***********************************"<<endl;
   cout<<"请输入您想进行的操作:"<<endl;
   cin>>temp_action;

   switch(temp_action)
   {
     case 1:
       cout<<"******请选择您想使用的代码版本*******"<<endl;
       cout<<"*         1.递归版                 *"<<endl;
       cout<<"*         2.迭代版                 *"<<endl;
       cout<<"************************************"<<endl;
       cout<<"请输入您的选择:"<<endl;
       cin>>temp_action_1;
       switch(temp_action_1)
       {
         case 1:
          temp_x->travPre(look,temp_action_1);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         case 2:
          temp_x->travPre(look,temp_action_1);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         default:
          cout<<"警告:检测到非法输入!执行返回操作"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
       }
       break;
     case 2:
       cout<<"******请选择您想使用的代码版本*******"<<endl;
       cout<<"*         1.递归版                 *"<<endl;
       cout<<"*         2.迭代版                 *"<<endl;
       cout<<"************************************"<<endl;
       cout<<"请输入您的选择:"<<endl;
       cin>>temp_action_2;
       switch(temp_action_2)
       {
         case 1:
          temp_x->travIn(look,temp_action_2);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         case 2:
          temp_x->travIn(look,temp_action_2);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         default:
          cout<<"警告:检测到非法输入!执行返回操作"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
       }
       break;
     case 3:
       cout<<"******请选择您想使用的代码版本*******"<<endl;
       cout<<"*         1.递归版                 *"<<endl;
       cout<<"*         2.迭代版                 *"<<endl;
       cout<<"************************************"<<endl;
       cout<<"请输入您的选择:"<<endl;
       cin>>temp_action_3;
       switch(temp_action_3)
       {
         case 1:
          temp_x->travPost(look,temp_action_3);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         case 2:
          temp_x->travPost(look,temp_action_3);
          cout<<"提示:遍历成功!执行返回操作"<<endl;
          cout<<"欢迎您再次使用遍历功能!"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
         default:
          cout<<"警告:检测到非法输入!执行返回操作"<<endl;
          cout<<"----------------------------------"<<endl;
          break;
       }
       break;
     case 4:
       temp_x->travLevel(look);
       cout<<"提示:遍历成功!执行返回操作"<<endl;
       cout<<"欢迎您再次使用遍历功能!"<<endl;
       cout<<"----------------------------------"<<endl;
       break;
     case 5:
       cout<<"欢迎您再次使用遍历功能!"<<endl;
       cout<<"----------------------------------"<<endl;
       break;
   }
}

void Tree_5()
{
  int temp_action;
  int temp_action_1;
  int temp_shunxu;
  BinNodePosi(int) temp_x;
  BinTree<int>* temp_tree;
  bool symbol;
  string temp_name;
  
  cout<<"********欢迎您使用其他功能**********"<<endl;
  cout<<"*      请选择您想使用的功能        *"<<endl;
  cout<<"*          1.叶结点数             *"<<endl;
  cout<<"*          2.查看兄弟节点         *"<<endl;
  cout<<"*          3.退出                 *"<<endl;
  cout<<"***********************************"<<endl;
  cout<<"请输入您想进行的操作:"<<endl;
  cin>>temp_action;

  switch(temp_action)
  {
     case 1:
       cout<<"******欢迎使用查看节点功能!*********"<<endl;
       cout<<"*    请选择您想建立节点的二叉树    *"<<endl;
       cout<<"*          1.按名称选择           *"<<endl;
       cout<<"*          2.按编号查找           *"<<endl;
       cout<<"***********************************"<<endl;
       cout<<"请输入您的选择:"<<endl;
       cin>>temp_action_1;
       switch(temp_action_1)
       {
         case 1:
         cout<<"请输入您所查找的二叉树名称:"<<endl;
         cin>>temp_name;
         for(int i=0;i<tree_num;i++)
         {
            if(all[i].name==temp_name)
            {
             symbol=true;
             cout<<"您所查找的二叉树名称为:"<<all[i].name<<endl;
             cout<<"该二叉树的规模为:"<<all[i].tree.size()<<endl;
             temp_x=all[i].tree.root();
             temp_tree=&all[i].tree;
            }
          }
          if(!symbol)
          {
            cout<<"警告:查找失败!执行返回操作"<<endl;
          }
          break;
       case 2:
        cout<<"请输入您所查找的二叉树序号:"<<endl;
        cin>>temp_shunxu;
        if(temp_shunxu>tree_num||temp_shunxu<=0)
        {
          cout<<"警告:查找失败!执行返回操作"<<endl;
        }
        else 
        {
          symbol=true;
          cout<<"您所查找的二叉树名称为:"<<all[temp_shunxu-1].name<<endl;
          cout<<"该二叉树的规模为:"<<all[temp_shunxu-1].tree.size()<<endl;
          temp_x=all[temp_shunxu-1].tree.root();
          temp_tree=&all[temp_shunxu-1].tree;
        }
        break;
       default:
         cout<<"警告:检测到格式错误!执行返回操作"<<endl;
         break;
      }
      if(!symbol) {return;}
      else 
      {
         num_jiedian=0;
         temp_x->travPre(count,1);
         cout<<"所有的叶节点数为:"<<num_jiedian<<endl;
         cout<<"提示:计数成功!执行返回操作"<<endl;
         cout<<"-----------------------------------"<<endl;
      }
      break;
     case 2:
       cout<<"提示:锤偷懒没写，去催催他吧"<<endl;
     case 3:
       cout<<"欢迎您再次使用其他功能!"<<endl;
       cout<<"-------------------------------------"<<endl;
       break;
     default:
       cout<<"警告:检测到非法输入!执行返回操作"<<endl;

  }
}

#pragma endregion

int main()
{
   bool is_end=true;
   int action;
   while(is_end)
   {
     pre_pri();
     cin>>action;
     switch(action)
     {
        case 1:
          Tree_1();
          break;
        case 2:
          Tree_2();
          break;
        case 3:
          Tree_3();
          break;
        case 4:
          Tree_4();
          break;
        case 5:
          Tree_5();
          break;
        case 6:
          is_end=false;
          //释放空间
          for(int i=0;i<tree_num;i++)
            all[i].tree.remove(all[i].tree.root());
          cout<<"欢迎您再次使用二叉树!"<<endl;
          break;
        default:
          is_end=false;
          cout<<"警告:检测到格式错误,执行退出操作"<<endl;
          break;
     }
   }
   return 0;
}
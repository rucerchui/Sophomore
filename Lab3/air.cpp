#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<stack>
#include<queue>
#include<string.h>
#include<stdlib.h>
using namespace std;

//变量声明区
#pragma region 
struct Time_1
{
   public:
   string time_data;
   int year;
   int month;
   int day;
   int hour;
   int minute;
   void get_time()
   {
      bool have_hour=false;
      for(int i=0;i<time_data.length();i++)
      {
         if(time_data[i]==' ')
           have_hour=true;
      }
      if(have_hour)
      {
         int start;
         int end;
         int temp;
         month=time_data[0]-'0';
         day=time_data[2]-'0';
         year=2017;
         for(int i=0;i<time_data.length();i++)
         {
            if(time_data[i]==' ')
              start=i;
            if(time_data[i]==':')
              end=i;
         }
         temp=0;
         for(int i=start+1;i<end;i++)
         {
           temp=temp*10+(time_data[i]-'0');
         }
         hour=temp;
         temp=0;
         for(int i=end+1;i<time_data.length();i++)
         {
           temp=temp*10+(time_data[i]-'0');
         }
         minute=temp;
      }
      else
      {
         month=time_data[0]-'0';
         day=time_data[2]-'0';
         year=2017;
      }
   }
   friend bool operator<(const Time_1 &a,const Time_1 &b)
   {
      return (((a.month<b.month))
             ||((a.month==b.month)&&(a.day<b.day))
             ||((a.month==b.month)&&(a.day==b.day)&&(a.hour<b.hour))
             ||((a.month<=b.month)&&(a.day<=b.day)&&(a.hour==b.hour)&&(a.minute<=b.minute)));
   }
   friend int operator-(const Time_1 &a,const Time_1 &b)
   {
      int temp_day=a.day-b.day-1;
      int temp_hour=24-a.hour+b.hour;
      int temp_minute=a.hour*60+a.minute+24*60-(b.hour*60+b.minute);
      return temp_day*24*60+temp_minute;
   }
};
struct Airdata
{
   int flight_id;
   Time_1 departure_date;
   int flight_number;
   int departure_airport;
   int arrival_airport;
   Time_1 departure_time;
   Time_1 arrival_time;
   int airplane_id;
   int airplane_model;
   int air_fares;
};
Airdata airdata[2500];   //航线数据集合
int total_num;           //读入数据的总条数
struct Bian
{
  int end_dian;
  Bian* next_bian=NULL;
};
struct Dian
{
  int num_port;
  Bian* next_bian=NULL;
};
Dian dian[80];
Bian bian[2500];
struct qnode
{
   int now_time;
   int end_airport;
   int weight;
   int total_time;
   string across_air="";
   qnode(int time,int end):now_time(time),end_airport(end),across_air(""){}
   qnode(int time,int end,string across,int weight):now_time(time),end_airport(end),across_air(across),weight(weight){}
   qnode(int time,int end,string across,int weight,int total_time):
     now_time(time),end_airport(end),across_air(across),weight(weight),total_time(total_time){}
   friend bool operator<(const qnode& a,const qnode& b)
   {
      return a.now_time<b.now_time;
   }
};
string air_answer[5000];
#pragma endregion

//函数声明区
#pragma region
void Get_data();
void First_printf();
void Func_1();
void Func_1_1(int airport_id);
void Func_1_2(int airport_id);
void Make_chart();
void Delete_chart();
void Func_1_2_dfs(int* temp_arrport,int* temp_port_check,int now);
void Func_2();
void Func_2_1(int round_times);
void Func_3();
int  Func_3_1(int air_port_1,int air_port_2,int round_times);
void Func_4();
int  Func_4_1(int air_id_1,int air_id_2);
bool Judge_connect(int air_id_1,int air_id_2);
void Func_5();
int  Func_5_1(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2);
int  Func_5_1_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2);
int  Func_5_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2);
int  Func_5_2_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2);
int  Func_5_3(int air_id_1,int air_id_2,int round_time,int airplane_mod);
int  Func_5_3_2(int air_id_1,int air_id_2,int round_time,int airplane_mod);
void Func_6();
int  Func_6_1(int air_id_1,int air_id_2,Time_1 time1,Time_1 time2);
int  Func_6_2(int air_id_1,int air_id_2,Time_1 time1,Time_1 time2);
int  Func_6_3(int air_id_1,int air_id_2,int airplane_mod);
void Func_7();
int  Func_7_1(int air_id_1,int air_id_2,int round_time,int temp_time);
int  Func_7_2(int air_id_1,int air_id_2,int round_time,int temp_time);
void Func_8();
int  Func_8_1(int air_id_1,int air_id_2,int temp_time);
int  Func_8_2(int air_id_1,int air_id_2,int temp_time);
void Func_developer();
#pragma endregion

//图模板区
#pragma region 
//顶点状态
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;
//边状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; 
//图Graph模板类
template <typename Tv, typename Te> //顶点类型、边类型
class Graph {
  private:
    void reset() 
    { 
      //所有顶点、边的辅助信息复位
      for (int i=0;i<n;i++) 
      { 
        //所有顶点的
        //状态
        status(i)=UNDISCOVERED;
        //时间标签
        dTime(i)=fTime(i)=-1;
        //（在遍历树中的）父节点
        parent(i) =-1;
        //优先级数
        priority(i)=INT_MAX;
        //所有边的
        for (int j=0;j<n;j++)
          if (exists(i,j)) 
          //状态
          status(i,j)=UNDETERMINED;
      }
     }
    //（连通域）广度优先搜索算法
    void BFS(int,int&);
    //（连通域）深度优先搜索算法
    void DFS(int,int&); 
    //（连通域）基于DFS的双连通分量分解算法
    void BCC(int,int&,stack<int>&);
    //（连通域）基于DFS癿拓扑排序算法
    bool TSort(int,int&,stack<Tv>*);
    //（连通域）优先级搜索框架
    template <typename PU> void PFS(int,PU);
  public:
   // 顶点
   //顶点总数
   int n;
   //插入顶点，返回编号
   virtual int insert(Tv const&)=0;
   //删除顶点及其关联边，返回该顶点信息
   virtual Tv remove(int)=0;
   //顶点v的数据（该顶点的确存在）
   virtual Tv& vertex(int)=0;
   //顶点v的入度（该顶点的确存在）
   virtual int inDegree(int)=0;
   //顶点v的出度（该顶点的确存在）
   virtual int outDegree(int)=0;
   //顶点v的首个邻接顶点
   virtual int firstNbr(int)=0;
   //顶点v的（相对于顶点j的）下一邻接顶点
   virtual int nextNbr(int, int)=0;
   //顶点v的状态
   virtual VStatus& status(int)=0;
   //顶点v的时间标签dTime
   virtual int& dTime(int)=0;
   //顶点v的时间标签fTime
   virtual int& fTime(int)=0;
   //顶点v在遍历树中的父亲
   virtual int& parent(int)=0;
   //顶点v在遍历树中的优先级数
   virtual int& priority(int)=0;
   //边：返里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
   //边总数
   int e;
   //边（v,u）是否存在
   virtual bool exists(int, int)=0;
   //在顶点v与u之间插入权重为w的边e
   virtual void insert(Te const&, int, int, int)=0;
   //删除顶点v与u之间的边e，返回该边信息
   virtual Te remove(int, int) = 0;
   //边（v,u）的状态
   virtual EStatus& status(int, int) = 0;
   //边（v,u）的数据（该边的确存在）
   virtual Te& edge(int, int) = 0;
   //边（v,u）的权重
   virtual int& weight(int, int) = 0;
   //算法
   //广度优先搜索算法
   void bfs(int);
   //深度优先搜索算法
   void dfs(int);
   //基于DFS的双连通分量分解算法
   void bcc(int);
   //基于DFS的拓扑排序算法
   stack<Tv>* tSort(int);
   //最小支撑数Prim算法
   void prim(int);
   //最短路径Dijkstra算法
   void dijkstra(int);
   //优先级搜索框架
   template <typename PU> void pfs(int, PU);
};

//顶点
template <typename Tv> 
struct Vertex 
{
  public:
    //数据
    Tv data; 
    //出入度数
    int inDegree;
    int outDegree; 
    //状态
    VStatus status;
    //时间标签
    int dTime;
    int fTime;
    //在遍历树中的父节点
    int parent;
    //在遍历树中的优先级数
    int priority;
    //构造新顶点
    Vertex(Tv const& d=(Tv)0):
    //暂不考虑权重溢出
    data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
    dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
 };

//边
template <typename Te> 
struct Edge 
{
  public:
    //数据
    Te data;
    //权重
    int weight; 
    //状态
    EStatus status;
    //构造新边
    Edge(Te const& d, int w):
    data(d),weight(w),status(UNDETERMINED){}
 };

 //基于向量，以邻接矩阵形式实现的图
 template <typename Tv, typename Te> //顶点类型、边类型
 class GraphMatrix:public Graph<Tv, Te>
 {
   private:
     //顶点集（向量）
     vector<Vertex<Tv>> V;
     //边集（邻接矩阵）
     vector<vector<Edge<Te>*>> E;
   public:
     //构造
     GraphMatrix() {this->n=0;this->e=0;}
     //析构
     ~GraphMatrix() 
     {
       //所有动态创建的边记录，逐条删除
       for (int j=0;j<this->n;j++)
         for (int k=0;k<this->n;k++)
           delete E[j][k];
     }
     //顶点的基本操作：查询第i个顶点(0<=i<n)
     //数据
     virtual Tv& vertex(int i) { return V[i].data;}
     //入度
     virtual int inDegree(int i) { return V[i].inDegree;}
     //出度
     virtual int outDegree(int i) { return V[i].outDegree;}
     //首个邻接顶点
     virtual int firstNbr(int i) { return nextNbr(i,this->n);}
     //相对于顶点j的下一邻接顶点
     virtual int nextNbr(int i,int j)
     {
       //逆向线性试探（改用邻接表可提高效率）
       while ((-1<j)&&(!exists(i,--j))); 
       return j; 
     }
     //状态
     virtual VStatus& status(int i) {return V[i].status;}
     //时间标签dTime
     virtual int& dTime(int i) {return V[i].dTime;}
     //时间标签fTime
     virtual int& fTime(int i) {return V[i].fTime;}
     //在遍历树中的父亲
     virtual int& parent(int i) {return V[i].parent;}
     //在遍历树中的优先级数
     virtual int& priority(int i) {return V[i].priority;}
    
     //顶点的动态操作
     //插入顶点，返回编号
     virtual int insert(Tv const& vertex) 
     {
       for(int j=0;j<this->n;j++) 
         E[j].insert(NULL); 
       //各顶点预留一条潜在的关联边
       this->n++;
       //创建新顶点对应的边向量
       E.insert(vector<Edge<Te>*>(this->n,this->n,(Edge<Te>*)NULL));
       //顶点向量增加一个顶点
       return V.insert(Vertex<Tv>(vertex));
     }
     //删除第i个顶点及其关联边(0<=i<n)
     virtual Tv remove(int i) 
     {
       //所有入边
       for (int j=0;j<this->n;j++)
         if (exists(i,j)) 
         { 
            //逐条删除
            delete E[i][j];
            V[j].inDegree--; 
         }
       //删除第i行
       E.remove(i); 
       this->n--;
       //所有出边
       for (int j=0;j<this->n;j++)
         if (exists(j, i)) 
         { 
            //逐条删除
            delete E[j].remove(i);
            V[j].outDegree--; 
         }
       //删除顶点i
       Tv vBak=vertex(i);
       V.remove(i); //初除顶点i
       //返回被删除顶点的信息
       return vBak;
     }
     //边的确认操作
     //确认边(i,j)是否存在
     virtual bool exists(int i,int j)
     { 
        return (0<=i)&&(i<this->n)&&(0<=j)&&(j<this->n)&&E[i][j]!=NULL;
     }
     //边的基本操作:查询顶点i与j之间的联边
     //边(i,j)的状态
     virtual EStatus& status(int i,int j) { return E[i][j]->status;}
     //边(i,j)的数据
     virtual Te& edge(int i,int j) { return E[i][j]->data;}
     //边(i,j)的权重
     virtual int& weight(int i,int j) { return E[i][j]->weight;}
     //边的动态操作
     //插入权重为w的边e=(i,j)
     virtual void insert(Te const& edge,int w,int i,int j) 
     {
       //确保该边不存在
       if(exists(i,j)) 
         return;
       //创建新边
       E[i][j]=new Edge<Te>(edge,w);
       //更新边计数与关联顶点的度数
       this->e++; 
       V[i].outDegree++; 
       V[j].inDegree++;
     }
     //删除顶点i和j之间的联边(exists(i,j))
     virtual Te remove(int i, int j)
     {
       //备份后删除边记录
       Te eBak=edge(i,j);
       delete E[i][j];
       E[i][j]=NULL;
       //更新边计数与关联顶点的度数
       this->e--;
       V[i].outDegree--;
       V[j].inDegree--;
       //返回被删除边的信息
       return eBak;
     }
};
#pragma endregion

//功能函数实现区
#pragma region 
void Get_data()
{
   ifstream infile("data.csv",ios::in);
   if(!infile)
   {
     cout<<"打开文件失败!"<<endl;
     exit(1);
   }
   int i=1;
   string line;
   string field;
   //读取标题行
   std::getline(infile,line);
   //读取数据
   while(std::getline(infile,line))
   {
      istringstream sin(line);
      //filght_id
      getline(sin,field,',');
      airdata[i].flight_id=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //departure date
      getline(sin,field,',');
      airdata[i].departure_date.time_data=field;
      airdata[i].departure_date.get_time();
      //cout<<field<<" ";
      //intl/dome
      getline(sin,field,',');
      //filght number
      getline(sin,field,',');
      airdata[i].flight_number=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //departure airport
      getline(sin,field,',');
      airdata[i].departure_airport=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //arrival airport
      getline(sin,field,',');
      airdata[i].arrival_airport=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //departure time
      getline(sin,field,',');
      airdata[i].departure_time.time_data=field;
      airdata[i].departure_time.get_time();
      //cout<<field<<" ";
      //arrival time
      getline(sin,field,',');
      airdata[i].arrival_time.time_data=field;
      airdata[i].arrival_time.get_time();
      //cout<<field<<" ";
      //airplane id
      getline(sin,field,',');
      airdata[i].airplane_id=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //airplane model
      getline(sin,field,',');
      airdata[i].airplane_model=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<" ";
      //air fares
      getline(sin,field);
      airdata[i].air_fares=atoi(field.c_str());
      //cout<<atoi(field.c_str())<<endl;
      
      i++;
   }
   total_num=i-1;
}
void First_printf()
{
   bool is_end=true;
   int temp_action;
   
   while(is_end)
   {
     cout<<"**********欢迎使用航班查询系统**********"<<endl;
     cout<<"*         请选择您想使用的功能         *"<<endl;
     cout<<"*           1.机场遍历查询             *"<<endl;
     cout<<"*           2.机场连通性查询           *"<<endl;
     cout<<"*           3.机场航线数目查询         *"<<endl;
     cout<<"*           4.航线最短时间查询         *"<<endl;
     cout<<"*           5.特定要求航线查询         *"<<endl;
     cout<<"*           6.最低航费航线查询         *"<<endl;
     cout<<"*           7.特殊中转航线查询         *"<<endl;
     cout<<"*           8.特殊航费航线查询         *"<<endl;
     cout<<"*           9.开发者选项               *"<<endl;
     cout<<"*           10.退出                    *"<<endl;
     cout<<"****************************************"<<endl;
     cout<<"请输入您的选择:"<<endl;
     cin>>temp_action;

     switch(temp_action)
     {
       case 1:
         cout<<"-----------------------------------------"<<endl;
         Func_1();
         break;
       case 2:
         cout<<"-----------------------------------------"<<endl;
         Func_2();
         break;
       case 3:
         cout<<"-----------------------------------------"<<endl;
         Func_3();
         break;
       case 4:
         cout<<"-----------------------------------------"<<endl;
         Func_4();
         break;
       case 5:
         cout<<"-----------------------------------------"<<endl;
         Func_5();
         break;
       case 6:
         cout<<"-----------------------------------------"<<endl;
         Func_6();
         break;
       case 7:
         cout<<"-----------------------------------------"<<endl;
         Func_7();
         break;
       case 8:
         cout<<"-----------------------------------------"<<endl;
         Func_8();
         break;
       case 9:
         Func_developer();
         break;
       case 10:
         cout<<"欢迎您再次使用航班查询系统!"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break;
       default:
         cout<<"警告:检测到非法输入!执行退出操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break;
     }
   }
}
void Func_1()
{
  bool is_end=true;
  int temp_action;
  int airport_id;

  while(is_end)
  {
    cout<<"**********欢迎使用机场遍历查询功能**********"<<endl;
    cout<<"*          请选择您需要的遍历方式          *"<<endl;
    cout<<"*              1.广度优先遍历              *"<<endl;
    cout<<"*              2.深度优先遍历              *"<<endl;
    cout<<"*              3.退出                      *"<<endl;
    cout<<"********************************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
    {
      case 1:
        cout<<"-----------------------------------------"<<endl;
        cout<<"请输入您想查询的机场ID:"<<endl;
        cin>>airport_id;
        if(airport_id>=1&&airport_id<80)
        {
          Func_1_1(airport_id);
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
        }
        else
        {
          cout<<"警告:检测到非法输入!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
        }
        break;
      case 2:
        cout<<"-----------------------------------------"<<endl;
        cout<<"请输入您想查询的机场ID:"<<endl;
        cin>>airport_id;
        if(airport_id>=1&&airport_id<80)
        {
          Func_1_2(airport_id);
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
        }
        else
        {
          cout<<"警告:检测到非法输入!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
        }
        break;
      case 3:
        cout<<"欢迎您再次使用机场遍历功能!"<<endl;
        cout<<"-----------------------------------------"<<endl;
        is_end=false;
        break;
      default:
        cout<<"警告:检测到非法输入!执行退出操作"<<endl;
        cout<<"-----------------------------------------"<<endl;
        is_end=false;
        break;
    }
  }
}
//构造邻接表
void Make_chart()
{
  int i,j;
  for(i=1;i<80;i++)
    dian[i].num_port=i;
  for(i=1;i<=total_num;i++)
  {
    //确定当前机场数据范围
    j=i;
    while(airdata[i].departure_airport==airdata[++j].departure_airport&&j<total_num-1);
    //确定有几个机场会到达
    int temp_check_port[80]={0};
    for(int a=i;a<j;a++)
      temp_check_port[airdata[a].arrival_airport]=1;
    //构造当前机场的邻接表
    bool time=true;
    for(int a=1;a<80;a++)
    {
       if(a!=airdata[i].departure_airport&&temp_check_port[a]==1)
       {
         Bian* temp=new Bian;
         temp->end_dian=a;
         if(time)
         {
           dian[airdata[i].departure_airport].next_bian=temp;
           time=false;
         }
         else
         {
            Bian* check=dian[airdata[i].departure_airport].next_bian;
            for(;check->next_bian!=NULL;check=check->next_bian);
            check->next_bian=temp;
         }
       }
    }
    i=j-1;
  }
}
//释放邻接表
void Delete_chart()
{
   for(int i=1;i<80;i++)
   {
     Bian* temp_1=dian[i].next_bian;
     Bian* temp_2;
     while(temp_1)
     {
       temp_2=temp_1->next_bian;
       delete temp_1;
       temp_1=temp_2;
     }
   }
}
void Func_1_1(int airport_id)
{
   Make_chart();
   //能否到达
   int temp_arrport[80]={0};
   //避免循环
   int temp_port_check[80]={0};
   queue<int> temp_port;
   Bian* temp=dian[airport_id].next_bian;
   temp_port.push(temp->end_dian);
   while(!temp_port.empty())
   {
     //如果当前机场没有搜索过
     if(temp_port_check[temp->end_dian]==0)
     {
       int this_change=temp->end_dian;
       temp_arrport[this_change]=1;
       temp_port_check[this_change]=1;
       //这个机场可以到达的目的地进队
       while(temp)
       {
         temp_arrport[temp->end_dian]=1;
         if(!temp_port_check[temp->end_dian])
           temp_port.push(temp->end_dian);
         temp=temp->next_bian;
       }
      }
      //队首元素出队，搜索下一个机场
      temp=dian[temp_port.front()].next_bian;
      temp_port.pop();
   }
   cout<<"当前"<<airport_id<<"机场可以达到的机场为:"<<endl;
   for(int i=0;i<80;i++)
   {
     if(temp_arrport[i]==1)
       cout<<i<<" ";
   }
   cout<<endl;
   Delete_chart();
}
void Func_1_2(int airport_id)
{
   Make_chart();
   //能否到达
   int temp_arrport[80];
   //避免循环
   int temp_port_check[80];
   Func_1_2_dfs(temp_arrport,temp_port_check,dian[airport_id].num_port);
   cout<<"当前"<<airport_id<<"机场可以达到的机场为:"<<endl;
   for(int i=0;i<80;i++)
   {
     if(temp_arrport[i]==1)
       cout<<i<<" ";
   }
   cout<<endl;
   Delete_chart();
}
void Func_1_2_dfs(int* temp_arrport,int* temp_port_check,int now)
{
   if(*(temp_port_check+now)==1)
     return;
   Bian* temp=dian[now].next_bian;
   *(temp_port_check+now)=1;
   while(temp)
   {
     *(temp_arrport+temp->end_dian)=1;
     Func_1_2_dfs(temp_arrport,temp_port_check,temp->end_dian);
     temp=temp->next_bian;
   }
}
void Func_2()
{
    bool is_end=true;
    int temp_action;
    int round_times;
    
    while(is_end)
    {
      cout<<"********欢迎使用机场连通性查询功能*******"<<endl;
      cout<<"*              1.查询                   *"<<endl;
      cout<<"*              2.退出                   *"<<endl;
      cout<<"*****************************************"<<endl;
      cout<<"请输入您的选择:"<<endl;
      cin>>temp_action;
      
      switch(temp_action)
      {
        case 1:
          cout<<"请输入您想查询的中转次数:"<<endl;
          cin>>round_times;
          if(round_times>=0)
          {
            cout<<"-----------------------------------------"<<endl;
            Func_2_1(round_times);
            cout<<"提示:查询成功!执行重置操作"<<endl;
            cout<<"-----------------------------------------"<<endl;
            break;
          }
          else 
          {
            cout<<"警告:检测到非法输入!执行退出操作"<<endl;
            cout<<"-----------------------------------------"<<endl;
            is_end=false;
            break;
          }
        case 2:
          cout<<"欢迎您再次使用机场连通性查询功能!"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
        default:
          cout<<"警告:检测到非法输入!执行退出操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
      }
    }
}
void Func_2_1(int round_times)
{
   Make_chart();
   int temp_time=0;
   int able_sirport[80][80]={0};
   int temp_check_1[80];
   int temp_check_2[80];
   for(int i=1;i<80;i++)
   {
     temp_time=0;
     memset(temp_check_1,0,sizeof(temp_check_1));
     temp_check_1[i]=1;
     while(temp_time<=round_times)
     {
       memset(temp_check_2,0,sizeof(temp_check_2));
       for(int j=1;j<80;j++)
       {
        if(temp_check_1[j]==1)
         {
           temp_check_2[j]=1;
           Bian* temp=dian[j].next_bian;
           while (temp)
           {
             temp_check_2[temp->end_dian]=1;
             temp=temp->next_bian;
           }
         }
       }
       for(int j=1;j<80;j++)
         temp_check_1[j]=temp_check_2[j];
       temp_time++;
     }
     for(int j=1;j<80;j++)
       able_sirport[i][j]=temp_check_1[j];
   }
   for(int i=1;i<80;i++)
   {
     for(int j=1;j<80;j++)
     {
       cout<<able_sirport[i][j]<<" ";
     }
     cout<<endl;
   }
   Delete_chart();
}
void Func_3()
{
    bool is_end=true;
    int temp_action;
    int air_id_1;
    int air_id_2;
    int round_times;
    int answer;
    
    while(is_end)
    {
      cout<<"*******欢迎使用机场航线数目查询功能******"<<endl;
      cout<<"*              1.查询                   *"<<endl;
      cout<<"*              2.退出                   *"<<endl;
      cout<<"*****************************************"<<endl;
      cout<<"请输入您的选择:"<<endl;
      cin>>temp_action;
      
      switch(temp_action)
      {
        case 1:
          cout<<"请输入您想查询的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您想查询的第二个机场ID:"<<endl;
          cin>>air_id_2;
          cout<<"请输入您想查询的中转次数:"<<endl;
          cin>>round_times;
          if(round_times>=0&&air_id_1>0&&air_id_1<80&&air_id_2>0&&air_id_2<80)
          {
            cout<<"-----------------------------------------"<<endl;
            answer=Func_3_1(air_id_1,air_id_2,round_times);
            cout<<"第"<<air_id_1<<"号机场与第"<<air_id_2<<"号机场在中转次数为"
            <<round_times<<"的条件下有"<<answer<<"条航线"<<endl;
            cout<<"提示:查询成功!执行重置操作"<<endl;
            cout<<"-----------------------------------------"<<endl;
            break;
          }
          else 
          {
            cout<<"警告:检测到非法输入!执行退出操作"<<endl;
            cout<<"-----------------------------------------"<<endl;
            is_end=false;
            break;
          }
        case 2:
          cout<<"欢迎您再次使用机场航线数目查询功能!"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
        default:
          cout<<"警告:检测到非法输入!执行退出操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
      }
    }
}
int Func_3_1(int air_port_1,int air_port_2,int round_times)
{
   int final_answer=0;
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_port_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_port_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
      //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(temp.weight>round_times)
           continue;
        if(temp.end_airport==air_port_2)
         {
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time)
           {
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
      
    }
   return final_answer;
}
void Func_4()
{
  bool is_end=true;
  int temp_action;
  int air_id_1;
  int air_id_2;
  int answer;

  while(is_end)
  {
    cout<<"*******欢迎使用航线最短时间查询功能******"<<endl;
    cout<<"*              1.查询                   *"<<endl;
    cout<<"*              2.退出                   *"<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
      {
        case 1:
          cout<<"请输入您想查询的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您想查询的第二个机场ID:"<<endl;
          cin>>air_id_2;
          if(air_id_1>0&&air_id_1<80&&air_id_2>0&&air_id_2<80)
          {
            cout<<"-----------------------------------------"<<endl;
            if(Judge_connect(air_id_1,air_id_2))
            {
              answer=Func_4_1(air_id_1,air_id_2);
              cout<<"从第"<<air_id_1<<"号机场到第"<<air_id_2<<"号机场最短需要"<<answer<<"分钟。"<<endl;
              cout<<"提示:查询成功!执行重置操作"<<endl;
              cout<<"-----------------------------------------"<<endl;
              break;
            }
            else 
            {
              cout<<"提示:第"<<air_id_1<<"号机场到第"<<air_id_2<<"号机场无合法航线,执行重置操作"<<endl;
              cout<<"-----------------------------------------"<<endl;
              break;
            }
          }
          else 
          {
            cout<<"警告:检测到非法输入!执行退出操作"<<endl;
            cout<<"-----------------------------------------"<<endl;
            is_end=false;
            break;
          }
        case 2:
          cout<<"欢迎您再次使用航线最短时间查询功能!"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
        default:
          cout<<"警告:检测到非法输入!执行退出操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
      }
  }
}
bool Judge_connect(int air_id_1,int air_id_2)
{
   bool answer=false;
   Make_chart();
   //能否到达
   int temp_arrport[80];
   //避免循环
   int temp_port_check[80];
   Func_1_2_dfs(temp_arrport,temp_port_check,dian[air_id_1].num_port);
   if(temp_arrport[air_id_2]==1)
     answer=true;
   Delete_chart();
   return answer;
}
int Func_4_1(int air_id_1,int air_id_2)
{
   int final_answer=INT_MAX;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短时间
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     memset(have_find,0,sizeof(have_find));
     for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
     priority_queue<qnode> que;
     qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport);
     Time_1 start_time=airdata[i].departure_time;
     have_find[air_id_1]=1;
     distance[air_id_1]=0;
     distance[airdata[i].arrival_airport]=temp.now_time;
     que.push(temp);
     while(!que.empty())
     {
       qnode temp=que.top(); que.pop();
       //排除已检查过的机场
       if(have_find[temp.end_airport]==1)
         continue;
       have_find[temp.end_airport]=1;
       //从该机场的航线里更新下一步机场
         //确认该机场信息范围
       int start_add;
       int end_add=total_num;
       for(int a=0;a<total_num;a++)
       {
         if(airdata[a].departure_airport==temp.end_airport)
         {
           for(int b=a;b<total_num;b++)
           {
             if(airdata[b].departure_airport!=temp.end_airport)
             {
               end_add=b;
               break;
             }
           }
           start_add=a;
           break;
         }
       } 
         //开始计算与入队操作
       for(int j=start_add;j<end_add;j++)
       {
         if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>(airdata[j].arrival_time-start_time))
         {
           distance[airdata[j].arrival_airport]=(airdata[j].arrival_time-start_time);
           que.push(qnode(distance[airdata[j].arrival_airport],airdata[j].arrival_airport));
         }
       }
     }
     if(final_answer>distance[air_id_2])
     {
       final_answer=distance[air_id_2];
     }
   }
   return final_answer;
}
void Func_5()
{
  bool is_end=true;
  int temp_action;
  int temp_answer;
  int air_id_1;       //机场编号1
  int air_id_2;       //机场编号2
  int round_time;     //中转次数
  Time_1 time1;       //时段1
  Time_1 time2;       //时段2
  int airplane_mod;   //机型

  while(is_end)
  {
    cout<<"********欢迎使用特定要求航线查询功能*******"<<endl;
    cout<<"*              请选择您的要求             *"<<endl;
    cout<<"*          1.起飞时段要求(全部航线)       *"<<endl;
    cout<<"*          2.起飞时段要求(最优航线)       *"<<endl;
    cout<<"*          3.降落时段要求(全部航线)       *"<<endl;
    cout<<"*          4.降落时段要求(最优航线)       *"<<endl;
    cout<<"*          5.机型要求(所有航线)           *"<<endl;
    cout<<"*          6.机型要求(最优航线)           *"<<endl;
    cout<<"*          7.退出                         *"<<endl;
    cout<<"*******************************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
    {
       case 1:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的起飞时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的起飞时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_1_2(air_id_1,air_id_2,round_time,time1,time2);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 2:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的起飞时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的起飞时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_1(air_id_1,air_id_2,round_time,time1,time2);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;;
       case 3:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的降落时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的降落时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_2_2(air_id_1,air_id_2,round_time,time1,time2);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 4:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的降落时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的降落时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_2(air_id_1,air_id_2,round_time,time1,time2);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 5:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的机型:"<<endl;
         cin>>airplane_mod;
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_3(air_id_1,air_id_2,round_time,airplane_mod);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 6:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的中转次数上限:"<<endl;
         cin>>round_time;
         cout<<"请输入您的机型:"<<endl;
         cin>>airplane_mod;
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_5_3_2(air_id_1,air_id_2,round_time,airplane_mod);
         cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
         for(int i=0;i<temp_answer;i++)
           cout<<air_answer[i]<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 7:
        cout<<"欢迎您再次使用特定要求航线查询功能!"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break;
       default:
        cout<<"警告:检测到非法输入!执行退出操作"<<endl;
        cout<<"-----------------------------------------"<<endl;
        is_end=false;
        break;
    }
  }
}
int Func_5_1(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2)
{
   int final_answer=0;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].departure_time<time2&&time1<airdata[i].departure_time)
     {
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
         distance[j]=INT_MAX;
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(have_find[temp.end_airport]==1)
           continue;
         if(temp.weight>round_time)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>temp.weight+1)
           {
             distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
      }
    }
   return final_answer;
}
int Func_5_1_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2)
{
   int final_answer=0;
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].departure_time<time2&&time1<airdata[i].departure_time)
     {
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(temp.weight>round_time)
           continue;
        if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time)
           {
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
      }
    }
   return final_answer;
}
int Func_5_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2)
{
   int final_answer=0;
   int time_min;
   int time_max;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     time_max=time2-airdata[i].departure_time;
     time_min=time1-airdata[i].departure_time;
     memset(have_find,0,sizeof(have_find));
     for(int j=0;j<80;j++)
     distance[j]=INT_MAX;
     //初始状态进队
     priority_queue<qnode> que;
     qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
     Time_1 start_time=airdata[i].departure_time;
     have_find[air_id_1]=1;
     distance[air_id_1]=0;
     distance[airdata[i].arrival_airport]=temp.weight;
     que.push(temp);
     while(!que.empty())
     {
       qnode temp=que.top(); que.pop();
       //排除已检查过的机场
       if(have_find[temp.end_airport]==1)
         continue;
       if(temp.weight>round_time)
         continue;
       have_find[temp.end_airport]=1;
       if(temp.end_airport==air_id_2)
       {
         if(temp.now_time>=time_min&&temp.now_time<=time_max)
         {
          air_answer[final_answer]=temp.across_air;
          final_answer++;
          continue;
         }
       }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>temp.weight+1)
           {
             distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
      
    }
   return final_answer;
}
int Func_5_2_2(int air_id_1,int air_id_2,int round_time,Time_1 time1,Time_1 time2)
{
   int final_answer=0;
   int time_min;
   int time_max;
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     time_max=time2-airdata[i].departure_time;
     time_min=time1-airdata[i].departure_time;
     //初始状态进队
     priority_queue<qnode> que;
     qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
     Time_1 start_time=airdata[i].departure_time;
     que.push(temp);
     while(!que.empty())
     {
       qnode temp=que.top(); que.pop();
       //排除已检查过的机场
       if(temp.weight>round_time)
         continue;
       if(temp.end_airport==air_id_2)
       {
         if(temp.now_time>=time_min&&temp.now_time<=time_max)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
       }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time)
           {
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
      
    }
   return final_answer;
}
int Func_5_3(int air_id_1,int air_id_2,int round_time,int airplane_mod)
{
   int final_answer=0;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].airplane_model==airplane_mod)
     {
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(have_find[temp.end_airport]==1)
           continue;
         if(temp.weight>round_time)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>temp.weight+1&&
              airdata[j].airplane_model==airplane_mod)
           {
             distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
     }
   }
   return final_answer;
}
int Func_5_3_2(int air_id_1,int air_id_2,int round_time,int airplane_mod)
{
   int final_answer=0;
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].airplane_model==airplane_mod)
     {
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(temp.weight>round_time)
           continue;
         if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&
              airdata[j].airplane_model==airplane_mod)
           {
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
     }
   }
   return final_answer;
}
void Func_6()
{
  bool is_end=true;
  int temp_action;
  int temp_answer;
  int air_id_1;       //机场编号1
  int air_id_2;       //机场编号2
  Time_1 time1;       //时段1
  Time_1 time2;       //时段2
  int airplane_mod;   //机型

  while(is_end)
  {
    cout<<"********欢迎使用最低航费航线查询功能*******"<<endl;
    cout<<"*              请选择您的要求             *"<<endl;
    cout<<"*              1.起飞时段要求             *"<<endl;
    cout<<"*              2.降落时段要求             *"<<endl;
    cout<<"*              3.机型要求                 *"<<endl;
    cout<<"*              4.退出                     *"<<endl;
    cout<<"*******************************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
    {
       case 1:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的起飞时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的起飞时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_6_1(air_id_1,air_id_2,time1,time2);
         if(temp_answer==INT_MAX)
         {
           cout<<"提示:无符合条件的航线!"<<endl;
         }
         else 
         {
           cout<<"已为您查询到最低航费航线!共花费"<<temp_answer<<"航费."<<endl;
           cout<<air_answer[0]<<endl;
         }
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 2:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的降落时段上限:(例:5/5/2017 12:20)"<<endl;
         cin.clear();
         cin.ignore();
         getline(cin,time1.time_data);
         time1.get_time();
         cout<<"请输入您的降落时段下限:(例:5/5/2017 12:20)"<<endl;
         getline(cin,time2.time_data);
         time2.get_time();
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_6_2(air_id_1,air_id_2,time1,time2);
         if(temp_answer==INT_MAX)
         {
           cout<<"提示:无符合条件的航线!"<<endl;
         }
         else 
         {
           cout<<"已为您查询到最低航费航线!共花费"<<temp_answer<<"航费."<<endl;
           cout<<air_answer[0]<<endl;
         }
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 3:
         cout<<"-----------------------------------------"<<endl;
         cout<<"请输入您的第一个机场ID:"<<endl;
         cin>>air_id_1;
         cout<<"请输入您的第二个机场ID:"<<endl;
         cin>>air_id_2;
         cout<<"请输入您的机型:"<<endl;
         cin>>airplane_mod;
         for(int i=0;i<5000;i++)
           air_answer[i]="";
         temp_answer=Func_6_3(air_id_1,air_id_2,airplane_mod);
         if(temp_answer==INT_MAX)
         {
           cout<<"提示:无符合条件的航线!"<<endl;
         }
         else 
         {
           cout<<"已为您查询到最低航费航线!共花费"<<temp_answer<<"航费."<<endl;
           cout<<air_answer[0]<<endl;
         }
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 4:
         cout<<"欢迎您再次使用特定要求航线查询功能!"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break;
       default:
        cout<<"警告:检测到非法输入!执行退出操作"<<endl;
        cout<<"-----------------------------------------"<<endl;
        is_end=false;
        break;
    }
  }
}
int Func_6_1(int air_id_1,int air_id_2,Time_1 time1,Time_1 time2)
{
   int final_answer=INT_MAX;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短航费
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].departure_time<time2&&time1<airdata[i].departure_time)
     {
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
         distance[j]=INT_MAX;
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),airdata[i].air_fares);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(have_find[temp.end_airport]==1)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           if(final_answer>temp.weight)
           {
             final_answer=temp.weight;
             air_answer[0]=temp.across_air;
             continue;
           }
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>(temp.weight+airdata[j].air_fares))
           {
             distance[airdata[j].arrival_airport]=temp.weight+airdata[j].air_fares;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),distance[airdata[j].arrival_airport]));
           }
         }
       }
      }
    }
   return final_answer;
}
int Func_6_2(int air_id_1,int air_id_2,Time_1 time1,Time_1 time2)
{
  int final_answer=INT_MAX;
   int time_min;
   int time_max;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最低航费
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     time_max=time2-airdata[i].departure_time;
     time_min=time1-airdata[i].departure_time;
     memset(have_find,0,sizeof(have_find));
     for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
     //初始状态进队
     priority_queue<qnode> que;
     qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),airdata[i].air_fares);
     Time_1 start_time=airdata[i].departure_time;
     have_find[air_id_1]=1;
     distance[air_id_1]=0;
     distance[airdata[i].arrival_airport]=temp.weight;
     que.push(temp);
     while(!que.empty())
     {
       qnode temp=que.top(); que.pop();
       //排除已检查过的机场
       if(have_find[temp.end_airport]==1)
         continue;
       have_find[temp.end_airport]=1;
       if(temp.end_airport==air_id_2)
       {
         if(temp.now_time>=time_min&&temp.now_time<=time_max)
         {
           if(final_answer>temp.weight)
           {
             final_answer=temp.weight;
             air_answer[0]=temp.across_air;
             continue;
           }
         }
       }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>temp.weight+airdata[j].air_fares)
           {
             distance[airdata[j].arrival_airport]=temp.weight+airdata[j].air_fares;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+airdata[j].air_fares));
           }
         }
       }
    }
   return final_answer;
}
int Func_6_3(int air_id_1,int air_id_2,int airplane_mod)
{
   int final_answer=INT_MAX;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     if(airdata[i].airplane_model==airplane_mod)
     {
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
         distance[j]=INT_MAX;
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),airdata[i].air_fares);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         if(have_find[temp.end_airport]==1)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           if(final_answer>temp.weight)
           {
             final_answer=temp.weight;
             air_answer[0]=temp.across_air;
             continue;
           }
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time&&distance[airdata[j].arrival_airport]>temp.weight+airdata[j].air_fares&&
              airdata[j].airplane_model==airplane_mod)
           {
             distance[airdata[j].arrival_airport]=temp.weight+airdata[j].air_fares;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+airdata[j].air_fares));
           }
         }
       }
     }
   }
   return final_answer;
}
void Func_7()
{
   bool is_end=true;
   int temp_action;
   int air_id_1;
   int air_id_2;
   int round_time;
   int temp_time;
   int temp_answer;

   while(is_end)
   {
      cout<<"*******欢迎使用特殊中转航线查询功能******"<<endl;
      cout<<"*          1.查询每次中转时间           *"<<endl;
      cout<<"*          2.查询总计中转时间           *"<<endl;
      cout<<"*          3.退出                       *"<<endl;
      cout<<"*****************************************"<<endl;
      cout<<"请输入您的选择:"<<endl;
      cin>>temp_action;

      switch(temp_action)
      {
        case 1:
          cout<<"-----------------------------------------"<<endl;
          cout<<"请输入您的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您的第二个机场ID:"<<endl;
          cin>>air_id_2;
          cout<<"请输入您的中转次数上限:"<<endl;
          cin>>round_time;
          cout<<"请输入您的中转时间上限:"<<endl;
          cin>>temp_time;
          for(int i=0;i<5000;i++)
            air_answer[i]="";
          temp_answer=Func_7_1(air_id_1,air_id_2,round_time,temp_time);
          cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
          for(int i=0;i<temp_answer;i++)
            cout<<air_answer[i]<<endl;
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          break;
        case 2:
          cout<<"-----------------------------------------"<<endl;
          cout<<"请输入您的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您的第二个机场ID:"<<endl;
          cin>>air_id_2;
          cout<<"请输入您的中转次数上限:"<<endl;
          cin>>round_time;
          cout<<"请输入您的中转时间上限:"<<endl;
          cin>>temp_time;
          for(int i=0;i<5000;i++)
            air_answer[i]="";
          temp_answer=Func_7_2(air_id_1,air_id_2,round_time,temp_time);
          cout<<"为您查询到"<<temp_answer<<"条符合条件的航线"<<endl;
          for(int i=0;i<temp_answer;i++)
            cout<<air_answer[i]<<endl;
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          break;
        case 3:
          cout<<"欢迎您再次使用特殊中转航线查询功能!"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
        default:
          cout<<"警告:检测到非法输入!执行退出操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
      }
   }
}
int Func_7_1(int air_id_1,int air_id_2,int round_time,int temp_time)
{
   int final_answer=0;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
       //初始状态进队
       queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.front(); que.pop();
         //排除已检查过的机场
         /*if(have_find[temp.end_airport]==1)
           continue;*/
         if(temp.weight>round_time)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time/*&&distance[airdata[j].arrival_airport]>temp.weight+1*/
           &&((airdata[j].departure_time-start_time)<=(temp.now_time+temp_time)))
           {
             //distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1));
           }
         }
       }
   }
   return final_answer;
}
int Func_7_2(int air_id_1,int air_id_2,int round_time,int temp_time)
{
   int final_answer=0;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
       //初始状态进队
       queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),0,0);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.front(); que.pop();
         //排除已检查过的机场
         /*if(have_find[temp.end_airport]==1)
           continue;*/
         if(temp.total_time>temp_time)
           continue;
         if(temp.weight>round_time)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           air_answer[final_answer]=temp.across_air;
           final_answer++;
           continue;
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time/*&&distance[airdata[j].arrival_airport]>temp.weight+1*/
           &&((airdata[j].departure_time-start_time)<=(temp.now_time+temp_time)))
           {
             //distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+1,
               temp.total_time+(airdata[j].departure_time-start_time)-temp.now_time));
           }
         }
       }
   }
   return final_answer;
}
void Func_8()
{
   bool is_end=true;
   int temp_action;
   int air_id_1;
   int air_id_2;
   int temp_time;
   int temp_answer;

   while(is_end)
   {
      cout<<"*******欢迎使用特殊航费航线查询功能******"<<endl;
      cout<<"*          1.查询每次中转时间           *"<<endl;
      cout<<"*          2.查询总计中转时间           *"<<endl;
      cout<<"*          3.退出                       *"<<endl;
      cout<<"*****************************************"<<endl;
      cout<<"请输入您的选择:"<<endl;
      cin>>temp_action;

      switch(temp_action)
      {
        case 1:
          cout<<"-----------------------------------------"<<endl;
          cout<<"请输入您的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您的第二个机场ID:"<<endl;
          cin>>air_id_2;
          cout<<"请输入您的中转时间上限:"<<endl;
          cin>>temp_time;
          for(int i=0;i<5000;i++)
            air_answer[i]="";
          temp_answer=Func_8_1(air_id_1,air_id_2,temp_time);
          if(temp_answer==INT_MAX)
          {
            cout<<"提示:无符合条件的航线!"<<endl;
          }
          else 
          {
            cout<<"已为您查询到最低航费航线!共花费"<<temp_answer<<"航费."<<endl;
            cout<<air_answer[0]<<endl;
          }
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          break;
        case 2:
          cout<<"-----------------------------------------"<<endl;
          cout<<"请输入您的第一个机场ID:"<<endl;
          cin>>air_id_1;
          cout<<"请输入您的第二个机场ID:"<<endl;
          cin>>air_id_2;
          cout<<"请输入您的中转时间上限:"<<endl;
          cin>>temp_time;
          for(int i=0;i<5000;i++)
            air_answer[i]="";
          temp_answer=Func_8_2(air_id_1,air_id_2,temp_time);
          if(temp_answer==INT_MAX)
          {
            cout<<"提示:无符合条件的航线!"<<endl;
          }
          else 
          {
            cout<<"已为您查询到最低航费航线!共花费"<<temp_answer<<"航费."<<endl;
            cout<<air_answer[0]<<endl;
          }
          cout<<"提示:查询成功!执行重置操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          break;
        case 3:
          cout<<"欢迎您再次使用特殊航费航线查询功能!"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
        default:
          cout<<"警告:检测到非法输入!执行退出操作"<<endl;
          cout<<"-----------------------------------------"<<endl;
          is_end=false;
          break;
      }
   }
}
int Func_8_1(int air_id_1,int air_id_2,int temp_time)
{
   int final_answer=INT_MAX;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
       //初始状态进队
       queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),airdata[i].air_fares);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.front(); que.pop();
         //排除已检查过的机场
         /*if(have_find[temp.end_airport]==1)
           continue;*/
         if(temp.now_time>2000)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           if(final_answer>temp.weight)
           {
             final_answer=temp.weight;
             air_answer[0]=temp.across_air;
             continue;
           }
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time/*&&distance[airdata[j].arrival_airport]>temp.weight+1*/
           &&((airdata[j].departure_time-start_time)<=(temp.now_time+temp_time)))
           {
             //distance[airdata[j].arrival_airport]=temp.weight+1;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+airdata[j].air_fares));
           }
         }
       }
   }
   return final_answer;
}
int Func_8_2(int air_id_1,int air_id_2,int temp_time)
{
   int final_answer=INT_MAX;
   //已找到的机场
   int have_find[80]={0};
   //到出发站的最短中转次数
   int distance[80];
   //从出发站开始的所有的航线都遍历一遍
     //确定遍历的航线范围
   int start_number;
   int end_number=total_num;
   for(int i=0;i<total_num;i++)
   {
     if(airdata[i].departure_airport==air_id_1)
     {
       for(int j=i;j<total_num;j++)
       {
         if(airdata[j].departure_airport!=air_id_1)
         {
           end_number=j;
           break;
         }
       }
       start_number=i;
       break;
     }
   }
   //开始挨个遍历
   for(int i=start_number;i<end_number;i++)
   {
     
       memset(have_find,0,sizeof(have_find));
       for(int j=0;j<80;j++)
       distance[j]=INT_MAX;
       //初始状态进队
       priority_queue<qnode> que;
       qnode temp(airdata[i].arrival_time-airdata[i].departure_time,airdata[i].arrival_airport,to_string(airdata[i].flight_id),airdata[i].air_fares,0);
       Time_1 start_time=airdata[i].departure_time;
       have_find[air_id_1]=1;
       distance[air_id_1]=0;
       distance[airdata[i].arrival_airport]=temp.weight;
       que.push(temp);
       while(!que.empty())
       {
         qnode temp=que.top(); que.pop();
         //排除已检查过的机场
         /*if(have_find[temp.end_airport]==1)
           continue;*/
         if(temp.total_time>temp_time)
           continue;
         have_find[temp.end_airport]=1;
         if(temp.end_airport==air_id_2)
         {
           if(final_answer>temp.weight)
           {
             final_answer=temp.weight;
             air_answer[0]=temp.across_air;
             continue;
           }
         }
         //从该机场的航线里更新下一步机场
           //确认该机场信息范围
         int start_add;
         int end_add=total_num;
         for(int a=0;a<total_num;a++)
          {
           if(airdata[a].departure_airport==temp.end_airport)
           {
             for(int b=a;b<total_num;b++)
             {
               if(airdata[b].departure_airport!=temp.end_airport)
               {
                 end_add=b;
                 break;
               }
             }
             start_add=a;
             break;
           }
         } 
           //开始计算与入队操作
         for(int j=start_add;j<end_add;j++)
         {
           if((airdata[j].departure_time-start_time)>=temp.now_time/*&&distance[airdata[j].arrival_airport]>temp.weight+airdata[j].air_fares*/
           &&((airdata[j].departure_time-start_time)<=(temp.now_time+temp_time)))
           {
             distance[airdata[j].arrival_airport]=temp.weight+airdata[j].air_fares;
             que.push(qnode(airdata[j].arrival_time-start_time,airdata[j].arrival_airport,
               temp.across_air+" -> "+to_string(airdata[j].flight_id),temp.weight+airdata[j].air_fares,
                temp.total_time+(airdata[j].departure_time-start_time)-temp.now_time));
           }
         }
       }
   }
   return final_answer;
}
void Func_developer()
{
   bool is_end=true;
   int temp_action;
   while(is_end)
   {
     cout<<"******欢迎使用开发者选项功能******"<<endl;
     cout<<"*      请选择您想使用的功能      *"<<endl;
     cout<<"*        1.所有机场邻接表        *"<<endl;
     cout<<"*        2.所有机场遍历          *"<<endl;
     cout<<"*        3.所有机场最短时长      *"<<endl;
     cout<<"*        4.退出                  *"<<endl;
     cout<<"**********************************"<<endl;
     cout<<"请输入您的选择:"<<endl;
     cin>>temp_action;
     switch(temp_action)
     {
       case 1:
         cout<<"-----------------------------------------"<<endl;
         Make_chart();
         for(int i=1;i<80;i++)
         {
           cout<<i<<":";
           Bian* temp=dian[i].next_bian;
           while(temp)
           {
              cout<<temp->end_dian<<" ";
              temp=temp->next_bian;
           }
           cout<<endl;
         }
         Delete_chart();
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 2:
         cout<<"-----------------------------------------"<<endl;
         for(int i=1;i<80;i++)
           Func_1_2(i);
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 3:
         cout<<"-----------------------------------------"<<endl;
         for(int i=1;i<80;i++)
         {
           for(int j=1;j<80;j++)
           {
             cout<<Func_4_1(i,j)<<" ";
           }
           cout<<endl;
         }
         cout<<"提示:查询成功!执行重置操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         break;
       case 4: 
         cout<<"欢迎您再次使用开发者选项功能!"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break;
       default:
         cout<<"警告:检测到非法输入!执行退出操作"<<endl;
         cout<<"-----------------------------------------"<<endl;
         is_end=false;
         break; 
     }
   }
}
#pragma endregion

int main()
{
  Get_data();
  First_printf();
  return 0;
}
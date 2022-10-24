#include"vector_2.h"
#include<iostream>
#include<string.h>
#include<stack>
#include<cstdio>
#include<math.h>
using namespace std;

#pragma region 
//全局变量区
  //结构体区
  #pragma region 
struct Func{
    double coe;    //系数
    string var;    //自变量
    double index;  //指数
    string answer; //答案
};

struct SP_Func{
    string spe_func;  //函数主体  eg:3*x+1
    string fuc_name;  //函数名称  eg:f(x)
    string var_name;  //变量名称  eg:x
    string fuc_only;  //函数头    eg:f
}sp_fuc[100];

template<typename T>class Stack_2:public Vector_2<T>{
    public:
    void push(T const& e){ Vector_2<T>::insert(Vector_2<T>::size(),e);}
    T pop(){return remove(Vector_2<T>::size()-1);}
    T& top(){return (*this)[Vector_2<T>::size()-1];}
};
  #pragma endregion
  //变量区
  int fuc_num;   //函数数量

//全局函数区
  //声明区
  #pragma region 
  void First_show();
  void Qua_ope();
  void Qua_ope_1();
  void Qua_ope_2();
  void Qua_ope_3();
  void Mat_ope();
  void Mat_ope_1();
  void Mat_ope_2();
  void Mat_ope_3();
  void Mat_ope_4();
  void Mat_ope_5();
  void Mat_ope_6();
  void Fuc_ope();
  void Fuc_ope_1();
  void Fuc_ope_2();
  void Fuc_ope_3();
  void Fuc_ope_4();
  void Str_min(double* front,double* down,int hang,int where);
  void Make_mat(double make_mat[100][100],double* first,int hang);
  void Replace_all(string& str,const string& strfir,const string& strend);
  bool Modify_fuc(int now_num);
  int Get_ope_value(char a);
  double Get_deter(double *before,int where);
  double Max(double* first,int hang);
  double Make_qua_ope(string data);
  double Get_cal_num(double a,double b,char ope);
  double Get_num(string data,int i);
  string Make_integral_ope(string data);
  string Make_differential_ope(string data);
  #pragma endregion
  //使用指导
void First_show()
{
   cout<<"*********欢迎您使用计算器**********"<<endl;
   cout<<"*    本计算器可使用的功能如下     *"<<endl;
   cout<<"*        1.四则运算式计算         *"<<endl;
   cout<<"*        2.矩阵运算               *"<<endl;
   cout<<"*        3.函数计算               *"<<endl;
   cout<<"*        4.退出                   *"<<endl;
   cout<<"***********************************"<<endl;
   cout<<"请您选择您想进行的操作:";
}
#pragma endregion

//四则运算函数
#pragma region 
void Qua_ope()
{
   bool is_end=true;
   int temp_action;
   while(is_end)
   {
     cout<<"*******欢迎使用四则运算功能***********"<<endl;
     cout<<"*      请选择您想进行的操作          *"<<endl;
     cout<<"*          1.四则运算                *"<<endl;
     cout<<"*          2.积分运算                *"<<endl;
     cout<<"*          3.微分运算                *"<<endl;
     cout<<"*            4.退出                  *"<<endl;
     cout<<"**************************************"<<endl;
     cout<<"请您选择您想进行的操作:";
     cin>>temp_action;
   
     switch(temp_action)
     {
       case 1:
         Qua_ope_1();
         break;
       case 2:
         Qua_ope_2();
         break;
       case 3:
         Qua_ope_3();
         break;
       case 4:
         is_end=false;
         cout<<"欢迎您再次使用四则运算!"<<endl;
         break;
       default:
         is_end=false;
         cout<<"警告:检测到不合法输入,执行返回操作"<<endl;
         cout<<"欢迎您再次使用四则运算!"<<endl;
         break;
     }
   }

}

void Qua_ope_1()
{
   string data;
   double temp_answer;
   cout<<"请输入您的运算表达式:"<<endl;
   cin>>data;
   temp_answer=Make_qua_ope(data);
   cout<<"您输入的表达式的答案为:"<<temp_answer<<endl;

}

void Qua_ope_2()
{
   string data;
   string temp_answer;
   int time;
   cout<<"请输入您的积分运算表达式:(例:3*x^2+6*x+3)"<<endl;
   cin>>data;
   cout<<"请输入您想进行积分的阶数:"<<endl;
   cin>>time;
   for(int i=0;i<time;i++)
   {
     data=Make_integral_ope(data);
     temp_answer=data;
     if(temp_answer=="wrong")  break;
   }
   if(temp_answer=="wrong")
     cout<<"警告:检测到格式错误,执行返回操作"<<endl;
   else
     cout<<"您的积分表达式运算结果为:"<<temp_answer<<endl;
}

void Qua_ope_3()
{
   string data;
   string temp_answer;
   int time;
   cout<<"请输入您的微分运算表达式:(例:3*x^2+6*x+3)"<<endl;
   cin>>data;
   cout<<"请输入您想进行微分的阶数:"<<endl;
   cin>>time;
   for(int i=0;i<time;i++)
   {
     data=Make_differential_ope(data);
     temp_answer=data;
     if(temp_answer=="wrong")  break;
   }
   if(temp_answer=="wrong")
     cout<<"警告:检测到格式错误,执行返回操作"<<endl;
   else
     cout<<"您的微分表达式运算结果为:"<<temp_answer<<endl;
}

//积分表达式运算
string Make_integral_ope(string data)
{
   Func infor[100];
   char sym[100];   //记录正负号
   int cout=1;
   int now_cout=0;
   int start;
   int end;
   string temp_answer;
   string temp_check;
   bool is_vavild=true;       //是否合法
   bool is_have_coe=false;     //是否有系数
   //确定有几个小单位
   for(int i=0;i<data.length();i++)
   {
     if(data[i]=='+'||(data[i]=='-'&&data[i-1]!='('))  //与负数区分
     {
        if(data[i]=='+')
        {
          sym[cout]='+';
        }
        else 
        {
          sym[cout]='-';
        }
        cout++;
     }
   }
   //切割小单位
   start=-1;
   end=0;
   for(int i=0;i<data.length();i++)
   {
      //记录系数
      if(data[i]>='0'&&data[i]<='9')
      {
        infor[now_cout].coe=Get_num(data,i);
        for(;i<data.length();i++)
        {
          if(data[i]=='*')  break;
        }
        start=i;
        is_have_coe=true;
      }
      else if(data[i]=='('&&data[i+1]=='-')
      {
        i=i+2;
        infor[now_cout].coe=Get_num(data,i);
        infor[now_cout].coe*=-1;
        for(;i<data.length();i++)
        {
          if(data[i]=='*')  break;
        }
        start=i;
        is_have_coe=true;
      }
      for(;i<data.length();i++)
      {
        if(data[i]=='^'||data[i]=='+'||data[i]=='-')  break;
      }
      end=i;
      if(!is_have_coe)
      {
         infor[now_cout].coe=1;
      }
      //转移变量
      for(int j=start+1;j<end;j++)
        infor[now_cout].var+=data[j];
      //记录指数
      if(data[i]=='^'&&data[i+1]!='(')
      {
        infor[now_cout].index=Get_num(data,i+1);
        while(data[i]!='+'&&data[i]!='-'&&i<data.length())
        {
          i++;
        }
      }
      else if(data[i]=='^'&&data[i+1]=='('&&data[i+2]=='-')
      {
        i=i+2;
        infor[now_cout].index=Get_num(data,i+1);
        infor[now_cout].index*=-1;
        //跳过括号
        i++;
        while(data[i]!='+'&&data[i]!='-'&&i<data.length())
        {
          i++;
        }
      }
      else 
      {
        infor[now_cout].index=1;
      }
      start=i;
      is_have_coe=false;
      now_cout++;
   }
   //输入合法性检查
     //检查每一项的自变量是否相等
   temp_check=infor[0].var;
   for(int i=0;i<cout;i++)
   {
     if(infor[i].var.empty())
       continue;
     else if(temp_check!=infor[i].var)
     {
       is_vavild=false;break;
     }
   }
   if(!is_vavild)
   {
      string warning="wrong";
      return warning;
   }
   //进行积分操作
   for(int i=0;i<cout;i++)
   {
      double temp_coe;
      double temp_index;
      char temp_buf_1[20];
      char temp_buf_2[20];
      //如果不是常数项
      if(!infor[i].var.empty())
      {
        infor[i].index+=1;
        infor[i].coe/=infor[i].index;
        //将double转成字符串
        sprintf_s(temp_buf_1,"%.2lf",infor[i].coe);
        sprintf_s(temp_buf_2,"%.2lf",infor[i].index);
        //如果系数是正数
        if(infor[i].coe>0)
        {
          infor[i].answer=temp_buf_1;
        }
        //如果系数是负数
        else if(infor[i].coe<0)
        {
          infor[i].answer='(';
          infor[i].answer+=temp_buf_1;
          infor[i].answer+=')';
        }
        //如果指数是正数
        if(infor[i].index>0)
        {
          infor[i].answer+='*';
          infor[i].answer+=infor[i].var+'^'+temp_buf_2;
        }
        //如果指数为零，直接跳过
        //如果指数是负数
        else if(infor[i].index<0)
        {
          infor[i].answer+=infor[i].var+'^'+'(';
          infor[i].answer+=temp_buf_2;
          infor[i].answer+=')';
        }
        temp_answer+=infor[i].answer;
        if(i+1<cout)  temp_answer+=sym[i+1];
      }
      //处理常数项
      else 
      {
        //如果只有这一个常数项
        if(cout==1)
        {
          sprintf_s(temp_buf_1,"%.2lf",infor[i].coe);
          if(infor[i].coe>0) 
          {
            temp_answer+=temp_buf_1;
          }
          else if(infor[i].coe<0)
          {
            temp_answer+='(';
            temp_answer+=temp_buf_1;
            temp_answer+=')';
          }
          temp_answer+='*';
          temp_answer+='x';
        }
        //如果有其他项,找到变量
        else 
        {
           sprintf_s(temp_buf_1,"%.2lf",infor[i].coe);
           if(infor[i].coe>0)
           {
            for(int j=0;j<cout;j++)
            {
              if(j!=i&&!infor[j].var.empty())
              {
               temp_answer+=temp_buf_1;
               temp_answer+='*';
               temp_answer+=infor[j].var;
               break;
              }
            }
           }
           else if(infor[i].coe<0)
           {
             for(int j=0;j<cout;j++)
             {
               if(j!=i&&!infor[j].var.empty())
               {
                temp_answer+='(';
                temp_answer+=temp_buf_1;
                temp_answer+=')';
                temp_answer+='*';
                temp_answer+=infor[j].var;
                break;
               }
             }
           }  
        }
        if(i+1<cout)  temp_answer+=sym[i+1];
      }
   }
   return temp_answer;
}

//微分表达式运算
string Make_differential_ope(string data)
{
   Func infor[100];
   char sym[100];   //记录正负号
   int cout=1;
   int now_cout=0;
   int start;
   int end;
   string temp_answer;
   string temp_check;
   bool is_vavild=true;       //是否合法
   bool is_have_coe=false;     //是否有系数
   //确定有几个小单位
   for(int i=0;i<data.length();i++)
   {
     if(data[i]=='+'||(data[i]=='-'&&data[i-1]!='('))
     {
        if(data[i]=='+')
        {
          sym[cout]='+';
        }
        else 
        {
          sym[cout]='-';
        }
        cout++;
     }
   }
   //切割小单位
   start=-1;
   end=0;
   for(int i=0;i<data.length();i++)
   {
      //记录系数
      if(data[i]>='0'&&data[i]<='9')
      {
        infor[now_cout].coe=Get_num(data,i);
        for(;i<data.length();i++)
        {
          if(data[i]=='*')  break;
        }
        start=i;
        is_have_coe=true;
      }
      else if(data[i]=='('&&data[i+1]=='-')
      {
        i=i+2;
        infor[now_cout].coe=Get_num(data,i);
        infor[now_cout].coe*=-1;
        for(;i<data.length();i++)
        {
          if(data[i]=='*')  break;
        }
        start=i;
        is_have_coe=true;
      }
      for(;i<data.length();i++)
      {
        if(data[i]=='^'||data[i]=='+'||data[i]=='-')  break;
      }
      end=i;
      if(!is_have_coe)
      {
         infor[now_cout].coe=1;
      }
      //转移变量
      for(int j=start+1;j<end;j++)
        infor[now_cout].var+=data[j];
      //记录指数
      if(data[i]=='^'&&data[i+1]!='(')
      {
        infor[now_cout].index=Get_num(data,i+1);
        while(data[i]!='+'&&data[i]!='-'&&i<data.length())
        {
          i++;
        }
      }
      else if(data[i]=='^'&&data[i+1]=='('&&data[i+2]=='-')
      {
        i=i+2;
        infor[now_cout].index=Get_num(data,i+1);
        infor[now_cout].index*=-1;
        //跳过括号
        i++;
        while(data[i]!='+'&&data[i]!='-'&&i<data.length())
        {
          i++;
        }
      }
      else 
      {
        infor[now_cout].index=1;
      }
      start=i;
      is_have_coe=false;
      now_cout++;
   }
   //输入合法性检查
   temp_check=infor[0].var;
   for(int i=0;i<cout;i++)
   {
     if(infor[i].var.empty())
       continue;
     else if(temp_check!=infor[i].var)
     {
       is_vavild=false;break;
     }
   }
   if(!is_vavild)
   {
      string warning="wrong";
      return warning;
   }
   //进行微分操作
   for(int i=0;i<cout;i++)
   {
      double temp_coe;
      double temp_index;
      char temp_buf_1[20];
      char temp_buf_2[20];
      //如果不是常数项
      if(!infor[i].var.empty())
      {
        infor[i].coe*=infor[i].index;
        infor[i].index-=1;
        //将double转成字符串
        sprintf_s(temp_buf_1,"%.2lf",infor[i].coe);
        sprintf_s(temp_buf_2,"%.2lf",infor[i].index);
        //如果系数是正数
        if(infor[i].coe>=0)
        {
          infor[i].answer=temp_buf_1;
        }
        //如果系数是负数
        else if(infor[i].coe<0)
        {
          infor[i].answer='(';
          infor[i].answer+=temp_buf_1;
          infor[i].answer+=')';
        }
        //如果指数是正数
        if(infor[i].index>0)
        {
          infor[i].answer+='*';
          infor[i].answer+=infor[i].var+'^'+temp_buf_2;
        }
        //如果指数为0
        //直接跳过，避免多阶导数出错
        //如果指数是负数
        else if(infor[i].index<0)
        {
          infor[i].answer+=infor[i].var+'^'+'(';
          infor[i].answer+=temp_buf_2;
          infor[i].answer+=')';
        }
        temp_answer+=infor[i].answer;
        if(i+1<cout)  temp_answer+=sym[i+1];
      }
      //处理常数项
        //直接加0
      else 
      {
        temp_answer+='0';
        if(i+1<cout)  temp_answer+=sym[i+1];
      }
   }
   return temp_answer;
}

//四则表达式运算
double Make_qua_ope(string data)
{
   stack<double> num;
   stack<char> opera;
   
   /*Stack_2<char> opera2;
   Stack_2<double> num2;*/

   //对负数的预处理
   for(int i=0;i<data.length();i++)
   {
     if(data[i]=='('&&data[i+1]=='-')
       data.insert(i+1,"0");
   }

   double temp_action_num_1;
   double temp_fianl_answer;
   for(int i=0;i<data.length();i++)
   {
      if(data[i]>='0'&&data[i]<='9'||data[i]=='.')
      {
        temp_action_num_1=Get_num(data,i);
        num.push(temp_action_num_1);
        //num2.push(temp_action_num_1);
        while((data[i]>='0'&&data[i]<='9'||data[i]=='.')&&i<data.length())
        {
          i++;
        }
        //记得给这里打个点
        i--;
      }
      else if(data[i]=='(')
      {
        opera.push(data[i]);
      }
      else if(data[i]==')')
      {
        char temp_ope_1;
        double temp_num_1;
        double temp_num_2;
        double temp_answer;
        while(opera.top()!='('&&!opera.empty())
        {
          temp_num_2=num.top();
          num.pop();
          temp_num_1=num.top();
          num.pop();
          temp_ope_1=opera.top();
          opera.pop();
          temp_answer=Get_cal_num(temp_num_1,temp_num_2,temp_ope_1);
          num.push(temp_answer);
        }
        opera.pop();
      }
      else if(data[i]=='+'||data[i]=='-'||data[i]=='*'||data[i]=='/')
      {
        char temp_ope_1;
        char temp_ope_2;
        double temp_num_1;
        double temp_num_2;
        double temp_answer;
        if(opera.empty())
        {
          opera.push(data[i]);
        }
        else 
        {
          temp_ope_1=opera.top();
          if(Get_ope_value(temp_ope_1)>=Get_ope_value(data[i]))
          {
             while(!(Get_ope_value(temp_ope_1)<Get_ope_value(data[i])||opera.empty()||opera.top()=='('))
             {
               temp_ope_1=opera.top();
               temp_num_2=num.top();
               num.pop();
               temp_num_1=num.top();
               num.pop();
               temp_answer=Get_cal_num(temp_num_1,temp_num_2,temp_ope_1);
               num.push(temp_answer);
               opera.pop();
             }
             opera.push(data[i]);
          }
          else 
          {
            opera.push(data[i]);
          }
        }
      }
      else if(data[i]=='^')
      {
        double temp_ope_num;
        i++;
        temp_action_num_1=Get_num(data,i);
        temp_ope_num=num.top();
        num.pop();
        temp_action_num_1=pow(temp_ope_num,temp_action_num_1);
        num.push(temp_action_num_1);
        //num2.push(temp_action_num_1);
        while((data[i]>='0'&&data[i]<='9'||data[i]=='.')&&i<data.length())
        {
          i++;
        }
        //记得给这里打个点
        i--;
      }
   }
   if(!opera.empty())
   {
      char temp_ope_1;
      double temp_num_1;
      double temp_num_2;
      double temp_answer;
      while(!opera.empty())
      {
        temp_ope_1=opera.top();
        opera.pop();
        temp_num_2=num.top();
        num.pop();
        temp_num_1=num.top();
        num.pop();
        temp_answer=Get_cal_num(temp_num_1,temp_num_2,temp_ope_1);
        num.push(temp_answer);
      }
   }
   temp_fianl_answer=num.top();
   num.pop();
   return temp_fianl_answer;
}

//从字符串获取数字
double Get_num(string data,int i)
{
   bool is_double=false;   //确定该数是不是浮点数
   int start=i;
   int end;
   int middle;
   for(int j=i;j<data.length();j++)
   {
      if(data[j]=='.')
      {
        middle=j;
        is_double=true;
      }
     if(!(data[j]>='0'&&data[j]<='9'||data[j]=='.')) 
      {
        end=j;
        break;
      }
   }
   if(is_double)
   {
      double temp_answer=0;
      double temp_mul=1;
      for(int m=start;m<middle;m++)
      {
        temp_answer*=10;
        temp_answer+=data[m]-'0';
      }
      for(int m=middle+1;m<end;m++)
      {
         temp_mul/=10;
         temp_answer+=temp_mul*(data[m]-'0');
      }
      return temp_answer;
   }
   else 
   {
      double temp_answer=0;
      for(int m=start;m<end;m++)
      {
        temp_answer*=10;
        temp_answer+=data[m]-'0';
      }
      return temp_answer;
   }
}

//使用运算符计算数字
double Get_cal_num(double a,double b,char ope)
{
  switch(ope)
  {
    case '+':
      return a+b;
    case '-':
      return a-b;
    case '*':
      return a*b;
    case '/':
      return a/b;
  }
  return 0;
}

//赋值运算符优先级
int Get_ope_value(char a)
{
  switch(a)
  {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
  }
  return 0;
}
#pragma endregion

//矩阵运算函数
#pragma region 
void Mat_ope()
{
    bool is_end=true;
    int temp_action;
    
    while(is_end)
    {
      cout<<"*******欢迎使用矩阵运算功能**********"<<endl;
      cout<<"*      请选择您想进行的操作         *"<<endl;
      cout<<"*         1.矩阵加法                *"<<endl;
      cout<<"*         2.矩阵减法                *"<<endl;
      cout<<"*         3.矩阵乘法                *"<<endl;
      cout<<"*         4.矩阵转置                *"<<endl;
      cout<<"*        5.求矩阵行列式             *"<<endl;
      cout<<"*        6.求矩阵特征值             *"<<endl;
      cout<<"*           7.退出                  *"<<endl;
      cout<<"*************************************"<<endl;
      cout<<"请您选择您想进行的操作:";
      cin>>temp_action;
    
      switch(temp_action)
     {
        case 1:
          Mat_ope_1();
          break;
        case 2:
          Mat_ope_2();
          break;
        case 3:
          Mat_ope_3();
          break;
        case 4:
          Mat_ope_4();
          break;
        case 5:
          Mat_ope_5();
          break;
        case 6:
          Mat_ope_6();
          break;
        case 7:
          is_end=false;
          cout<<"欢迎您再次使用矩阵运算!"<<endl;
          break;
        default:
          cout<<"警告:检测到不合法输入，执行退出操作"<<endl;
          cout<<"欢迎您再次使用矩阵运算!"<<endl;
          is_end=false;
          break;
      }

    }
}

void Mat_ope_1()
{
    int hang_1,lie_1,hang_2,lie_2;
    int temp_mat_1[100][100];
    int temp_mat_2[100][100];
    int temp_mat_3[100][100];
    memset(temp_mat_1,0,sizeof(temp_mat_1));
    memset(temp_mat_2,0,sizeof(temp_mat_2));
    memset(temp_mat_3,0,sizeof(temp_mat_3));
    
    cout<<"请输入您第一个矩阵的行数:";
    cin>>hang_1;
    cout<<"请输入您第一个矩阵的列数:";
    cin>>lie_1;
    cout<<"请输入您的第一个矩阵:"<<endl;
    for(int i=1;i<=hang_1;i++)
      for(int j=1;j<=lie_1;j++)
        cin>>temp_mat_1[i][j];
    
    cout<<"请输入您第二个矩阵的行数:";
    cin>>hang_2;
    cout<<"请输入您第二个矩阵的列数:";
    cin>>lie_2;
    if(hang_1==hang_2&&lie_1==lie_2)
    {
      cout<<"请输入您的第二个矩阵:"<<endl;
      for(int i=1;i<=hang_2;i++)
        for(int j=1;j<=lie_2;j++)
          cin>>temp_mat_2[i][j];
      
      //进行运算
      for(int i=1;i<=hang_1;i++)
        for(int j=1;j<=lie_1;j++)
          temp_mat_3[i][j]=temp_mat_1[i][j]+temp_mat_2[i][j];
      
       //进行输出
       cout<<"运算结果如下:"<<endl;
       for(int i=1;i<=hang_1;i++)
       {
          for(int j=1;j<=lie_1;j++)
            cout<<temp_mat_3[i][j]<<' ';
          cout<<endl;
       }
        
       cout<<"提示:运算完毕,执行返回操作"<<endl;
    }
    else 
      cout<<"警告:检测到格式错误,执行返回操作"<<endl;
    
}

void Mat_ope_2()
{
    int hang_1,lie_1,hang_2,lie_2;
    int temp_mat_1[100][100];
    int temp_mat_2[100][100];
    int temp_mat_3[100][100];
    memset(temp_mat_1,0,sizeof(temp_mat_1));
    memset(temp_mat_2,0,sizeof(temp_mat_2));
    memset(temp_mat_3,0,sizeof(temp_mat_3));
    
    cout<<"请输入您第一个矩阵的行数:";
    cin>>hang_1;
    cout<<"请输入您第一个矩阵的列数:";
    cin>>lie_1;
    cout<<"请输入您的第一个矩阵:"<<endl;
    for(int i=1;i<=hang_1;i++)
      for(int j=1;j<=lie_1;j++)
        cin>>temp_mat_1[i][j];
    
    cout<<"请输入您第二个矩阵的行数:";
    cin>>hang_2;
    cout<<"请输入您第二个矩阵的列数:";
    cin>>lie_2;
    if(hang_1==hang_2&&lie_1==lie_2)
    {
      cout<<"请输入您的第二个矩阵:"<<endl;
      for(int i=1;i<=hang_2;i++)
        for(int j=1;j<=lie_2;j++)
          cin>>temp_mat_2[i][j];
      
      //进行运算
      for(int i=1;i<=hang_1;i++)
        for(int j=1;j<=lie_1;j++)
          temp_mat_3[i][j]=temp_mat_1[i][j]-temp_mat_2[i][j];
      
       //进行输出
       cout<<"运算结果如下:"<<endl;
       for(int i=1;i<=hang_1;i++)
       {
          for(int j=1;j<=lie_1;j++)
            cout<<temp_mat_3[i][j]<<' ';
          cout<<endl;
       }
        
       cout<<"提示:运算完毕,执行返回操作"<<endl;
    }
    else 
      cout<<"警告:检测到格式错误,执行返回操作"<<endl;
}

void Mat_ope_3()
{
    int hang_1,lie_1,hang_2,lie_2;
    int temp_mat_1[100][100];
    int temp_mat_2[100][100];
    int temp_mat_3[100][100];
    memset(temp_mat_1,0,sizeof(temp_mat_1));
    memset(temp_mat_2,0,sizeof(temp_mat_2));
    memset(temp_mat_3,0,sizeof(temp_mat_3));
    
    cout<<"请输入您第一个矩阵的行数:";
    cin>>hang_1;
    cout<<"请输入您第一个矩阵的列数:";
    cin>>lie_1;
    cout<<"请输入您的第一个矩阵:"<<endl;
    for(int i=1;i<=hang_1;i++)
      for(int j=1;j<=lie_1;j++)
        cin>>temp_mat_1[i][j];
    
    cout<<"请输入您第二个矩阵的行数:";
    cin>>hang_2;
    cout<<"请输入您第二个矩阵的列数:";
    cin>>lie_2;
    if(hang_2==lie_1)
    {
      cout<<"请输入您的第二个矩阵:"<<endl;
      for(int i=1;i<=hang_2;i++)
        for(int j=1;j<=lie_2;j++)
          cin>>temp_mat_2[i][j];
      
      //进行运算
      for(int i=1;i<=hang_1;i++)
        for(int j=1;j<=lie_2;j++)
        {
            for(int k=0;k<lie_2;k++)
              temp_mat_3[i][j]+=temp_mat_1[i][k]*temp_mat_2[k][j];
        }
      
       //进行输出
       cout<<"运算结果如下:"<<endl;
       for(int i=1;i<=hang_1;i++)
       {
          for(int j=1;j<=lie_2;j++)
            cout<<temp_mat_3[i][j]<<' ';
          cout<<endl;
       }
        
       cout<<"提示:运算完毕,执行返回操作"<<endl;
    }
    else 
      cout<<"警告:检测到格式错误,执行返回操作"<<endl;
}

void Mat_ope_4()
{
   int hang_1;
   int lie_1;
   int temp_mat_1[100][100];
   int temp_mat_2[100][100];
   memset(temp_mat_1,0,sizeof(temp_mat_1));
   memset(temp_mat_2,0,sizeof(temp_mat_2));
   cout<<"请输入您的矩阵的行数:";
   cin>>hang_1;
   cout<<"请输入您的矩阵的列数:";
   cin>>lie_1;
   cout<<"请输入您的矩阵:"<<endl;
   for(int i=1;i<=hang_1;i++)
     for(int j=1;j<=lie_1;j++)
      {
        cin>>temp_mat_1[i][j];
        temp_mat_2[j][i]=temp_mat_1[i][j];
      }
    cout<<"运算结果如下:"<<endl;
    for(int i=1;i<=lie_1;i++)
    {
      for(int j=1;j<=hang_1;j++)
        cout<<temp_mat_2[i][j]<<' ';
      cout<<endl;
    }
    cout<<"提示:运算完毕,执行返回操作"<<endl;  

}

void Mat_ope_5()
{
  int hang_1;
  int lie_1;
  double temp_mat_1[100][100];
  double temp_deter[10000];
  memset(temp_mat_1,0,sizeof(temp_mat_1));
  memset(temp_deter,0,sizeof(temp_deter));
  cout<<"请输入您的矩阵的行数:";
  cin>>hang_1;
  cout<<"请输入您的矩阵的列数:";
  cin>>lie_1;
  if(hang_1==lie_1)
    {
      int z=0;
      double temp_answer;
      cout<<"请输入您的矩阵:"<<endl;
      for(int i=1;i<=hang_1;i++)
        for(int j=1;j<=lie_1;j++)
        {
            cin>>temp_mat_1[i][j];
            temp_deter[z++]=temp_mat_1[i][j];
        }
      temp_answer=Get_deter(temp_deter,hang_1);
      cout<<"该行列式的值为:"<<temp_answer<<endl;
      cout<<"提示:运算完毕,执行返回操作"<<endl;
    }
    else 
      cout<<"警告:检测到格式错误,执行返回操作"<<endl;
}

void Mat_ope_6()
{
  int hang_1;
  int lie_1;
  double temp_mat_1[100][100];
  double first[100];
  double temp_answer;
  memset(temp_mat_1,0,sizeof(temp_mat_1));
  memset(first,0,sizeof(first));
  cout<<"请输入您的矩阵的行数:";
  cin>>hang_1;
  cout<<"请输入您的矩阵的列数:";
  cin>>lie_1;
  for(int i=1;i<=hang_1;first[i++]=1);
  cout<<"请输入您的矩阵:"<<endl;
  for(int i=1;i<=hang_1;i++)
    for(int j=1;j<=lie_1;j++)
    {
      cin>>temp_mat_1[i][j];
    }
  temp_answer=Max(first,hang_1);
  //十次数值逼近
  for(int i=0;i<10;i++)
  {
    Make_mat(temp_mat_1,first,hang_1);
    temp_answer=Max(first,hang_1);
    for(int j=1;j<=hang_1;j++)
    {
      first[j]=first[j]/temp_answer;
    }
  }
  cout<<"该矩阵的特征值为:"<<temp_answer<<endl;
  cout<<"该矩阵的特征向量为:"<<endl;
  for(int i=1;i<=hang_1;i++)
    cout<<first[i]<<' ';
  cout<<endl;
}

//左乘向量
void Make_mat(double make_mat[100][100],double* first,int hang)
{
   double temp_num;
   double second[100];
   memcpy(second,first,sizeof(int)*hang);
   for(int i=1;i<=hang;i++)
   {
     temp_num=0;
     for(int j=1;j<=hang;j++)
     {
       temp_num+=make_mat[i][j]*second[j];
     }
     first[i]=temp_num;
   }
}

double Max(double* first,int hang)
{
   double temp_answer=first[1];
   for(int i=1;i<=hang;i++)
   {
     if(first[i]>temp_answer) 
       temp_answer=first[i];
   }
   return temp_answer;
}

//构造余子式
void Str_min(double* front,double* down,int hang,int where)
{
    //front->构造前矩阵  down->构造后矩阵
    //hang->矩阵的行   where->当前构造哪一行
    int now=0;
    for(int i=1;i<hang;i++)
      for(int j=0;j<hang;j++)
        if(j!=where)
          down[now++]=front[i*hang+j];
    return ;
}

//计算行列式的值
double Get_deter(double *before,int hang)
{
   //递归基
   if(hang<=1)
   {
      return *before;
   }
   else 
   {
      double after[(hang-1)*(hang-1)];
      double temp_answer=0;
      int prefix=1;   //前缀
      for(int i=0;i<hang;i++)
      {
        Str_min(before,after,hang,i);
        temp_answer+=prefix*before[i]*Get_deter(after,hang-1);
        prefix*=-1;
      }
      return  temp_answer;
   }
}
#pragma endregion

//函数运算
#pragma region 
void Fuc_ope()
{
   bool is_end=true;
   int temp_action;

   while(is_end)
   {
     cout<<"*******欢迎使用函数运算功能**********"<<endl;
     cout<<"*      请选择您想进行的操作         *"<<endl;
     cout<<"*          1.定义函数               *"<<endl;
     cout<<"*          2.运行函数               *"<<endl;
     cout<<"*          3.查看历史函数           *"<<endl;
     cout<<"*          4.更改历史函数           *"<<endl;
     cout<<"*          5.退出                   *"<<endl;
     cout<<"*************************************"<<endl;
     cout<<"请您选择您想进行的操作:";
     cin>>temp_action;

     switch(temp_action)
     {
       case 1:
         Fuc_ope_1();
         break;
       case 2:
         Fuc_ope_2();
         break;
       case 3:
         Fuc_ope_3();
         break;
       case 4:
         Fuc_ope_4();
         break;
       case 5:
         cout<<"欢迎您再次使用矩阵运算!"<<endl;
         is_end=false;
         break;
       default:
         cout<<"警告:检测到不合法输入，执行退出操作"<<endl;
         cout<<"欢迎您再次使用矩阵运算!"<<endl;
         is_end=false;
         break;
     }
   }

}

void Fuc_ope_1()
{
   string data;
   cout<<"请输入您定义的函数:(例:f(x)=2*x+1)"<<endl;
   cin>>data;
   
    int j;
    int start;
    int end;
    //录入函数头
    for(j=0;j<data.length();j++)
      if(data[j]=='(') break;
    for(int temp=0;temp<j;temp++)
    {
      sp_fuc[fuc_num].fuc_only+=data[temp];
    }
    //录入函数名称
    for(j=0;j<data.length();j++)
      if(data[j]=='=') break;
    for(int temp=0;temp<j;temp++)
    {
      sp_fuc[fuc_num].fuc_name+=data[temp];
    }
    //录入函数主体
    for(int temp=j+1;temp<data.length();temp++)
    {
      sp_fuc[fuc_num].spe_func+=data[temp];
    }
    //录入变量名称
    for(int i=0;i<sp_fuc[fuc_num].fuc_name.length();i++)
    {
      if(sp_fuc[fuc_num].fuc_name[i]=='(') start=i;
      if(sp_fuc[fuc_num].fuc_name[i]==')') end=i;
    }
    for(int i=start+1;i<end;i++)
      sp_fuc[fuc_num].var_name+=data[i];
    //检查是否重复
    for(int i=0;i<fuc_num;i++)
    {
      if(sp_fuc[fuc_num].fuc_only==sp_fuc[i].fuc_only)
      {
        cout<<"警告:该函数已被定义,请前往修改或者重新输入"<<endl;
        cout<<"检测到不合法输入,执行返回操作"<<endl;
        return ;
      }
    }
    //检测合法性
    if(sp_fuc[fuc_num].spe_func.find(sp_fuc[fuc_num].var_name)==string::npos)
    {
      cout<<"检测到不合法输入,执行返回操作"<<endl;
    }
    else 
    {
      fuc_num++;
      cout<<"提示:您已成功定义函数!执行重置操作"<<endl;
    }
}

void Fuc_ope_2()
{
  string data;
  string temp_fuc_name;   //函数头
  string temp_spe_func;   //函数主体
  string temp_ope_num;    //操作数
  int start;
  int end;
  double temp_answer;
  cout<<"请输入您想运行的函数:(例:f(2))"<<endl;
  cin>>data;
  
  for(int i=0;i<data.length();i++)
  {
    if(data[i]=='(') start=i;
    if(data[i]==')') end=i;
  }
  //获取函数头
  for(int i=0;i<start;i++)
    temp_fuc_name+=data[i];
  //获取运算数
    //如果是正数
  if(data[start+1]!='-')
    for(int i=start+1;i<end;i++)
      temp_ope_num+=data[i];
    //如果是负数
  else 
    for(int i=start;i<=end;i++)
      temp_ope_num+=data[i];
  //查找函数
  for(int i=0;i<fuc_num;i++)
  {
     if(sp_fuc[i].fuc_only==temp_fuc_name)
     {
       temp_spe_func=sp_fuc[i].spe_func;
       Replace_all(temp_spe_func,sp_fuc[i].var_name,temp_ope_num);
       temp_answer=Make_qua_ope(temp_spe_func);
       break;
     }
  }
  cout<<"您的函数运行结果为:"<<temp_answer<<endl;
  cout<<"提示:已完成运行,执行返回操作"<<endl;
  cout<<"欢迎您再次使用运行函数功能!"<<endl;
}

void Fuc_ope_3()
{
  bool is_end=true;
  int temp_action;
  string temp_name;
  int temp_add;
  bool is_suc=false;
  
  while(is_end)
  {
    cout<<"*****请选择您查找的方式********"<<endl;
    cout<<"*      1.按名称查找           *"<<endl;
    cout<<"*      2.按顺序查找           *"<<endl;
    cout<<"*      3.退出                 *"<<endl;
    cout<<"*******************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
    {
      case 1:
        cout<<"请输入您的函数名称:(例:f(x))"<<endl;
        cin>>temp_name;
        for(int i=0;i<fuc_num;i++)
        {
           if(sp_fuc[i].fuc_name==temp_name)
           {
             cout<<"您所查找的函数为:"<<sp_fuc[i].fuc_name<<'='<<sp_fuc[i].spe_func<<endl;
             cout<<"提示:您已成功查找!执行重置操作"<<endl;
             is_suc=true;
             break;
           }
        }
        if(!is_suc)
        {
           cout<<"警告:未找到您的函数!执行重置操作"<<endl;
        }
        break;
      case 2:
        cout<<"请输入您的函数的顺序:"<<endl;
        cin>>temp_add;
        if(temp_add<=fuc_num)
        {
          temp_add--;
          cout<<"您所查找的函数为:"<<sp_fuc[temp_add].fuc_name<<'='<<sp_fuc[temp_add].spe_func<<endl;
          cout<<"提示:您已成功查找!执行重置操作"<<endl;
        }
        else 
        {
           cout<<"警告:未找到您的函数!执行重置操作"<<endl;
        }
        break;
      case 3:
        is_end=false;
        cout<<"欢迎您再次使用查找函数功能!"<<endl;
        break;
      default:
        is_end=false;
        cout<<"检测到不合法输入,执行返回操作"<<endl;
        cout<<"欢迎您再次使用查找函数功能!"<<endl;
        break;
    }
  }  
}

void Fuc_ope_4()
{
  bool is_end=true;
  int temp_action;
  string temp_name;
  int temp_add;
  bool is_suc=false;
  bool is_vavild=false;
  
  while(is_end)
  {
    cout<<"*********请选择您更改的函数**********"<<endl;
    cout<<"*      1.按名称查找并更改           *"<<endl;
    cout<<"*      2.按顺序查找并更改           *"<<endl;
    cout<<"*           3.退出                  *"<<endl;
    cout<<"*************************************"<<endl;
    cout<<"请输入您的选择:"<<endl;
    cin>>temp_action;

    switch(temp_action)
    {
      case 1:
        cout<<"请输入您的函数名称:(例:f(x))"<<endl;
        cin>>temp_name;
        for(int i=0;i<fuc_num;i++)
        {
           if(sp_fuc[i].fuc_name==temp_name)
           {
             cout<<"您所修改的函数为:"<<sp_fuc[i].fuc_name<<'='<<sp_fuc[i].spe_func<<endl;
             is_vavild=Modify_fuc(i);
             if(is_vavild)
             {
               cout<<"提示:您已成功修改!执行重置操作"<<endl;
               is_suc=true;
               break;
             }
             else 
             {
               cout<<"警告:修改失败!"<<endl;
             }
           }
        }
        if(!is_suc&&(!is_vavild))
        {
          cout<<"警告:未找到您的函数!执行重置操作"<<endl;
        }
        break;
      case 2:
        cout<<"请输入您的函数的顺序:"<<endl;
        cin>>temp_add;
        if(temp_add<=fuc_num)
        {
          temp_add--;
          cout<<"您所修改的函数为:"<<sp_fuc[temp_add].fuc_name<<'='<<sp_fuc[temp_add].spe_func<<endl;
          is_vavild=Modify_fuc(temp_add);
          if(is_vavild)
            cout<<"提示:您已成功修改!执行重置操作"<<endl;
        }
        else 
        {
           cout<<"警告:未找到您的函数!执行重置操作"<<endl;
        }
        break;
      case 3:
        is_end=false;
        cout<<"欢迎您再次使用修改函数功能!"<<endl;
        break;
      default:
        is_end=false;
        cout<<"检测到不合法输入,执行返回操作"<<endl;
        cout<<"欢迎您再次使用修改函数功能!"<<endl;
        break;
    }
  }  
}

//修改函数
bool Modify_fuc(int now_num)
{
   //now_num -> 当前函数的位置
   bool is_end=true;
   int temp_action;
   string temp_name;
   string ago_var_name;
   string temp_var_name;
   string temp_spe_fuc;
   int start;
   int end;

   while(is_end)
   {
     cout<<"********请选择您想更改的内容**********"<<endl;
     cout<<"*         1.函数名称                 *"<<endl;
     cout<<"*         2.变量名称                 *"<<endl;
     cout<<"*         3.函数解析式               *"<<endl;
     cout<<"*         4.退出                     *"<<endl;
     cout<<"**************************************"<<endl;
     cout<<"请输入您的选择:"<<endl;
     cin>>temp_action;

     switch(temp_action)
     {
       case 1:
         cout<<"请输入您更改后的函数名称:(例:g(x))"<<endl;
         cin>>temp_name;
         //获取新的变量名
         for(int i=0;i<temp_name.length();i++)
         {
           if(temp_name[i]=='(')  start=i;
           if(temp_name[i]==')')  end=i;
         }
         for(int i=start+1;i<end;i++)
           temp_var_name+=temp_name[i];
         //更改函数头
         sp_fuc[now_num].fuc_only.clear();
         for(int i=0;i<start;i++)
         sp_fuc[now_num].fuc_only+=temp_name[i];
         //更改函数名
         sp_fuc[now_num].fuc_name.clear();
         sp_fuc[now_num].fuc_name=temp_name;
         //更改变量名
           //记录先前的变量名
           ago_var_name=sp_fuc[now_num].var_name;
         sp_fuc[now_num].var_name.clear();
         sp_fuc[now_num].var_name=temp_var_name;
         //更改函数主体
         Replace_all(sp_fuc[now_num].spe_func,ago_var_name,temp_var_name);
         cout<<"提示:您已成功修改函数!"<<endl;
         cout<<"修改后的函数为:"<<sp_fuc[now_num].fuc_name<<'='<<sp_fuc[now_num].spe_func<<endl;
         cout<<"提示:执行返回操作"<<endl;
         return true;
         break;
       case 2:
         cout<<"请输入您更改后的变量名称:"<<endl;
         cin>>temp_var_name;
         //更改变量名
         ago_var_name=sp_fuc[now_num].var_name;
         sp_fuc[now_num].var_name.clear();
         sp_fuc[now_num].var_name=temp_var_name;
         //更改函数名
         Replace_all(sp_fuc[now_num].fuc_name,ago_var_name,temp_var_name);
         //更改函数主体
         Replace_all(sp_fuc[now_num].spe_func,ago_var_name,temp_var_name);
         cout<<"提示:您已成功修改函数!"<<endl;
         cout<<"修改后的函数为:"<<sp_fuc[now_num].fuc_name<<'='<<sp_fuc[now_num].spe_func<<endl;
         cout<<"提示:执行返回操作"<<endl;
         return true;
         break;
       case 3:
         cout<<"请输入您更改后的函数主体:"<<endl;
         cin>>temp_spe_fuc;
         //合法性检查
         if(temp_spe_fuc.find(sp_fuc[now_num].var_name)==string::npos)
         {
           cout<<"检测到格式错误,执行返回操作"<<endl;
           return false;
         }
         else 
         {
           sp_fuc[now_num].spe_func=temp_spe_fuc;
           cout<<"提示:您已成功修改函数!"<<endl;
           cout<<"修改后的函数为:"<<sp_fuc[now_num].fuc_name<<'='<<sp_fuc[now_num].spe_func<<endl;
           cout<<"提示:执行返回操作"<<endl;
           return true; 
         }
         break;
       case 4:
         is_end=false;
         cout<<"欢迎您继续使用修改函数功能!"<<endl;
         break;
       default:
         is_end=false;
         cout<<"警告:检测到不合法输入,执行返回操作"<<endl;
         cout<<"欢迎您继续使用修改函数功能!"<<endl;
         break;
     }
   }
   return true;
}

//批量化替换string
void Replace_all(string& str,const string& strfir,const string& strend)
{
  //str 原字符串   strfir 被替换的字符串  strend  用来替换的字符串
  string::size_type pos(0);
  string::size_type symbol(0);
  while(1)
  {
    if((pos=str.find(strfir))==0)
    {
      str.replace(pos,strfir.length(),strend);
    }
    else if((pos=str.find(strfir))!=string::npos&&pos>symbol)
    {
      str.replace(pos,strfir.length(),strend);
      symbol=pos;
    }
    else break;
  }
}
#pragma endregion

int main()
{
   bool is_end=true;
   int  temp_ope;
   while(is_end)
   {
      First_show();
      cin>>temp_ope;
      switch (temp_ope)
      {
      case 1:
        Qua_ope();
        break;
      case 2:
        Mat_ope();
        break;
      case 3:
        Fuc_ope();
        break;
      case 4:
        is_end=false;
        cout<<"欢迎您再次使用!"<<endl;
        break;
      default:
        cout<<"警告:检测到不合法输入，执行退出操作"<<endl;
        cout<<"欢迎您再次使用!"<<endl;
        is_end=false;
        break;
      }
   }

   return 0;
}
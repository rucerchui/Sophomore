#include<iostream>
#include<string.h>
#include<string>
#include<fstream>
#include<sstream>
#include<windows.h>
#define M 90001
using namespace std;

struct Node
{
   public:
     string data="";
     int num=0;
     Node* next=NULL;
     Node(){}
     Node(int _num):num(_num){}
};

struct List
{
   int node_num=0;
   Node* begin=NULL;
};

List list[M];

//ascii
int hash1(string data)
{
   int length=data.length();
   int hash=0;
   for(int i=0;i<length;i++)
   {
      hash=(hash*3+(unsigned)data[i])%M;
   }
   return hash;
}


int check(char infor)
{
   char temp;
   char answer;
   temp=0x80;
   answer=infor&temp;
   if(answer==0)
     return 1;
   temp=0xe0;
   answer=temp&infor;
   if(answer==temp)
     return 3;
   else return 2;
}

//utf
int hash2(string data)
{
   int length=data.length();
   int hash=0;
   int temp_num;
   string temp_data;
   for(int i=0;i<length;i++)
   {
      if((check(data[i]))==1)
      {
        temp_data=data[i];
        temp_num=atoi(temp_data.c_str());
        hash=(hash*2+temp_num)%M;
      }
      else if((check(data[i]))==2)
      {
        temp_data=data[i];temp_data+=data[i+1];
        temp_num=atoi(temp_data.c_str());
        hash=(hash*2+temp_num)%M;
        i=i+1;
      }
      else
      {
        temp_data=data[i];temp_data+=data[i+1];temp_data+=data[i+2];
        temp_num=atoi(temp_data.c_str());
        hash=(hash*2+temp_num)%M;
        i=i+2;
      }
   }
   return hash;
}

void confilct1(int hash_key,Node* data)
{
   if(list[hash_key].node_num==0)
   {
     list[hash_key].begin=data;
     list[hash_key].node_num++;
   }
   else
   {
     Node* temp=list[hash_key].begin;
     while(temp->next!=NULL)
     {
       temp=temp->next;
     }
     temp->next=data;
     list[hash_key].node_num++;
   }
}

void confilct2(int hash_key,Node* data)
{
  if(list[hash_key].node_num==0)
  {
    list[hash_key].begin=data;
    list[hash_key].node_num++;
  }
  else
  {
     for(int i=1;i<300;i++)
     {
        int temp_try_1;
        int temp_try_2;
        temp_try_1=(hash_key+i*i)%M;
        temp_try_2=(hash_key-i*i)%M;
        if(temp_try_1>=0)
        {
          if(list[temp_try_1].node_num==0)
          {
            list[temp_try_1].begin=data;
            list[temp_try_1].node_num++;
            break;
          }
        }
        if(temp_try_2>=0)
        {
          if(list[temp_try_2].node_num==0)
          {
            list[temp_try_2].begin=data;
            list[temp_try_2].node_num++;
            break;
          }
        }
     }
  }
}

void confilct3(int hash_key,Node* data)
{
  if(list[hash_key].node_num==0)
  {
    list[hash_key].begin=data;
    list[hash_key].node_num++;
  }
  else
  {
     for(int i=1;i<M;i++)
     {
        int temp_try_1;
        temp_try_1=(hash_key+i)%M;
        if(list[temp_try_1].node_num==0)
        {
          list[temp_try_1].begin=data;
          list[temp_try_1].node_num++;
          break;
        }
      }
  }
}

int Find_1(Node* data,int hash)
{
  if(list[hash].node_num==0) 
    return -1;
  Node* temp=list[hash].begin;
  while(temp!=NULL)
  {
    if(temp->data==data->data)
      return temp->num;
    temp=temp->next;
  }
  return -1;
}

int Find_2(Node* data,int hash)
{
  if(list[hash].node_num==0) 
    return -1;
  for(int i=1;i<300;i++)
  {
    int temp_try_1;
    int temp_try_2;
    temp_try_1=(hash+i*i)%M;
    temp_try_2=(hash-i*i)%M;
    if(temp_try_1>=0)
    {
      if(list[temp_try_1].node_num==0)
        return -1;
      if(list[temp_try_1].begin->data==data->data)
        return list[temp_try_1].begin->num;
    }
    if(temp_try_2>=0)
    {
      if(list[temp_try_2].node_num==0)
        return -1;
      if(list[temp_try_2].begin->data==data->data)
        return list[temp_try_2].begin->num;
    }
  }
  return -1;
}

int Find_3(Node* data,int hash)
{
  if(list[hash].node_num==0) 
    return -1;
  for(int i=1;i<M;i++)
  {
    int temp_try_1;
    temp_try_1=(hash+i)%M;
    if(list[temp_try_1].node_num==0)
      return -1;
    if(list[temp_try_1].begin->data==data->data)
      return list[temp_try_1].begin->num;
  }
  return -1;
}

void Make_list(Node* data,int data_type,int confilct_type)
{
   if(data_type==1)
   {
      int hash=hash1(data->data);
      switch (confilct_type)
      {
        case 1:
          confilct1(hash,data);
          break;
        case 2:
          confilct2(hash,data);
          break;
        case 3:
          confilct3(hash,data);
          break;
      }
   }
   else if(data_type==2)
   {
      int hash=hash2(data->data);
      switch (confilct_type)
      {
        case 1:
          confilct1(hash,data);
          break;
        case 2:
          confilct2(hash,data);
          break;
        case 3:
          confilct3(hash,data);
          break;
      }
   }
}

int Find_num(Node* data,int data_type,int confilct_type)
{
   if(data_type==1)
   {
      switch(confilct_type)
      {
        case 1:
          return Find_1(data,hash1(data->data));
        case 2:
          return Find_2(data,hash1(data->data));
        case 3:
          return Find_3(data,hash1(data->data));
      }
   }
   else 
   {
     switch(confilct_type)
      {
        case 1:
          return Find_1(data,hash2(data->data));
        case 2:
          return Find_2(data,hash2(data->data));
        case 3:
          return Find_3(data,hash2(data->data));
      }
   }
   return -1;
}

void Get_data(string data_set,int data_type,int confilct_type)
{ 
   int now_time=0;
   ifstream infile(data_set,ios::in);
   ofstream outfile("answer.txt",ios::out);
   if(!infile)
   {
     cout<<"fail to open the file!"<<endl;
     exit(1);
   }
   int function_type;
   string line;
   string filed;
   while(std::getline(infile,line))
   {
     Node* temp=new Node;
     istringstream sin(line);
     getline(sin,filed,' ');
     function_type=atoi(filed.c_str());
     if(function_type==0)
     {
       getline(sin,filed,' ');
       temp->data=filed;
       getline(sin,filed);
       temp->num=atoi(filed.c_str());
       Make_list(temp,data_type,confilct_type);
       now_time++;
     }
     else
     {
       getline(sin,filed);
       temp->data=filed;
       now_time++;
       outfile<<Find_num(temp,data_type,confilct_type)<<endl;
       //outfile<<temp->data<<endl;
       //outfile<<"now time="<<now_time<<endl;
       //cout<<Find_num(temp,data_type,confilct_type)<<endl;
     }
   }
   infile.close();
   outfile.close();
}

void First_printf()
{
   string goal1="test1.txt";
   string goal2="test2.txt";
   string goal3="test3.txt";
   string goal4="test4.txt";
   string goal5="test5.txt";
   string goal6="test6.txt";
   int data_set,data_type,confilct_type;
   
   cout<<"*****欢迎您使用哈希测试器!*****"<<endl;
   cout<<"请输入您想测试的数据编号:(1-6)"<<endl;
   cin>>data_set;
   cout<<"请输入您想选择的哈希函数:(1-2)"<<endl;
   cin>>data_type;
   cout<<"请输入您想选择的冲突策略:(1-3)"<<endl;
   cin>>confilct_type;
   DWORD start,end;
   start=GetTickCount();
   switch(data_set)
   {
      case 1:
        Get_data(goal1,data_type,confilct_type);
        break;
      case 2:
        Get_data(goal2,data_type,confilct_type);
        break;
      case 3:
        Get_data(goal3,data_type,confilct_type);
        break;
      case 4:
        Get_data(goal4,data_type,confilct_type);
        break;
      case 5:
        Get_data(goal5,data_type,confilct_type);
        break;
      case 6:
        Get_data(goal6,data_type,confilct_type);
        break;
   }
   end=GetTickCount()-start;
   cout<<"测试成功!结果已保存至answer.txt"<<endl;
   cout<<"本次测试用时"<<end<<"ms"<<endl;
   cout<<"欢迎您再次使用!"<<endl;
}

void Delete_all()
{
   for(int i=0;i<M;i++)
   {
      if(list[i].node_num==0)
        continue;
      Node* temp=list[i].begin;
      while(temp->next!=NULL)
      {
        Node* temp2=temp;
        temp=temp->next;
        delete temp2;
      }
   }
}

int main()
{
   First_printf();
   Delete_all();
   return 0;
}
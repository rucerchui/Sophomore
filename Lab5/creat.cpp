#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>
using namespace std;

struct Data
{
  string name="";
  int number=0;
};
Data data[616000];
int total_num;

void Get_data(string data_set)
{
   ifstream infile(data_set,ios::in);
   if(!infile)
   {
     cout<<"打开文件失败!"<<endl;
     exit(1);
   }
   int i=0;
   string line;
   string filed;
   while(std::getline(infile,line))
   {
     istringstream sin(line);
     getline(sin,filed,' ');
     data[i].number=atoi(filed.c_str());
     getline(sin,filed,' ');
     data[i].name=filed;
     getline(sin,filed,' ');
     getline(sin,filed);
     i++;
   }
   total_num=i-1;
   infile.close();
}

void Out_data(int type)
{
    time_t time(time_t* t);
    //type1: write 10000 data
    if(type==1)
    {
      ofstream outfile("test1.txt",ios::out);
      int now=0;
      int temp_type;
      int temp_number;
      int temp_true;
      vector<short> out_data;
      std::vector<short>::iterator iter=out_data.begin();
      srand((unsigned)time(NULL));
      while(now<10000)
      {
        temp_type=rand()%2;
        if(temp_type==0)
        {
          temp_number=rand()%total_num;
          iter=find(out_data.begin(),out_data.end(),temp_number);
          if(iter!=out_data.end())
          {
             continue;
          }
          out_data.push_back(temp_number);
          outfile<<temp_type<<' '<<data[temp_number].name<<' '<<data[temp_number].number<<endl;
          now++;
        }
        else
        {
           temp_true=rand()%100;
           if(temp_true<81&&out_data.size()>1)
           {
              temp_number=rand()%out_data.size();
              outfile<<temp_type<<' '<<data[out_data[temp_number]].name<<endl;
           }
           else
           {
              temp_number=rand()%total_num;
              outfile<<temp_type<<' '<<data[temp_number].name<<endl;
           }
           now++;
        }
      }
      outfile.close();
    }
    //type2: write 100000 data
    else if(type==2)
    {
      ofstream outfile("test2.txt",ios::out);
      int now=0;
      int temp_type;
      int temp_number;
      int temp_true;
      vector<short> out_data;
      std::vector<short>::iterator iter=out_data.begin();
      srand((unsigned)time(NULL));
      while(now<100000)
      {
        temp_type=rand()%2;
        if(temp_type==0)
        {
          temp_number=rand()%total_num;
          iter=find(out_data.begin(),out_data.end(),temp_number);
          if(iter!=out_data.end())
          {
             continue;
          }
          out_data.push_back(temp_number);
          outfile<<temp_type<<' '<<data[temp_number].name<<' '<<data[temp_number].number<<endl;
          now++;
        }
        else
        {
           temp_true=rand()%100;
           if(temp_true<81&&out_data.size()>1)
           {
              temp_number=rand()%out_data.size();
              outfile<<temp_type<<' '<<data[out_data[temp_number]].name<<endl;
           }
           else
           {
              temp_number=rand()%total_num;
              outfile<<temp_type<<' '<<data[temp_number].name<<endl;
           }
           now++;
        }
      }
      outfile.close();
    }
    //type3: write 500000 data
    else
    {
      ofstream outfile("test3.txt",ios::out);
      int now=0;
      int temp_type;
      int temp_number;
      int temp_true;
      vector<short> out_data;
      std::vector<short>::iterator iter=out_data.begin();
      srand((unsigned)time(NULL));
      while(now<500000)
      {
        temp_type=rand()%2;
        if(temp_type==0)
        {
          temp_number=rand()%total_num;
          iter=find(out_data.begin(),out_data.end(),temp_number);
          if(iter!=out_data.end())
          {
             continue;
          }
          out_data.push_back(temp_number);
          outfile<<temp_type<<' '<<data[temp_number].name<<' '<<data[temp_number].number<<endl;
          now++;
        }
        else
        {
           temp_true=rand()%100;
           if(temp_true<81&&out_data.size()>1)
           {
              temp_number=rand()%out_data.size();
              outfile<<temp_type<<' '<<data[out_data[temp_number]].name<<endl;
           }
           else
           {
              temp_number=rand()%total_num;
              outfile<<temp_type<<' '<<data[temp_number].name<<endl;
           }
           now++;
        }
      }
      outfile.close();
    }
}

int main()
{
   //the type of data set to read in
   int in_type;
   //the type od data set to write out
   int out_type;
   string set1="poj.txt";
   string set2="hdu.txt";
   cin>>in_type;
   if(in_type==1)
   {
     Get_data(set1);
   }
   else 
   {
     Get_data(set2);
   }
   cin>>out_type;
   Out_data(out_type);
   return 0;
}
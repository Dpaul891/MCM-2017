#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>
using namespace std;
#define MAXN 1000
#define INF 999999
int Rou=36000;//密度
int L=5;//车道数
int B=13;//收费站亭数
int Vmax=5;//太大吧？
int L0=16;
int N=100;//多出来的车道长度
int Wait_time=3;
int sum2;
struct stu
{
    bool occupy;
    int v;
    bool dao;
    int d_left,d_right,d_back;
    int dn;
    int wait_time;
};
stu Car[MAXN][MAXN];
int Max(int a,int b)
{
    if(a>b)
        return a;
    return b;
}
int Min(int a,int b)
{
    if(a>b)
        return b;
    return a;
}
void initial()
{
    for(int i=1;i<=B;i++)
    {
        for(int j=1;j<=2*N+2*L0+1;j++)
        {
            Car[i][j].dn=INF;
            Car[i][j].d_left=INF;
            Car[i][j].d_right=INF;
            Car[i][j].d_back=INF;
        }
    }
    for(int i=5;i<=9;i++)
        for(int j=1;j<=2*N+2*L0+1;j++)
            Car[i][j].dao=true;
    for(int j=N+1;j<=N+2*L0+1;j++)
    {
        Car[4][j].dao=true;
        Car[10][j].dao=true;
    }
    for(int j=N+1+L0/4;j<=N+2*L0+1-L0/4;j++)
    {
        Car[3][j].dao=true;
        Car[11][j].dao=true;
    }
    for(int j=N+1+2*L0/4;j<=N+2*L0+1-2*L0/4;j++)
    {
        Car[2][j].dao=true;
        Car[12][j].dao=true;
    }
    for(int j=N+1+3*L0/4;j<=N+2*L0+1-3*L0/4;j++)
    {
        Car[1][j].dao=true;
        Car[13][j].dao=true;
    }
    /*初始形状
    for(int i=1;i<=13;i++)
    {
        for(int j=101;j<=141;j++)
            cout<<Car[i][j].dao;
        cout<<endl;
    }*/
}
int sum;
bool check_changestreet1(int i,int k)
{
    if(i==5&&(k==1))
        return true;
    else if(i==6&&(k==2||k==3||k==4||k==5))
        return true;
    else if(i==7&&(k==6||k==7||k==8))
        return true;
    else if(i==8&&(k==9||k==10||k==11||k==12))
        return true;
    else if(i==9&&(k==13))
        return true;
    return false;
}
void generate()
{
    int cnt=0;
    while(1)
    {
        int k=rand()%L+1+4;
        cnt++;
        if(Car[k][1].occupy==false)
        {
            sum++;
            Car[k][1].occupy=true;
            Car[k][1].v=rand()%Vmax+1;
        }
        if(cnt==Rou/3600)
            break;
    }
}
int find_dn(int i,int j)
{
    int dn;
    if(j>=N+L0+1)
        dn=INF;
    else dn=N+L0+1-j;//?
    for(int k=j+1;k<=2*N+2*L0+1;k++)
    {
        if(Car[i][k].occupy==true||Car[i][k].dao==false)
        {
            dn=Min(dn,k-j);
            break;
        }
    }
    return dn;
}
int find_d_left(int i,int j)
{
    int d_left;
    if(j>=N+L0+1)
        d_left=INF;
    else d_left=N+L0+1-j;//?
    for(int k=j+1;k<=2*N+2*L0+1;k++)
    {
        if(Car[i-1][k].occupy==true||Car[i][k].dao==false)
        {
            d_left=Min(d_left,k-j);
            break;
        }
    }
    return d_left;
}
int find_d_right(int i,int j)
{
    int d_right;
    if(j>=N+L0+1)
        d_right=INF;
    else d_right=N+L0+1-j;//?
    for(int k=j+1;k<=2*N+2*L0+1;k++)
    {
        if(Car[i+1][k].occupy==true||Car[i][k].dao==false)
        {
            d_right=Min(d_right,k-j);
            break;
        }
    }
    return d_right;
}
int rinima;
void fare()
{

    for(int i=1;i<=B;i++)
    {
        if(Car[i][N+L0+1].occupy==true)
        {
            rinima++;
            Car[i][N+L0+1].wait_time++;
            if(Car[i][N+L0+1].wait_time==Wait_time)
            {
                sum2++;
                Car[i][N+L0+1].occupy=false;
                Car[i][N+L0+1].wait_time=0;
                Car[i][N+L0+1+1].occupy=true;
                Car[i][N+L0+1+1].v=Car[i][N+L0+1].v;
                int j=N+L0+1+1;
                int dn=find_dn(i,j);
                Car[i][j].v=Min(Car[i][j].v+1,Vmax);//加速
                Car[i][j].v=Min(Car[i][j].v,dn);//减速
                //更新位置
                Car[i][j+Car[i][j].v-1].occupy=true;
                Car[i][j].occupy=false;
                Car[i][j+Car[i][j].v-1].v=Car[i][j].v;
            }
        }
    }
}
int huandao;
void add_minus()
{
    int d_left,d_right,dn;
    for(int i=1;i<=B;i++)
    {
        for(int j=2*N+2*L0+1;j>=1;j--)
        {
            if(j==N+L0+1||Car[i][j].dao==false||Car[i][j].occupy==false)
                continue;
            //找dn,d_left,d_right,d_back;
            dn=find_dn(i,j);
            d_left=find_d_left(i,j);
            d_right=find_d_right(i,j);
            Car[i][j].v=Min(Car[i][j].v+1,Vmax);//加速
            Car[i][j].v=Min(Car[i][j].v,dn);//减速
            //变道&更新位置
            if(check_changestreet1(i,i+1)==true&&dn<Min(Car[i][j].v+1,Vmax)&&d_right>dn&&d_left<=d_right&&Car[i+1][j].occupy==false&&Car[i+1][j].dao==true)
            {
                Car[i][j].occupy=false;
                Car[i+1][j].occupy=true;
                huandao++;
                Car[i+1][j].v=Car[i][j].v;
            }
            else if(check_changestreet1(i,i-1)==true&&dn<Min(Car[i][j].v+1,Vmax)&&d_left>dn&&d_left>=d_right&&Car[i-1][j].occupy==false&&Car[i-1][j].dao==true)
            {
                Car[i][j].occupy=false;
                Car[i-1][j].occupy=true;
                Car[i-1][j].v=Car[i][j].v;
                huandao++;
            }
            else
            {

                if(j+Car[i][j].v==N+L0+1&&Car[i][N+L0+1].occupy==false)
                {
                    Car[i][j+Car[i][j].v].occupy=true;
                    Car[i][j].occupy=false;
                    Car[i][j+Car[i][j].v].v=Car[i][j].v;
                }
                else
                {
                    Car[i][j+Car[i][j].v-1].occupy=true;
                    Car[i][j].occupy=false;
                    Car[i][j+Car[i][j].v-1].v=Car[i][j].v;
                }
            }
        }
    }
}
void shape()
{
    for(int m=1;m<=B;m++)
    {
        for(int n=1;n<=2*N+2*L0+1;n++)
            cout<<Car[m][n].occupy;
        cout<<endl;
    }
    cout<<endl;
}
int main()
{
    //cin>>Rou;
    srand(time(NULL));
    initial();
    for(int i=1;i<=3600;i++)
    {
        generate();
        fare();
        add_minus();
    }
    //cout<<sum<<endl;
    cout<<sum2<<endl;
    return 0;
}

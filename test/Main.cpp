#include<cstdio>
#include<vector>
#include<stack>
#include<iostream>
using namespace std;
typedef pair<int,int>pii;
const int MAX=100000+5;
char source[MAX];
bool is[MAX];
stack<int>save;
vector<pii>ans;
int main()
{
    char c;
    int i=0;
    puts("Case #1:");
    while(~(c=getchar()))source[i++]=c;
    int p=-1;
    for(int j=0;j<i;j++)
    {
        if(source[j]=='/')
        {
            if(p!=(j-1)&&source[j-1]=='*'&&!save.empty())
            {
                int left=save.top();
                save.pop();
                if(save.empty())ans.push_back(make_pair(left,j));
            }
            else
            {
                if(j<i-1&&source[j+1]=='*')
                {
                    save.push(j);
                    p=j+1;
                }
            }
        }
    }
    const int len=ans.size();
    for(int j=0;j<len;j++)
    {
        is[ans[j].first]=1;
        is[ans[j].second]=1;
    }
    bool k=1;
    for(int j=0;j<i;j++)
    {
        if(k&&!is[j])putchar(source[j]);
        k^=is[j];
    }
    putchar('\n');
    return 0;
}
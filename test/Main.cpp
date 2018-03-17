#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <cstring>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

const int maxn = 300005;
char str[maxn];

struct Aho_Corasick_Automaton
{
    struct node
    {
        int pre, c;
        int next[2];
        int failed;
        int is_accept;
        int count;
        int dep;
        int word;
    };
    node trie[maxn];
    int top;
    void buildnode(int p, int pre, int c, int d, int w)
    {
        trie[p].pre = pre;
        trie[p].word = w;
        trie[p].dep = d;
        trie[p].c = c;
        for(int i = 0; i < 2; i++)
            trie[p].next[i] = -1;
        trie[p].failed = 0;
        trie[p].is_accept = 0;
        trie[p].count = 0;
    }

    void init()
    {
        top = 1;
        buildnode(0, 0, -1, 0, 0);
    }
    void insert(char *str, int len)
    {
        int p = 0;
        for(int i = 0; i < len; i++)
        {
            int c = str[i] - '0';
            if(trie[p].next[c] == -1)
            {
                buildnode(top, p, c, trie[p].dep + 1, (trie[p].word<<1)+c);
                trie[p].next[c] = top;
                top++;
            }
            p=trie[p].next[c];
        }
        trie[p].is_accept++;
    }
    void buildfail()
    {
        queue<int> q;
        q.push(0);
        while(!q.empty())
        {
            int p = q.front();
            q.pop();
            int pp = trie[p].pre;
            while(pp != 0)
            {
                pp = trie[pp].failed;
                if(trie[pp].next[trie[p].c] != -1)
                {
                    pp = trie[pp].next[trie[p].c];
                    break;
                }
            }
            trie[p].failed = pp;
            for(int i = 0; i < 2; i++)
            {
                if(trie[p].next[i] != -1)
                    q.push(trie[p].next[i]);
            }
        }
    }
    int ans;
    void RunAC(char *str, int len)
    {
        ans = 0;
        int p = 0;
        for(int i = 0; i < len; i++)
        {
            int c = str[i] - '0';
            while(trie[p].next[c] == -1 && p != 0) p = trie[p].failed;
            p = trie[p].next[c];
            if(p == -1) p = 0;
            int temp = p;
            while(temp != 0)
            {
                if(trie[temp].is_accept)
                {
                    trie[temp].count++;
                    ans += trie[temp].is_accept;
                }
                temp = trie[temp].failed;
            }
        }
    }
}A;

struct node1
{
    int word, len;
    node1(int ww, int ll):word(ww), len(ll) {}
    void printString()
    {
        char s[20];
        for(int i = len-1; i >= 0; i--)
        {
            s[len-i-1] = ((word&(1<<i))?'1':'0');
        }
        s[len] = '\0';
        printf("%s", s);
    }
    bool operator < (const node1 &a) const
    {
        if(len == a.len) return word < a.word;
        return len < a.len;
    }
};

vector<int> vec;
map<int, int> mp;
vector<node1> ans[55];

int main()
{
    //freopen("1.txt", "r", stdin);
    char buffer[1000];
    char name[1000];
    int flag = scanf("%s", buffer);
    while(true)
    {
        if(flag == EOF) break;
        strcpy(name, buffer);
        int a, b, n;
        scanf("%d%d%d", &a, &b, &n);
        vec.clear();
        mp.clear();
        for(int i = 0; i < n; i++) ans[i].clear();
        A.init();
        char s[15];
        for(int len = a; len <= b; len++)
        {
            s[len] = '\0';
            for(int i = 0; i < (1<<(len)); i++)
            {
                for(int j = 0; j < len; j++)
                {
                    s[len-j-1] = (i&(1<<j))? '1' : '0';
                }
                A.insert(s, len);
            }
        }
        int cnt = 0;
        str[0] = '\0';
        flag = scanf("%s", buffer);
        while(flag != EOF && (buffer[0] == '0' || buffer[0] == '1'))
        {
            for(int i = 0; i < strlen(buffer); i++)
                str[cnt++] = buffer[i];
            flag = scanf("%s", buffer);
        }

        str[cnt] = '\0';
        A.buildfail();
        A.RunAC(str, cnt);
        for(int i = 0; i < A.top; i++)
        {
            vec.push_back(A.trie[i].count);
        }
        sort(vec.begin(), vec.end());
        int k = unique(vec.begin(), vec.end()) - vec.begin();
        for(int i = k-1, j = 0; i > 0 && j < n; i--, j++)
        {
            mp[vec[i]] = j;
        }
        for(int i = 0; i < A.top; i++)
        {
            int cnt = A.trie[i].count;
            //printf("%d %d\n", i, A.trie[i].count);
            if(mp.find(cnt) == mp.end()) continue;
            ans[mp[cnt]].push_back(node1(A.trie[i].word, A.trie[i].dep));
        }
        printf("%s\n", name);
        char s1[50];
        for(map<int,int>::reverse_iterator rit = mp.rbegin(); rit != mp.rend(); rit++)
        {
            printf("%d\n", rit->first);
            int i = rit->second;
            sort(ans[i].begin(), ans[i].end());
            for(int j = 0; j < ans[i].size(); j++)
            {
                //printf("%d ", ans[i][j].word);
                //cout << ans[i][j].toString() << " ";
                ans[i][j].printString();
                printf("%c", ((j%6==5)||(j == (ans[i].size()-1)))? '\n':' ');
            }
        }
    }
    return 0;
}
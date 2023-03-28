#include<iostream>
#include<vector>
#include<queue>
#include<set>
using namespace std;




int main()
{   
    set<vector<int>> s;
    s.insert({1,0,0});
    s.insert({0,0,1});
    cout<<s.size();
    return 0;
}
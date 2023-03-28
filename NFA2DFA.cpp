#include<iostream>
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
#include<queue>
#include<set>
#include<algorithm>
using namespace std;

vector<int> extractStates(string str){
    vector<int> v;
    stringstream ss(str);
    string word;
    while(!ss.eof()){
        getline(ss, word, ',');
        v.push_back(stoi(word));
    }
    return v;
}

int main(){

    // ios_base::sync_with_stdio(false);
    // cout.tie(NULL);
    // #ifndef ONLINE_JUDGE
    // freopen("output.txt", "w", stdout);
    // #endif

    ifstream fin;
    string line;
    
    vector<vector<vector<int>>> NFA;
    set<vector<int>> combinedStates;
    queue<vector<int>> q;
    
    vector<int> final;
    int row=0, initial;
    fin.open("NFA2DFA.txt");
    
    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        vector<vector<int>> temp;
        int col=0;
        
        while(ss >> word){
            if(row==0){
                initial=stoi(word);
            }
            else if(row==1){
                final.push_back(stoi(word));
            }   
            else{
                vector<int> states = extractStates(word);
                sort(states.begin(), states.end());
                temp.push_back(states);
                if(states.size()>1){
                    combinedStates.insert(states);
                    q.push(states);
                }
            }
        }
        if(row>1){
            NFA.push_back(temp);
        }
        row++;
                                        
    }
    fin.close();

    cout<<"NFA : \n";
    for(auto r:NFA){
        for(auto c:r){
            for(auto st:c) cout<<st;
            cout<<" ";
        }
        cout<<endl;
    }
    cout<<endl;


    while(!q.empty()){
        auto states = q.front();
        q.pop();

        vector<vector<int>> newTransition;

        for(int i=0; i<NFA[0].size(); i++){
            // vector<int> newStates;
            set<int> newStates;

            for(auto st:states){
                for(auto sp:NFA[st][i]){
                    if(sp!=-1)
                        newStates.insert(sp);
                }
            }
            vector<int> v;
            for(auto ns:newStates){
                v.push_back(ns);
            }

            if(newStates.size()<2){
                if(v.size()==0){
                    v.push_back(-1);
                }
                newTransition.push_back(v);
                continue;
            }

            newTransition.push_back(v);

            int sz = combinedStates.size();
            combinedStates.insert(v);

            if(combinedStates.size()>sz){
                q.push(v);
            }
        }
        NFA.push_back(newTransition);
    }

    cout<<"\nDFA : \n";
    for(auto r:NFA){
        for(auto c:r){
            for(auto st:c) cout<<st;
            cout<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    
    return 0;
}
#include<iostream>
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
#include<queue>
#include<set>
#include<map>
#include<algorithm>
using namespace std;

char startingSymbol;
vector<vector<char> > Grammar;
map<char, vector<int>> variables;
map<char, vector<pair<int,int> > > varPos;
map<char, bool> terminals, produceEpsilon;
map<char, set<char> > First, Follow;

set<char>::iterator st;
map<char, vector<int> >::iterator v;
map<char,bool>::iterator t;
map<char, set<char> >::iterator f;
map<char, vector<pair<int,int> > >::iterator pos;

void takeUnion(char ch, set<char> s, bool val){
    if(val){
        for(st=s.begin(); st!=s.end(); st++){
            if(*st!='#')
                First[ch].insert(*st);
        }
    }
    else{
        for(st=s.begin(); st!=s.end(); st++){
            if(*st!='#')
                Follow[ch].insert(*st);
        }
    }
}

set<char> FindFirst(char ch, char parent){
    if(terminals[ch]){
        set<char> s;
        s.insert(ch);
        return s;
    }

    if(First[ch].size()>0)
        return First[ch];
    
    vector<int> indices = variables[ch];
    for(int i=0; i<indices.size(); i++){
        // cout<<indices[i]<<" ";
        int j=1;
        // for all those variables which produce epsilons
        while(j<Grammar[indices[i]].size() && produceEpsilon.find(Grammar[indices[i]][j])!=produceEpsilon.end() && ch!=Grammar[indices[i]][j] && Grammar[indices[i]][j]!=parent){
            set<char> s = FindFirst(Grammar[indices[i]][j], ch);
            takeUnion(ch, s, true);
            j++;
        }
        // all variables produde epsilon
        if(j==Grammar[indices[i]].size()){
            First[ch].insert('#');
            produceEpsilon[ch]=true;
        }
        // if you encounter same variable leave this production
        else if(ch==Grammar[indices[i]][j]){
            continue;
        }
        // to avoid inter dependency of variables
        else if(parent==Grammar[indices[i]][j]){
            continue;
        }
        // else you got your first
        else{
            set<char> s = FindFirst(Grammar[indices[i]][j], ch);
            takeUnion(ch, s, true);
        }
    }
    
    return First[ch];
}

void ComputingFirsts(){
    for(v=variables.begin(); v!=variables.end(); v++){
        set<char> s = FindFirst(v->first, '@');
        takeUnion(v->first, s, true);
    }

    for(t=produceEpsilon.begin(); t!=produceEpsilon.end(); t++){
        First[t->first].insert('#');
    }

    cout<<"\n\nFirsts : \n";
    for(f=First.begin(); f!=First.end(); f++){
        cout<<f->first<<" : {";
        for(st=f->second.begin(); st!=f->second.end(); st++){
            cout<<*st<<", ";
        }
        cout<<"}\n";
    }
}

set<char> FindFollow(char ch, char parent){
    if(terminals[ch]){
        set<char> s;
        s.insert(ch);
        return s;
    }

    // starting symbol does not only have dollar
    if(ch==startingSymbol && Follow[ch].size()>1)
        return Follow[ch];

    if(ch!=startingSymbol && Follow[ch].size()>0)
        return Follow[ch];

    vector<pair<int,int> > coor = varPos[ch];
    for(int i=0; i<coor.size(); i++){
        int row=coor[i].first;
        int col=coor[i].second;

        int j=col+1;

        while(j<Grammar[row].size() && produceEpsilon.find(Grammar[row][j])!=produceEpsilon.end() && ch!=Grammar[row][j]){
            set<char> s = First[Grammar[row][j]];
            takeUnion(ch, s, false);
            j++;
        }

        // all variables produce variables
        if(j==Grammar[row].size() && ch!=Grammar[row][0] && parent!=Grammar[row][0]){
            set<char> s = FindFollow(Grammar[row][0], ch);
            takeUnion(ch, s, false);
        }
        //  else you got your follow
        else{
            if(terminals[Grammar[row][j]]){
                Follow[ch].insert(Grammar[row][j]);
            }
            else{
                set<char> s = First[Grammar[row][j]];
                takeUnion(ch, s, false);
            }
        }
    }
    return Follow[ch];
}

void ComputingFollows(){
    Follow[startingSymbol].insert('$');
    for(v=variables.begin(); v!=variables.end(); v++){
        set<char> s = FindFollow(v->first, '@');
        takeUnion(v->first, s, false);
    }

    cout<<"\nFollows : \n";
    for(f=Follow.begin(); f!=Follow.end(); f++){
        cout<<f->first<<" : {";
        for(st=f->second.begin(); st!=f->second.end(); st++){
            cout<<*st<<", ";
        }
        cout<<"}\n";
    }
}

void printDetails(){
    cout<<"GRAMMAR : \n";
    for(int i=0; i<Grammar.size(); i++){
        cout<<Grammar[i][0]<<" -> ";
        for(int j=1; j<Grammar[i].size(); j++){
            cout<<Grammar[i][j];
        }
        cout<<endl;
    }
    
    cout<<"\nVariables : {";
    map<char, vector<int>>::iterator v;
    for(v=variables.begin(); v!=variables.end(); v++){
        cout<<v->first<<", ";
        // for(int i=0; i<v->second.size(); i++){
        //     cout<<v->second[i]<<" ";
        // }
        // cout<<endl;
    }
    cout<<"}";
    
    cout<<"\n\nTerminals : {";
    for(t=terminals.begin(); t!=terminals.end(); t++) cout<<t->first<<", ";
    cout<<"}";
    // cout<<"\nProduce Epsilons : ";
    // for(t=produceEpsilon.begin(); t!=produceEpsilon.end(); t++) cout<<t->first<<" ";
    // cout<<endl;

    // cout<<"\nVariables positions on RHS : \n";
    // for(pos=varPos.begin(); pos!=varPos.end(); pos++){
    //     cout<<pos->first<<" : ";
    //     for(int i=0; i<pos->second.size(); i++)
    //         cout<<"{"<<pos->second[i].first<<", "<<pos->second[i].second<<"} ";
    //     cout<<endl;
    // }
}

int main(){

    // ios_base::sync_with_stdio(false);
    // cout.tie(NULL);
    // #ifndef ONLINE_JUDGE
    // freopen("output.txt", "w", stdout);
    // #endif
    
    ifstream fin;
    string line;
    
    fin.open("FirstFollow.txt");
    int row=0;

    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        vector<char> temp; 
        int col=0;
        char firstV=' ';
        while(ss >> word){
            if(row==0 && col==0){
                startingSymbol=word[0];
            }
            if(col==0){
                firstV=word[0];
                variables[word[0]].push_back(row);
            }
            else if(isalpha(word[0]) && isupper(word[0])){
                pair<int,int> p = make_pair(row,col);
                varPos[word[0]].push_back(p);
            }
            if((isalpha(word[0]) && islower(word[0])) || !isalpha(word[0])){
                terminals[word[0]]=true;
            }
            if(word[0]=='#'){
                produceEpsilon[firstV]=true;
            }
            temp.push_back(word[0]);
            col++;
        }
        Grammar.push_back(temp);
        row++;
    }
    fin.close();

    printDetails();
    
    ComputingFirsts();

    ComputingFollows();

    return 0;
}
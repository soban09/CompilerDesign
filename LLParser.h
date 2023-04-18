#include "FirstFollowHeader.h"

map<char, int> rowVariables, colTerminals;
int row, col;
vector<vector<vector<int> > > LLParser;
map<char,int>::iterator rc;
vector<char> V,T;

set<char> setUnion(set<char> s1, set<char> s2){
    set<char> sett;
    for(st=s1.begin(); st!=s1.end(); st++){
        if(*st!='#')
            sett.insert(*st);
    }
    for(st=s2.begin(); st!=s2.end(); st++){
        if(*st!='#')
            sett.insert(*st);
    }
    return sett;
}

void add(int prod, char ch, set<char> var){
    for(st=var.begin(); st!=var.end(); st++){
        LLParser[rowVariables[ch]][colTerminals[*st]].push_back(prod);
    }
}

void printLine(){
    string hyphens="------------";
    cout<<endl;
    for(int j=0; j<colTerminals.size()+1; j++){
        if(j==0)
            cout<<"+----------";
        else if(j==colTerminals.size())
            cout<<"-----------+"; 
        else
            cout<<hyphens;
    }  
    cout<<endl;
}

void printLLParserTable(){
    cout<<"\n\nLL PARSER TABLE : \n";
    printLine();
    
    cout<<"|         ";
    for(int j=0; j<T.size(); j++)
        cout<<"|     "<<T[j]<<"     ";
    cout<<"|";
    printLine();
    
    int row=0;
    for(int i=0; i<LLParser.size(); i++){
        cout<<"|    "<<V[row]<<"    |";
        for(int j=0; j<LLParser[i].size(); j++){
            if(LLParser[i][j].size()==1){
                cout<<"      "<<LLParser[i][j][0]<<"    |";
            }
            else if(LLParser[i][j].size()==0){
                cout<<"           |";
            }
            else{
                cout<<"    ";
                for(int k=0; k<LLParser[i][j].size(); k++){
                    cout<<LLParser[i][j][k]<<",";
                }
                cout<<"   |";
            }
        }
        printLine();
        row++;
    }
}

void run2(){
    
    run();
    printDetails();
    ComputingFirsts();
    ComputingFollows();
    // cout<<endl<<endl;
    
    int i=0;
    for(v=variables.begin(); v!=variables.end(); v++){
        rowVariables[v->first]=i++;
    }
    row=i;
    i=0;
    for(t=terminals.begin(); t!=terminals.end(); t++){
        if(t->first == '#' || (isalpha(t->first) && isupper(t->first)))
            continue;
        colTerminals[t->first]=i++;
    }
    colTerminals['$']=i++;
    col=i;
    
    LLParser.resize(row, vector<vector<int> >(col));
    V.resize(row);
    T.resize(col);
    
    for(int i=0; i<Grammar.size(); i++){
        set<char> var;
        int j=1;
        while(j<Grammar[i].size() && Grammar[i][j]!='#' && produceEpsilon.find(Grammar[i][j])!=produceEpsilon.end() && colTerminals.find(Grammar[i][j])==colTerminals.end()){
            var = setUnion(var, First[Grammar[i][j]]);
            j++;
        }
        if(j==Grammar[i].size() || Grammar[i][j]=='#'){
            var = setUnion(var, Follow[Grammar[i][0]]);
        }
        else if(colTerminals.find(Grammar[i][j])!=colTerminals.end()){
            var.insert(Grammar[i][j]);
        }
        else{
            var = setUnion(var, First[Grammar[i][j]]);
        }
        add(i, Grammar[i][0], var);
    }
    
    
    i=0;
    for(rc=rowVariables.begin(); rc!=rowVariables.end(); rc++){
        V[i]=rc->first;
        i++;
    }
    
    i=col-1;
    for(rc=colTerminals.begin(); rc!=colTerminals.end(); rc++){
        T[i]=rc->first;
        i = (i+1)%col;
    }
    
    printLLParserTable();
}
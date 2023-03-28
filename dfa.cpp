#include<iostream>
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
using namespace std;

// int stoi(string s){
//     int res=0;
//     if(s[0]=='-'){
//         for(int i=1;i<s.length();i++){
//             res=(res*10)+(s[i]-'0');
//         }
//         res*=-1;
//     } else {
//         for(int i=0;i<s.length();i++ ){
//             res=(res*10)+(s[i]-'0');
//         }
//     }
//     return res;
// }

pair<int,int> RowsCol(){
    ifstream fin;
    string line;
    fin.open("dfa.txt");
    int mx=-1, row=0, col=0;
    
    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        int c=0;
        while(ss >> word){
            mx = max(mx, stoi(word));
            c++;
        }
        col = max(c, col);
        row++;
    }
    fin.close();
    return {max(mx+1, row-2), col};
}

int main(){
    ifstream fin;
    string line;
    
    vector<string> s;
    vector<int> initial, final;
    auto [noOfRows, noOfCol] = RowsCol();
    vector<vector<int>> DFA(noOfRows, vector<int>(noOfCol,-1));

    
    int row=0;
    fin.open("dfa.txt");
    bool firstRow=false, secRow=false;
    
    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        vector<int> temp;
        int col=0;
        while(ss >> word){
            if(!firstRow){
                initial.push_back(stoi(word));
            } else if(!secRow){
                final.push_back(stoi(word));
            } else {
                DFA[row][col]=stoi(word);
            }
            col++;
        }
        if(!firstRow)
            firstRow=true;
        else if(!secRow)
            secRow=true;
        else
            row++;
    }
    fin.close();
    
    while(1){
        string input;
        cout<<"\n\nEnter the input string : ";
        cin>>input;

        if(input=="null" && final[0]==0){
            cout<<"Accepted!"<<endl;
        }
        else if(input=="null" && final[0]!=0){
            cout<<"Not Accepted!"<<endl;
        }
        else if(input=="q"){
            cout<<"Quiting...";
            exit(0);
        }
        else{
            int k=0, n=input.length();
            int curr_state = (initial[0]);
            bool invalidToken=false;
            
            while(curr_state!=-1 && k<n){
                int input_token = input[k]-'0';
                k++;
                if(input_token>=noOfCol){
                    invalidToken=true;
                    break;
                }
                curr_state=DFA[curr_state][input_token];
            }
            bool flag=false;
            for(int i=0;i<final.size();i++){
                if(curr_state==final[i]){
                    flag=true;
                    break;
                }
            }
            if(flag && !invalidToken){
                cout<<endl;
                cout<<"Accepted!"<<endl;
            }
            else {
                cout<<"Not Accepted!"<<endl;
            }
        }
    }
    return 0;
}

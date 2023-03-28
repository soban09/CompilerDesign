#include<iostream>
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
#include<unordered_map>
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
    fin.open("mealey.txt");
    int mx=-1, row=0, col=0;
    
    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        int c=0;
        while(ss >> word){
            if(!isalpha(word[0])){
                mx = max(mx, stoi(word));            
            }
            c++;
        }
        col = max(c, col);
        row++;
    }
    fin.close();
    return {max(mx+1, row-1), col};
}

int main(){
    ifstream fin;
    string line;
    
    vector<int> initial;
    auto [noOfRows, noOfCol] = RowsCol();
    vector<vector<int>> mealey(noOfRows, vector<int>(noOfCol,-1));
    unordered_map<string, int> strMap;
    unordered_map<int, string> valMap;
    
    int row=0, val=0;
    bool firstRowDone=false;
    fin.open("mealey.txt");
    
    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        int col=0;
        while(ss >> word){
            if(!firstRowDone){
                initial.push_back(stoi(word));
            }
            else{
                if(word[0]=='-' || isdigit(word[0])){
                    mealey[row][col] = stoi(word);
                }
                else{
                    if(strMap.find(word)==strMap.end()){
                        val++;
                        valMap.insert({val,word});
                        strMap.insert({word,val});
                    }
                    mealey[row][col] = strMap[word];
                }
            }
            col++;
        }
        if(!firstRowDone)
            firstRowDone=true;
        else
            row++;
    }
    fin.close();
    
    while(1){
        string input;
        cout<<"\n\nEnter the input string : ";
        cin>>input;

        if(input=="null"){
            cout<<"Not Accepted!"<<endl;
        }
        else if(input=="q"){
            cout<<"Quiting...";
            exit(0);
        }
        else{
            int k=0, n=input.length();
            int curr_state = initial[0];
            string output;

            while(curr_state!=-1 && k<n){
                int input_token = (input[k]-'0');
                k++;
                if(input_token>=noOfCol/2)
                    break;

                int transition_state = mealey[curr_state][2*input_token];
                if(transition_state==-1)
                    break;
                int op = mealey[curr_state][2*input_token+1];
                // char output_char = op==1 ? 'A' : 'B';
                string output_string = valMap[op];
                output += " " + output_string;
                // output.push_back(output_char);
                curr_state = transition_state;
            }

            cout<<"Output : ";
            for(int i=0; i<output.size(); i++) cout<<output[i];
        }
        
    }
    return 0;
}

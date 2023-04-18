#include "LLParser.h"
#include <stack>
#include <algorithm>

int main(){
    
    run2();

    cout<<"\n\nString validation against LL Parser\nPress q to quit : \n\n";
    
    while(1){
        string str;
        cout<<"Enter a string : ";
        cin>>str;

        if(str=="q"){
            exit(0);
        }

        str.push_back('$');
        
        stack<char> s;
        s.push('$');
        s.push(startingSymbol);
        int i=0;
        bool validation=false;
        
        while(i<str.size()){

            if(colTerminals.find(str[i])==colTerminals.end())
                break;
            
            else if(s.top()=='$' && str[i]=='$'){
                validation=true;
                break;
            }
            
            else if((s.top()=='$' && str[i]!='$')){
                break;
            }
            
            else if(colTerminals.find(s.top())!=colTerminals.end() && colTerminals.find(str[i])!=colTerminals.end()){
                //if both same, pop
                if(s.top()==str[i]){
                    s.pop();
                    i++;
                }
                else break;   
            }

            else{
                if(LLParser[rowVariables[s.top()]][colTerminals[str[i]]].size()==0)
                    break;

                int productionNo = LLParser[rowVariables[s.top()]][colTerminals[str[i]]][0];
                s.pop();
                vector<char> toBePushed = Grammar[productionNo];
                reverse(toBePushed.begin(), toBePushed.end());
                toBePushed.pop_back();
                
                if(toBePushed[0]!='#'){
                    int j=0;
                    while(j<toBePushed.size()){
                        s.push(toBePushed[j]);
                        j++;
                    }
                }
            }
        }
        
        validation ? cout<<"String is ACCEPTED\n\n" : cout<<"String is NOT accepted\n\n";
    }
}
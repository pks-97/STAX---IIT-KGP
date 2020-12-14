#include<iostream>
using namespace std;
char abcd(char in[3],char st1[2],char st2[2]){
    cout<<"st1 in st2"<<endl;
    for(int i=0;i<3;i++){
        if(i==0)
            cout<<"    "<<in[i]<<"   "<<endl;
        else
            cout<<st1[i-1]<<"   "<<in[i]<<"  "<<st2[i-1]<<endl;

    }
}
int main(){
    char out[3];
    for(int i=0;i<3;i++){
        cin>>out[i];
    }
    char in[3],st1[2]={'0','0'},st2[2]={'0','0'};
    for(int i=0;i<3;i++){
        cin>>in[i];
    }
    abcd(in,st1,st2);
    int m=0,n=0,p=0;
    for(int i=0;i<3;i++){
        if(i==0){
            if(out[i]==in[2]){
                st1[1]=in[2];
                in[2]='0';
                abcd(in,st1,st2);
                st2[0]=in[1];
                in[1]='0';
                abcd(in,st1,st2);
                st2[1]=in[0];
                in[0]='0';
                abcd(in,st1,st2);
            }
            if(out[i]==in[1]){
                st2[0]=in[2];
                in[2]='0';
                abcd(in,st1,st2);
                st1[1]=in[1];
                in[1]='0';
                abcd(in,st1,st2);
                st2[1]=in[0];
                in[0]='0';
                abcd(in,st1,st2);
            }
            if(out[i]==in[0]){
                if(out[1]!=in[1]){
                    st2[1]=in[2];
                    in[2]='0';
                    abcd(in,st1,st2);
                    st1[1]=in[1];
                    in[1]='0';
                    abcd(in,st1,st2);

                }
            }
            if(in[0]=='0'){
                in[0]=st1[1];
                st1[1]='0';
                abcd(in,st1,st2);
            }
        }
        if(i==1){
            if(out[i]!=in[i]){
                if(st1[1]=='0'){
                    if(out[1]==st2[1]){
                        in[1]=st2[1];
                        st2[1]='0';
                        abcd(in,st1,st2);
                        in[2]=st2[0];
                        st2[0]='0';
                        abcd(in,st1,st2);
                    }
                    else{
                        st1[1]=st2[1];
                        st2[1]='0';
                        abcd(in,st1,st2);
                        in[1]=st2[0];
                        st2[0]='0';
                        abcd(in,st1,st2);
                        in[2]=st1[1];
                        st1[1]='0';
                        abcd(in,st1,st2);
                    }
                }
                else{
                    in[1]=st1[1];
                    st1[1]='0';
                    abcd(in,st1,st2);
                    in[2]=st2[1];
                    st2[1]='0';
                    abcd(in,st1,st2);
                }
            }
        }

    }
    return(0);
}

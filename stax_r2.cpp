#include<iostream>
using namespace std;
char abcd(char in[6],char st1[4],char st2[4]){
    cout<<"st1 in st2"<<endl;
    for(int i=0;i<6;i++){
        if(i==0 || i==1)
          {
            cout<<"    "<<in[i]<<"   "<<endl;

           }
        else
            cout<<st1[i-2]<<"   "<<in[i]<<"  "<<st2[i-2]<<endl;

    }
}
int main(){
   int q=0,w=0;
     char out[4];
    for(int i=0;i<=3;i++){
        cin>>out[i];
    }
    char in[6],st1[4]={'0','0','0','0',},st2[4]={'0','0','0','0'};
    for(int i=0;i<6;i++){
        cin>>in[i];
    }
    abcd(in,st1,st2);
       for(int i=5;i>=0;i--)
       {
             if(in[i]=='K' || in[i]=='W')
             {
                st1[q]=in[i];
                in[i]='0';
                abcd(in,st1,st2);
                q=q+1;
             }

            else
            {
                st2[w]=in[i];
                in[i]='0';
               abcd(in,st1,st2);
                w=w+1;
            }

       }

    for (int i=3;i>=0;i--)
    {

        if(out[t]==st2[i])
         {
         in[t]=st2[i];
          st2[i]='0';
        abcd(in,st1,st2);
         t=t+1;
        break;
         }
        else
           {
               st1[w]=st2[i];
                 st2[i]='0';
                 abcd(in,st1,st2);
                w=w+1;
           }

    }













































   /* for(int i=0;i<3;i++){
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
            else if(out[i]==in[1]){
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
            else if(out[i]==in[0]){
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

    }*/
    return(0);
}

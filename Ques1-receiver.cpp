#include<bits/stdc++.h>
using namespace std;
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
#define IP_PROTOCOL 0 
#define PORT_NO 15050 
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 
  
// function to clear buffer 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < NET_BUF_SIZE; i++) 
        b[i] = '\0'; 
} 
  
// function to encrypt 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// function sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, nofile); 
        len = strlen(nofile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = Cipher(buf[i]); 
        return 1; 
    } 
  
    char ch, ch2; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
        ch2 = Cipher(ch); 
        buf[i] = ch2; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
} 
  

string CyclicRedundancyCheck(string s,int n,string divisor)
{
    if(divisor.length()>n){
        string rem(divisor.length()-1,'0');
        for(int i=0;i<s.length();++i)
        {
            rem[rem.length()-i-1]=s[s.length()-1-i];
        }
        return s+rem;
    }
    cout<<s<<" "<<divisor<<endl;
    string rem=s.substr(0,divisor.length()-1);
    // cout<<rem<<endl;
    for(int i=divisor.length()-1;i<n;++i)
    {
        string temp=rem+s[i];
        rem="";
        cout<<temp<<endl;
        if(temp[0]=='0')
        {
            for(int j=1;j<temp.length();++j)
            {
                rem=rem+temp[j];
            }
        }
        else
        {   
            for(int j=1;j<temp.length();++j)
            {
                if(divisor[j]=='0')
                    rem=rem+temp[j];
                else{
                    if(temp[j]=='0')
                        rem=rem+"1";
                    else
                        rem=rem+"1";
                }
            }
        }
    }
    return s+rem;
}


int checksum(string s,int n,int seg)
{
    int v=0;
    for(int i=n-seg;i>=0;i-=seg)
    {
        int x=0;
        for(int j=0;j<seg;++j)
        {
            x+=(s[i+j]-'0')*(1<<j);
        }
        v+=x;
        v%=(1<<seg);
    }
    cout<<v<<endl;
    if(v==0)
        return 0;
    else
        return 1;
}
// driver code 
int main() 
{ 
    int sockfd, nBytes; 
    struct sockaddr_in addr_con; 
    unsigned int addrlen = sizeof(addr_con); 
    addr_con.sin_family = AF_INET; 
    addr_con.sin_port = htons(PORT_NO); 
    addr_con.sin_addr.s_addr = INADDR_ANY; 
    char net_buf[NET_BUF_SIZE]; 
    FILE* fp; 
  
    // socket() 
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 
  
    if (sockfd < 0) 
        printf("\nfile descriptor not received!!\n"); 
    else
        printf("\nfile descriptor %d received\n", sockfd); 
  
    // bind() 
    if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0) 
        printf("\nSuccessfully binded!\n"); 
    else
        printf("\nBinding Failed!\n"); 
  
    while (1) { 
        printf("\nWaiting for string\n"); 
   
        clearBuf(net_buf); 
  
        int ty;

        nBytes = recvfrom(sockfd, net_buf, 
                          NET_BUF_SIZE, sendrecvflag, 
                          (struct sockaddr*)&addr_con, &addrlen); 

        ty = stoi(string(net_buf));
        int n;

        clearBuf(net_buf); 
        nBytes = recvfrom(sockfd, net_buf, 
                          NET_BUF_SIZE, sendrecvflag, 
                          (struct sockaddr*)&addr_con, &addrlen); 

        n = stoi(string(net_buf));

        clearBuf(net_buf); 
        int error=0;
        string s;
        switch(ty)
        {
            case 1:
            {
                cout<<"Checker Type Single Parity Checker\n";
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 
                s=string(net_buf);
                int v=0;
                for(int i=0;i<s.length()-1;++i)
                {
                    if(s[i]=='1')
                        v=!v;
                }
                if(v)
                {
                    if(s[s.length()-1]=='0')
                        error=1;
                }
                else
                {
                    if(s[s.length()-1]=='1')
                        error=1;
                }
            }
            break;
            case 2:
            {
                cout<<"Checker Type Two-dimensional Parity Check\n";
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 

                int seg = stoi(string(net_buf));
                int mat[n/seg][seg];
                int a=n/seg;
                int b=seg;

                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 
                s=string(net_buf);
                
                for(int i=0;i<n;++i)
                {
                    if(s[i]=='1')
                        mat[i/seg][i%seg]=1;
                    else
                        mat[i/seg][i%seg]=0;
                }
                int z=0;
                for(int i=0;i<a;++i)
                {
                    int v=0;
                    for(int j=0;j<b;++j){
                        cout<<mat[i][j];
                        v^=mat[i][j];
                    }
                    cout<<"\n";
                    if(s[n+z]!=v+'0')
                        error=1;
                        ++z;
                }
                for(int i=0;i<b;++i)
                {
                    int v=0;
                    for(int j=0;j<a;++j)
                        v^=mat[j][i];
                    if(s[n+z]!=v+'0')
                        error=1;
                        ++z;
                }
            }
            break;
            case 3: 
            {
                cout<<"Checker Type Checksum\n";
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 

                int seg = stoi(string(net_buf));
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 
                s=string(net_buf);
                int val=checksum(s,s.length(),seg);
                if(val==1)
                {
                    error=1;
                }
                else
                    error=0;
            }
            break;
            case 4:
            {
                cout<<"Checker Type Cyclic Redundancy Check(CRC)\n";
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 

                string divisor = string(net_buf);
                clearBuf(net_buf); 
                nBytes = recvfrom(sockfd, net_buf, 
                                  NET_BUF_SIZE, sendrecvflag, 
                                  (struct sockaddr*)&addr_con, &addrlen); 
                s=string(net_buf);
                string val=CyclicRedundancyCheck(s.substr(0,s.length()-divisor.length()+1),s.length()-divisor.length()+1,divisor);
                cout<<val<<endl;
                if(val==s)
                {
                    error=0;
                }
                else
                    error=1;
            }
            break;
        } 
        printf("\nString Received: %s\n", s.c_str()); 

        if(error==0)
        {
            cout<<"NO ERROR DETECTED\n";
        }
        else
        {
            cout<<"ERROR DETECTED\n";
        }
        
    } 
    return 0; 
}
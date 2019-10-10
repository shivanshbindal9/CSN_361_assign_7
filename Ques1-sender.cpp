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
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 15050 
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 
  
// function to clear buffer 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < NET_BUF_SIZE; i++) 
        b[i] = '\0'; 
} 
  
// function for decryption 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// function to receive file 
int recvFile(char* buf, int s) 
{ 
    int i; 
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = buf[i]; 
        ch = Cipher(ch); 
        if (ch == EOF) 
            return 1; 
        else
            printf("%c", ch); 
    } 
    return 0; 
} 

string conv(int val,int seg)
{
    string s=string(seg,'1');
    for(int i=0;i<seg;++i)
    {
        if(val&(1<<(seg-i-1)))
        {
            s[i]='0';
        }
    }
    return s;
}

string checksum(string s,int n,int seg)
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
    s=s+conv(v,seg);
    return s;
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
    string rem=s.substr(0,divisor.length()-1);
    // cout<<rem<<endl;
    for(int i=divisor.length()-1;i<n;++i)
    {
        string temp=rem+s[i];
        rem="";
        if(s[i]=='0')
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

// driver code 
int main() 
{ 
    int sockfd, nBytes; 
    struct sockaddr_in addr_con; 
    unsigned int addrlen = sizeof(addr_con); 
    addr_con.sin_family = AF_INET; 
    addr_con.sin_port = htons(PORT_NO); 
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
    char net_buf[NET_BUF_SIZE]; 
    FILE* fp; 
  
    // socket() 
    sockfd = socket(AF_INET, SOCK_DGRAM, 
                    IP_PROTOCOL); 
  
    if (sockfd < 0) 
        printf("\nfile descriptor not received!!\n"); 
    else
        printf("\nfile descriptor %d received\n", sockfd); 
  
    while (1) { 

        string s;
        
        printf("\nSelect the algorithm \n1.Single Parity Check \n2.Two-dimensional Parity Check\n3. Checksum \n4. Cyclic Redundancy Check(CRC)\n"); 
        
        int choice;
        scanf("%d",&choice);
        
        strcpy(net_buf,to_string(choice).c_str());

        sendto(sockfd, net_buf, NET_BUF_SIZE, 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 

        int n;
        cout<<"Enter length of message :\n";
        cin>>n;

        s=to_string(n);
        strcpy(net_buf,s.c_str());

        sendto(sockfd, net_buf, NET_BUF_SIZE, 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 

        cout<<"Enter message :\n";
        cin>>s;
        switch(choice)
        {
            case 1: 
            {
                int v=0;
                for(int i=0;i<s.length();++i)
                {
                    char r=s[i];
                    if(r=='1')
                        v=!v;
                }
                if(v)
                    s=s+"1";
                else
                    s=s+"0";
            }
            break;
            case 2:
            {
                cout<<"Enter length of segments \n";
                int seg;
                cin>>seg;
                string ss=to_string(seg);
                strcpy(net_buf,ss.c_str());

                sendto(sockfd, net_buf, NET_BUF_SIZE, 
                       sendrecvflag, (struct sockaddr*)&addr_con, 
                       addrlen); 
                int mat[n/seg][seg];
                int a=n/seg;
                int b=seg;
                for(int i=0;i<n;++i)
                {
                    if(s[i]=='1')
                        mat[i/seg][i%seg]=1;
                    else
                        mat[i/seg][i%seg]=0;
                }
                cout<<"Matrix prepared\n";
                for(int i=0;i<a;++i)
                {
                    int v=0;
                    for(int j=0;j<b;++j){
                        cout<<mat[i][j];
                        v^=mat[i][j];
                    }
                    cout<<"\n";
                    if(v==1)
                        s=s+"1";
                    else
                        s=s+"0";
                }
                for(int i=0;i<b;++i)
                {
                    int v=0;
                    for(int j=0;j<a;++j)
                        v^=mat[j][i];
                    if(v==1)
                        s=s+"1";
                    else
                        s=s+"0";
                }
            }
            break;
            case 3: 
            {
                cout<<"Enter length of segment \n";
                int seg;
                cin>>seg;
                string ss=to_string(seg);
                strcpy(net_buf,ss.c_str());

                sendto(sockfd, net_buf, NET_BUF_SIZE, 
                       sendrecvflag, (struct sockaddr*)&addr_con, 
                       addrlen); 
                s=checksum(s,n,seg);
            }
            break;
            case 4:
            {
                cout<<"Enter divisor \n";
                string seg;
                cin>>seg;

                strcpy(net_buf,seg.c_str());

                sendto(sockfd, net_buf, NET_BUF_SIZE, 
                       sendrecvflag, (struct sockaddr*)&addr_con, 
                       addrlen); 
                s=CyclicRedundancyCheck(s,n,seg);
            }
            break;

            default: 
            printf("\nInvalid Choice\n"); 
            continue;
        }
        cout<<"Message prepared by sender\n";
        cout<<s<<endl;
        cout<<"Enter choice\n1. Random Error\n2. Manual Error\n";
        int ty;
        cin>>ty;
        if(ty==1)
        {
            for(int i=0;i<s.length();++i)
            {
                if(rand()%2)
                {
                    if(s[i]=='1')
                        s[i]='0';
                    else
                        s[i]='1';
                }
            }
        }
        else
        {
            int d;
            cout<<"Enter number of errors:\n";
            cin>>d;
            if(d)
            cout<<"Enter index of errors (1 indexing) :\n";
            while(d)
            {
                --d;
                int x;
                cin>>x;
                --x;
                if(s[x]=='1')
                    s[x]='0';
                else
                    s[x]='1';
            }
        }
        strcpy(net_buf,s.c_str());

        sendto(sockfd, net_buf, NET_BUF_SIZE, 
               sendrecvflag, (struct sockaddr*)&addr_con, 
               addrlen); 
  
        printf("\n---------Data Sent---------\n"); 
  
        printf("%s", net_buf );
        
        printf("\n-------------------------------\n"); 

    } 
    return 0; 
} 
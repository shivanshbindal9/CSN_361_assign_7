#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <math.h>
#include <stdlib.h> 
#include <time.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock=0,valread; 
    struct sockaddr_in serv_addr;  
    char buffer[1024]={0};
    if ((sock=socket(AF_INET,SOCK_STREAM,0))<0) {
        printf("\n Socket creation error \n"); 
        return -1; 
    }
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_port=htons(PORT); 
    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    int m,data_bits[1000],r=0,parity;
    printf("\nN: ");
    scanf("%d",&m); 
    char bol[1024];
    printf("\nM: ");
    scanf("%s",bol+1);
    for(int i=1; i<=m; i++)
    	data_bits[m+1-i]=bol[i]-'0';
    while((1<<r)<m+r+1)
        r++;
    // printf("Number of redundant bits: %d",r);
    int hamming[m+r+1],k=1;
    for(int i=1; i<=m+r; i++)
        if((i&(i-1))==0)
            hamming[i]=0;
        else
            hamming[i]=data_bits[k++];
    for(int i=0; i<r; i++) {
        int x=1<<i;
        for(int j=1; j<=m+r; j++)
            if((j>>i)&1)
                if(x!=j)
                    hamming[x]^=hamming[j];
    }
    int finhamming[m+r+1];
    printf("\nM(after adding parity): ");
    for(int i= 0; i<m+r; i++) {
        finhamming[i]=hamming[m+r-i];
        printf("%d",finhamming[i]);
    }
    
    printf("\nEnter 0 for Manual error");
    printf("\nEnter 1 for random errors");
    printf("\nEnter 2 for no errors\n");
	int res;
    scanf("%d", &res);
    if(res==0){
        printf("\nPress number of bits to be flipped :");
        int num;
        scanf("%d", &num);
        printf("\nEnter indices of the bits to be flipped\n");
        for (int i =0; i< num; i++){
            int pos;
            scanf("%d",&pos);
            finhamming[pos]^=1;
        }
    }
    if(res==1) {
        int index=rand()%(m+r);
        printf("\n%dth Bit is flipped\n", index);
        finhamming[index]^=1;
    }
    printf("\nM transmitted (after adding errors): ");
    for(int i= 0; i<m+r; i++)
        printf("%d",finhamming[i]);
    char message[m+r];
    for(int i= 0; i<m+r; i++)
        message[i]=(finhamming[i]+'0');
    send(sock,message,m+r,0); 
    printf("\nMessage sent\n");
    return 0; 
} 
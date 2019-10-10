#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

char vald[1024];

char correct(char buffer[], int len) {
	int r=0;
    while((1<<r)<(len+1))
        r++;
    printf("%d\n",r);
    int ul=0;
    for(int i=0; i<r; i++) {
        char parity=0;
        for(int j=1; j<=len; j++)
            if(j&(1<<i))
                parity^=buffer[j-1]-'0';
        if(parity)
            ul|=(1<<i);
    }
    printf("%d %d",ul,len);
    if(ul!=0) {
        buffer[ul-1]='1'+'0'-buffer[ul-1];
        return 1;
    } else if(ul>len)
        return 2;
    else
        return 0;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt=1;
    int addrlen=sizeof(address); 
    char buffer[1024]={0};
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))) {
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family=AF_INET; 
    address.sin_addr.s_addr=INADDR_ANY; 
    address.sin_port=htons( PORT ); 
    if(bind(server_fd, (struct sockaddr*)&address,sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    if(listen(server_fd,3)<0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    if((new_socket=accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))<0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Listening for encoded message");
    valread=read( new_socket , buffer, 1024);
    printf("%d\n",valread);

    printf("Message received is : %s\n",buffer ); 
    int te=correct(buffer,valread);
    if(te==0)
        printf("No error detected!\n");
    else if(te==1)
        printf("Message after correction is : %s\n",buffer ); 
    else {
        printf("Message is too corrupted for recovery \n");
        return 0;
    }
    printf("Original message is: ");
    char original[1024];
    int j=0;
    for(int i=1; i<=valread; i++) {
        if((i&(i-1))) {
            original[j]=buffer[valread-i];
            j++;
        }
    }
    for(int i=0; i<j; i++)
        printf("%c",original[j-1-i]);
    printf("\n");
    return 0; 
} 
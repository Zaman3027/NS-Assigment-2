#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>

#define BUFFER_SIZE 1024
#define PORT 8080

#define SERVER_N "client_n.key"
#define SERVER_V "client_v.key"

void receive();
void writeFile(FILE *fptr, char *buffer,char *fileName);

int temp_sock_desc = 0;

int main() {
    char buf[BUFFER_SIZE];
    int k;
    socklen_t len;
    int sock_desc;
    struct sockaddr_in server,client;

    memset(&server,0,sizeof(server));
    memset(&client,0,sizeof(client));

    sock_desc=socket(AF_INET,SOCK_STREAM,0);
    if(sock_desc==-1) {
        printf("Error in socket creation");
        exit(1);
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=PORT;

    k=bind(sock_desc,(struct sockaddr*)&server,sizeof(server));
    if(k==-1) {
        printf("Error in binding");
        exit(1);
    }

    k=listen(sock_desc,20);
    if(k==-1){
        printf("Error in listening");
        exit(1);
    }

    len=sizeof(client);//VERY IMPORTANT
    temp_sock_desc=accept(sock_desc,(struct sockaddr*)&client,&len);
    if(temp_sock_desc==-1){
        printf("Error in temporary socket creation");
        exit(1);
    }
    receive();
    exit(0);
    return 0;
}

void receive() {
    FILE *sPublic;
    int k;
    char buffer[BUFFER_SIZE];
    while (1) {
        k=recv(temp_sock_desc,buffer,BUFFER_SIZE,0);
        printf("%c \n",buffer[0]);
        int a = 0;

        switch (buffer[0]) {
        case '1':
           recv(temp_sock_desc,buffer,BUFFER_SIZE,0);
           writeFile(sPublic,buffer,SERVER_N);
           printf("Public key n %s\n",buffer);
           recv(temp_sock_desc,buffer,BUFFER_SIZE,0);
           writeFile(sPublic,buffer,SERVER_V);
           printf("Public key v %s\n",buffer);
           break;
        case '2':
           //TODO: Authentication 
           printf("Authentication \n");
           break;
        case '3':
           //TODO: Update Key
           printf("Update Key\n");
           break;
        case '4':
           //TODO: End the application
           exit(1);
           break;

        default:
            break;
        }
    }
    
}

void writeFile(FILE *fptr, char *buffer,char *fileName) {
    fptr = fopen(fileName,"w");
    if(fptr==NULL) {
        printf("Unable to write file %s\n",fileName);
        exit(1);
    }
    fputs(buffer,fptr);
    fclose(fptr);
}
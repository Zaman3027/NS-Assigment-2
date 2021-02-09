#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER_SIZE 1024
#define PORT 8080

void *receive();

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
    if(sock_desc==-1)
    {
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

    pthread_t thread_id;
    if(pthread_create(&thread_id,NULL,receive,NULL)) {
        printf("ERROR : Creating thread \n");
        exit(1);
    }

    while(fgets(buf,BUFFER_SIZE,stdin)!=NULL) {
        if(strncmp(buf,"end",3)==0)
            break;

        k=send(temp_sock_desc,buf,BUFFER_SIZE,0);
        if(k==-1) {
            printf("Error in sending");
            exit(1);
        }
    }
    pthread_join(thread_id,(void **)NULL);
    pthread_exit(NULL);
    exit(0);
    return 0;
}

void *receive() {
    int k;
    char buf[BUFFER_SIZE];
    while (1) {
        k=recv(temp_sock_desc,buf,BUFFER_SIZE,0);
        if(k==-1) {
            printf("Error in receiving");
            exit(1);
        }

        printf("Message got from server is : %s",buf);
    }
    
}
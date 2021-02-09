#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int sock_desc;

void *receive();
int main()
{
    char buf[BUFFER_SIZE];
    int k;
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    if(sock_desc==-1) {
        printf("Error in socket creation");
        exit(1);
    }

    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;
    client.sin_port=PORT;

    k=connect(sock_desc,(struct sockaddr*)&client,sizeof(client));
    if(k==-1)
    {
        printf("Error in connecting to server");
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

        k=send(sock_desc,buf,BUFFER_SIZE,0);
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
        k=recv(sock_desc,buf,BUFFER_SIZE,0);
        if(k==-1) {
            printf("Error in receiving");
            exit(1);
        }

        printf("Message got from server is : %s",buf);
    }
    
}
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include "../miracl.h"
#define BUFFER_SIZE 1024
#define PORT 8080
#define CLIENT_N "client_n.key"
#define CLIENT_S "client_s.key"
#define CLIENT_V "client_v.key"

int sock_desc;

big generateKey();
big generateSprivate(big key);
big generateVpublic(big key, big s);

void writeFile(FILE *fptr, char *buffer,char *fileName);

int main() {
    FILE * keyFile;
    FILE * sFile;
    //init client setup
    char buffer[BUFFER_SIZE];
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
    if(k==-1) {
        printf("Error in connecting to server");
        exit(1);
    }
    //End client set-up

    //MIRACL set-up
    miracl *mir = mirsys(1024,10);
    big key,s,v;
    key = mirvar(0);
    s = mirvar(0);
    v = mirvar(0);

    //Client - Server data exchange
    while (1) {
        fgets(buffer,BUFFER_SIZE,stdin);
        send(sock_desc,buffer,BUFFER_SIZE,0);
        switch (buffer[0]) {
            case '1':
            key = generateKey();
            s = generateSprivate(key);
            v = generateVpublic(key,s);
            otstr(key,buffer);
            send(sock_desc,buffer,BUFFER_SIZE,0);
            printf("Register key %s\n",buffer);
            otstr(s,buffer);
            printf("Private S key %s\n",buffer);
            otstr(v,buffer);
            send(sock_desc,buffer,BUFFER_SIZE,0);
            printf("Public V key %s\n",buffer);
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
        //End Client - server data exchange
    }

    exit(0);
    return 0;
}

big generateKey() {
    irand(2625);
    big prime = mirvar(0);
    big generator = mirvar(0);
    bigbits(32,prime);
    bigbits(3,generator);
    nxprime(prime,prime);
    nxprime(generator,generator);
    big key = mirvar(0);
    multiply(prime,generator,key);
    FILE *keyFile;
    char buffer[BUFFER_SIZE];
    otstr(key,buffer);
    writeFile(keyFile,buffer,CLIENT_N);
    return key;
}

big generateSprivate(big key) {
    big s = mirvar(0);
    big t = mirvar(0);
    big z = mirvar(0);
    big o = mirvar(0);
    convert(0,z);
    convert(1,o);
    do{
        bigrand(key,s);
        egcd(s,key,t);
    }while (mr_compare(s,z)==0|| 
    mr_compare(s,o)==0||
    mr_compare(t,o)!=0);
    FILE *sFile;
    char buffer[BUFFER_SIZE];
    otstr(s,buffer);
    writeFile(sFile,buffer,CLIENT_S);
    return s;
}

big generateVpublic(big key, big s) {
    big v = mirvar(0);
    powmod(s,mirvar(2),key,v);
    FILE *sFile;
    char buffer[BUFFER_SIZE];
    otstr(v,buffer);
    writeFile(sFile,buffer,CLIENT_V);
    return v;
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
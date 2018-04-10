#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MY_PORT 9999
#define MY_IP "127.0.0.1"
#define BUF 500

pthread_t receiver, emitter;

int FLAG = 0;

void *receiveMsg() {
    struct sockaddr_in local_addr;
    struct sockaddr_in connection_addr;
    int  my_socket, my_connection;
    char buffer[BUF];
    socklen_t leng;
    leng = sizeof(struct sockaddr_in);

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(MY_IP);
    local_addr.sin_port = htons(MY_PORT);
    bzero(&(local_addr.sin_zero), 8);
    
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
        
    bind(my_socket, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));
    listen(my_socket, 1);        

    my_connection = accept(my_socket, (struct sockaddr *)&connection_addr, &leng);
    printf("Uma conexao do end %s foi estabelecida! \n", inet_ntoa(connection_addr.sin_addr));

    do {
        memset(buffer, 0, sizeof(buffer));
        int flag = recv((my_connection), buffer, 500, 0);
        if(flag != -1 && flag != 0) {
            printf(" msg: %s \n", buffer);
        }
        else {
            FLAG = 1;
        }
    } while (FLAG == 0);
    close(my_connection);
    return NULL;
}

void *sendMsg() {
    char str[500], host_ip[15];
    int host_port, host_socket;
    struct sockaddr_in server_addr;

    printf("Digite o ip do destinatario \n");
	scanf("%s",host_ip);
	printf("Digite a porta do destinatario \n");
	scanf("%d", &host_port);

	host_socket = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(host_ip);
	server_addr.sin_port = htons(host_port);
	bzero(&(server_addr.sin_zero), 8);

	connect((host_socket),(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    do {
        __fpurge(stdin);
	    gets(str);
	    send((host_socket), str,strlen(str),0);
        if(strcmp(str, "quit") == 0) {
            FLAG = 1;
        }
    } while (FLAG == 0);
    close(host_socket);
    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_create(&receiver, NULL, &receiveMsg);
	pthread_create(&emitter, NULL, &sendMsg);
	
	pthread_join(receiver, NULL);
	pthread_join(emitter, NULL);
	
	exit(0);
}

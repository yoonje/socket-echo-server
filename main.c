#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   1024

int main(void) {
    int server_socket;
    int client_socket;
    int client_addr_size;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char buff_rcv[BUFF_SIZE + 5];
    char buff_snd[BUFF_SIZE + 5];


    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == server_socket) {
        printf("server socket 생성 실패\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        printf("bind() 실행 에러\n");
        exit(1);
    }

    if (-1 == listen(server_socket, 5)) {
        printf("listen() 실행 실패\n");
        exit(1);
    }

    while (1) {

        memset(buff_rcv, 0, sizeof(server_addr));
        memset(buff_snd, 0, sizeof(server_addr));

        memset(&client_addr, 0, sizeof(server_addr));
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_size);

        if (-1 == client_socket) {
            printf("클라이언트 연결 수락 실패\n");
            close(server_socket);
            exit(1);
        }

        while(1) {
            if(read(client_socket, buff_rcv, BUFF_SIZE)==0){
                break;
            }
            printf("receive: %s\n", buff_rcv);
        }
        close(client_socket);
    }
}
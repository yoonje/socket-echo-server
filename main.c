#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
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


    server_socket = socket(PF_INET, SOCK_STREAM, 0); // PF_INET은 프로토콜 체계에서 IPv4, SOCK_STREAM은 TCP를 의미
    if (-1 == server_socket) {
        printf("server socket 생성 실패\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr)); // 0으로 초기화
    server_addr.sin_family = AF_INET; // AF_INET은 주소 체계에서 IPv4를 의미
    server_addr.sin_port = htons(4000); // htons 함수를 통해서 Little Endian일 경우 Big Endian으로 변경하여 포트 번호 대입
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY로 설정하여 모든 IP의 요청을 받음

    if (-1 == bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        printf("bind() 실행 에러\n");
        exit(1);
    }

    if (-1 == listen(server_socket, 5)) {
        printf("listen() 실행 실패\n");
        exit(1);
    }

    while (1) { // 죽지 않는 서버를 만들기 위해 Outer 루프

        memset(buff_rcv, 0, sizeof(buff_rcv)); // 0으로 초기화
        memset(buff_snd, 0, sizeof(buff_snd)); // 0으로 초기화

        memset(&client_addr, 0, sizeof(client_addr)); // 0으로 초기화
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_size); // 클라이언트의 요청을 받아들임

        if (-1 == client_socket) { // accept 함수가 -1을 리턴한 경우 클라이언트와의 연결 실패
            printf("클라이언트 연결 수락 실패\n");
            close(server_socket);
            exit(1);
        }

        while (1) { // 클라이언트의 요청을 반복적으로 받기 위한 Inner 루프
                    // Inner 루프가 없는 경우 서버가 죽음
            if (read(client_socket, buff_rcv, BUFF_SIZE) == 0) { // 읽어온 데이터가 없는 경우 read 함수가 0 반환
                break;
            }
            printf("receive: %s\n", buff_rcv);
        }
        close(client_socket);
    }
}
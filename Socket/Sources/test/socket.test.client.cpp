#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    const char* hostname = "127.0.0.1";
    const char* port = "8080";
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo 오류: " << gai_strerror(status) << std::endl;
        return 1;
    }
    
    int clientSocket = -1;
    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "시도 중인 주소: " << ipstr << std::endl;
        
        clientSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (clientSocket == -1) {
            std::cerr << "Error: 클라이언트 소켓 생성 실패 - " << strerror(errno) << std::endl;
            continue;
        }
        
        status = connect(clientSocket, p->ai_addr, p->ai_addrlen);
        if (status == -1) {
            std::cerr << "Error: 서버 연결 실패 - " << strerror(errno) << std::endl;
            close(clientSocket);
            clientSocket = -1;
            continue;
        }
        
        std::cout << "서버에 성공적으로 연결되었습니다 (" << ipstr << ")" << std::endl;
        break;
    }
    
    if (clientSocket == -1) {
        std::cerr << "모든 주소에 대한 연결 시도 실패" << std::endl;
        freeaddrinfo(res);
        return 1;
    }
    
    char buf[1024];
    while (1) {
        std::cout << "서버에 보낼 메시지를 입력하세요 (종료하려면 'quit' 입력): ";
        std::string message;
        std::getline(std::cin, message);
        
        if (std::cin.eof()){
            break;
        }
        
        if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
            std::cerr << "send: " << strerror(errno) << std::endl;
            break;
        }
        
        memset(buf, 0, sizeof(buf));
        int recv_len = recv(clientSocket, buf, sizeof(buf), 0);
        if (recv_len == -1) {
            std::cerr << "recv: " << strerror(errno) << std::endl;
            break;
        }
        if (recv_len == 0) {
            std::cout << "서버 연결 종료" << std::endl;
            break;
        }
        
        std::cout << "서버로부터 받은 메시지: " << buf << std::endl;
    }
    
    close(clientSocket);
    freeaddrinfo(res);
    return 0;
}
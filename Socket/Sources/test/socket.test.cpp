#include <iostream>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include "../../Headers/Socket.hpp"

int main() {
    try {
        Socket server("localhost", "8080");
        server.autoActiveSock();
        char buf[1024];
        std::cout << "서버 " << server.getServerIP() << ":" << server.getPort() << "에서 대기중..." << std::endl;
        
        while (1) {
            int clientSocket = server.accept();
            if (clientSocket == -1) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    continue;
                }
                std::cerr << "accept: " << strerror(errno) << std::endl;
                continue;
            }
            if (clientSocket == 0) {
                continue;
            }
            
            std::cout << "클라이언트 연결됨" << std::endl;
            std::cout << "클라이언트 IP: " << server.getClientIP() << std::endl;
            
            while (1) {
                memset(buf, 0, sizeof(buf));
                int recv_len = server.recv(buf, sizeof(buf), 0);
                if (recv_len == -1) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        usleep(10000);
                        continue;
                    }
                    std::cerr << "recv: " << strerror(errno) << std::endl;
                    break;
                }
                if (recv_len == 0) {
                    std::cout << "클라이언트 연결 종료" << std::endl;
                    break;
                }
                
                std::cout << "클라이언트로부터 받은 메시지: " << buf << std::endl;
                
                std::string response = "서버: " + std::string(buf);
                if (send(clientSocket, response.c_str(), response.length(), 0) == -1) {
                    std::cerr << "send: " << strerror(errno) << std::endl;
                    break;
                }
            }
            
            close(clientSocket);
        }
    } catch (const Socket::SocketException& e) {
        std::cerr << "SocketException: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
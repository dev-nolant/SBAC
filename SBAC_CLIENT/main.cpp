#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int resultCode = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (resultCode != 0) {
        std::cout << "WSAStartup failed: " << resultCode << std::endl;
        return 1;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("localhost"); // Removed port from here

    SockAddr.sin_port = htons(8080); // Correct port number
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        std::cout << "Could not connect" << std::endl;
        return 1;
    }

    // Adjusted GET request for /detections endpoint
    const char* httpRequest = "GET /detections HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    send(Socket, httpRequest, strlen(httpRequest), 0);

    char buffer[10000];
    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, sizeof(buffer), 0)) > 0) {
        int i = 0;
        while (i < nDataLength) {
            std::cout << buffer[i];
            i++;
        }
    }

    closesocket(Socket);
    WSACleanup();

    return 0;
}

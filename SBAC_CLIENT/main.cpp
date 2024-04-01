#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "lib/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int resultCode = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (resultCode != 0) {
        cout << "WSAStartup failed: " << resultCode << endl;
        return 1;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("localhost");

    SockAddr.sin_port = htons(8080);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        cout << "Could not connect" << endl;
        return 1;
    }

    const char* httpRequest = "GET /detections HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    send(Socket, httpRequest, strlen(httpRequest), 0);

    string response;
    char buffer[512];
    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, nDataLength);
    }

    closesocket(Socket);
    WSACleanup();

    // Find the start of the JSON part (skip HTTP headers)
    auto jsonStartPos = response.find("\r\n\r\n");
    if (jsonStartPos != string::npos) {
        try {
            auto jsonResponse = json::parse(response.substr(jsonStartPos + 4)); // Parse JSON

            for (const auto& item : jsonResponse) {
                for (auto& element : item.items()) {
                    cout << element.key() << ": " << element.value() << endl;
                }
                cout << "-----------------------" << endl; // Separator between objects
            }
        } catch (const json::parse_error& e) {
            cout << "Parsing error: " << e.what() << "\n";
        }
    }

    return 0;
}
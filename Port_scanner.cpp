// Port_scanner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool scanPort(string hostname, int port) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    struct addrinfo hints, * res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname.c_str(), to_string(port).c_str(), &hints, &res);
    if (status != 0) {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        WSACleanup();
        return false;
    }

    SOCKET sock = INVALID_SOCKET;
    for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) {
            continue;
        }

        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(res);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }
    else {
        closesocket(sock);
        WSACleanup();
        return true;
    }
}

int main() {
    string hostname;
    int start_port, end_port;

    cout << "Enter hostname: ";
    cin >> hostname;
    cout << "Enter start port: ";
    cin >> start_port;
    cout << "Enter end port: ";
    cin >> end_port;

    for (int i = start_port; i <= end_port; i++) {
        if (scanPort(hostname, i)) {
            cout << "Port " << i << " is open" << endl;
        }
    }
    return 0;
}
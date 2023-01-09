#include "PSocketServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#include "../../1Base/Aspect/Exception.h"
#include "PSocketServerWorker.h"
#undef GetClassName

PSocketServer::PSocketServer(int nClassId, const char* pcClassName)
    : PComponent(nClassId, pcClassName)
{
}

PSocketServer::~PSocketServer() {
}

void PSocketServer::Initialize() {
    PComponent::Initialize();
}
void PSocketServer::Finalize() {
    PComponent::Finalize();
}

void PSocketServer::Start() {
    PComponent::Start();
}

void PSocketServer::Pause() {
    PComponent::Pause();
}

void PSocketServer::Stop() {
    PComponent::Stop();
}

// https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-socket-for-the-server

void PSocketServer::Run() {
    PComponent::Run();
    LOG("PSocket::Run");

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
    }

    while (this->GetEState() == ComponentPart::EState::eRunning) {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
        }

        PSocketServerWorker* pPSocketServerWorker = new("PSocketServerWorker") PSocketServerWorker(ClientSocket);
        this->AddPart(pPSocketServerWorker->GetObjectId(), pPSocketServerWorker);
        pPSocketServerWorker->Start();

    }
    // No longer need server socket
    closesocket(ListenSocket);
    WSACleanup();
}
#include "PSocketServerWorker.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include "../../1Base/Aspect/Exception.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#undef GetClassName

PSocketServerWorker::PSocketServerWorker(unsigned ClientSocket, int nClassId, const char* pcClassName)
    : PComponentPart(nClassId, pcClassName)
    , ClientSocket(ClientSocket)
    , pRSTP(nullptr)
{
}

PSocketServerWorker::~PSocketServerWorker() {
}

void PSocketServerWorker::Initialize() {
    PComponentPart::Initialize();
}
void PSocketServerWorker::Finalize() {
    PComponentPart::Finalize();
}

void PSocketServerWorker::Start() {
    PComponentPart::Start();
}

void PSocketServerWorker::Stop() {
    PComponentPart::Stop();
}

void PSocketServerWorker::Pause() {
    PComponentPart::Pause();
}

// https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-socket-for-the-server

void PSocketServerWorker::Run() {
    PComponentPart::Run();
    LOG("PSocket::Run");

    int iResult;
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
    }

    // cleanup
    closesocket(ClientSocket);
}
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }
    
    char Nickname[64];
    char color[3];
    char Message[receiveBufSize];
    int j = 0;
    int nicksize = 0;

    receiveBuf[bytesReceived] = '\0';

    int turn = 0;

    for (int i = 0; i < strlen(receiveBuf); i++)
    {
        if (receiveBuf[i] != ' ' && turn == 0)
        {
            Nickname[i] = receiveBuf[i];
            nicksize++;
        }
        if (receiveBuf[i] == ' ' && turn == 0 || turn == 1)
        {
            turn++;
            i++;
        }
        if (receiveBuf[i]!=' ' && turn == 1)
        {
            color[turn - 1] = receiveBuf[i];
        }
        if (turn == 2)
        {
            Message[j] = receiveBuf[i];
            j++;
        }
    }

    COORD point = { 0,1 };
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


    Message[j] = '\0';
    Nickname[nicksize] = '\0';
    color[2] = '\0';

    std::string temp = color;

    int IColor = std::stoi(temp);

    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, IColor);


    char str[15];
    sprintf_s(str, 15, "color %s\0", color);

    //system(str);

    cout << Nickname << ' ' << Message << '\n';

    point.X += strlen(Message);
    point.X += strlen(Nickname);
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, 7);
    const size_t sendBufSize = 1024;
    char sendBuf[sendBufSize] = "Hello from UDPExample";

    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }


    closesocket(udpSocket);
    WSACleanup();
}


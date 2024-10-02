#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    //** stores informatin abount winsock lib implementatiion */
    WSADATA WSAData;

    // ** initialize winsock lib  that i am using 2.2 version and &wsaData is a structure whick will store information about winsock lib implementation*/

    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        std::cerr << "Winsock initialization failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // ** Bind the socket to an IP address and port */

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8000);

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Socket binding failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if(listen(serverSocket,SOMAXCONN) == SOCKET_ERROR){
        std::cerr<<"Socket listening failed."<<std::endl;
        closesocket(serverSocket);
        WSACleanup();
    }

    std::cout<<"Server is listening on port 8080."<<std::endl;

    // ** Accept the incoming request 

    SOCKET clientSocket; // initialize a new socket to store the client socket
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket,(sockaddr*)&clientAddr,&clientAddrSize);

    if(clientSocket == INVALID_SOCKET){
        std::cerr<<"Socket accepting failed."<<std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }


    // ** perparing the http response */

    const char * httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>";

    send(clientSocket,httpResponse,strlen(httpResponse),0);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
    
}
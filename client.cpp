#include <iostream>
#include <udt.h>
#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;
using namespace UDT;

int main() {
    UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6890);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);   // server address here
    memset(&(serv_addr.sin_zero), '\0', 8);

    // selecting random local port
    srand(time(NULL));
    int myPort = 9001 + rand() % 200;
    printf("my port: %d\n", myPort);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myPort);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);


    // Connecting to server

    // binding my port
    if (UDT::ERROR == UDT::bind(client, (sockaddr*)&my_addr, sizeof(my_addr))) {
        cout << "bind error: " << UDT::getlasterror().getErrorMessage();
        return 0;
    }

    // connect to the server
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        cout << "connect error: " << UDT::getlasterror().getErrorMessage();
        return 42;
    }

    char data[6];
    if (UDT::ERROR == UDT::recv(client, data, 6, 0)) {
        cout << "recv error:" << UDT::getlasterror().getErrorMessage() << endl;
        return 0;
    }

    sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = *(uint32_t*)data;
    peer_addr.sin_port = *(uint16_t*)(data + 4);

    cout << "addr received: " << inet_ntoa(peer_addr.sin_addr) << ":" << ntohs(peer_addr.sin_port) << endl;
    UDT::close(client);

    client = UDT::socket(AF_INET, SOCK_STREAM, 0);
    bool rendezvous = true;
    UDT::setsockopt(client, 0, UDT_RENDEZVOUS, &rendezvous, sizeof(bool));

    if (UDT::ERROR == UDT::bind(client, (sockaddr*)&my_addr, sizeof(my_addr))) {
        cout << "bind error: " << UDT::getlasterror().getErrorMessage();
        return 0;
    }

    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&peer_addr, sizeof(peer_addr))) {
        cout << "connect error: " << UDT::getlasterror().getErrorMessage();
        return 42;
    }
    cout << "SUCCESS!\n";
    sleep(2); // give another client time to connect too

    return 1;
}

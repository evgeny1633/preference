//client // Все комментарие находятся на сервере
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

using namespace std;

char message[] = "Hello there!\n";
char buf[sizeof(message)];

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        cout << strerror(errno) << endl;
        return 99;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    //addr.sin_addr.s_addr = inet_addr("89.249.169.99");
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cout << strerror(errno) << endl;
        return 98;
    }

    send(sock, message, sizeof(message), 0);
    recv(sock, buf, sizeof(message), 0);

    cout << "buf = " << buf << endl;
    close(sock);

    return 0;
}

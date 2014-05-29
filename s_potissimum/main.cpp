//server
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

using namespace std;

int main()
{
    int sock, listener;
    struct sockaddr_in addr; //Структура, описывающая сокет для работы с протоколами IP
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0); //Создание сокета. Домен, тип(STRAM или DGRAM. непрерывный поток или отдельные пакеты. первый тип гарантирует доставку - второй нет), протокол(0 = автоматический выбор). Есть ещё RAW - он для низкоуровневых протоколов
    if(listener < 0) //функция socket возвращает номер сокета или -1 в случае неудачи
    {
        cout << strerror(errno) << endl;
        return 99;
    }

    addr.sin_family = AF_INET; //Область работы af_unix или af_inet - для работы на одной машине или в интернете соответсвенно (ничего общего с линухом, как я понял)
    addr.sin_port = htons(3425); //Адрес порта (0 - автоматический выбор оного). htons переводит число из локального формата к сетевому. host short
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //Здесь вводится ip адрес. htonl тот же htons, только long. Вместо адреса можно использовать одну из констат INADDR_ANY = 0.0.0.0, INADDR_LOOPBACK = 127.0.0.1, INADDR_BRODCAST = 255.255.255.255
    //addr.sin_addr.s_addr = inet_addr("255.255.255.255"); //пример команды, переводящей такой вид ip адреса в требуемый
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) //bind связывает сокет с адресом. В качестве аргументов выступают: дескриптор сокета, указатель на структуру с адресом и длинна структуры
    {
        cout << strerror(errno) << endl;
        return 98;
    }

    listen(listener, 1); //создаёт очередь запросов. параметры: дескриптор и размер очереди

    while(true)// бесконечный цикл
    {
        sock = accept(listener, NULL, NULL);// accept создаёт новый сокет для общения с клиентом и возвращает его дескриптор. Второй аргумент содержит сведения об адресе клиента, а третий размер второго, как в bind. Если эта информация не нужна можно вписать NULL, NULL
        if(sock < 0)
        {
            cout << strerror(errno) << endl;
            return 97;
        }

        while(true)// просто бесконечный цикл
        {
            bytes_read = recv(sock, buf, 1024, 0);// то же, что и send
            if(bytes_read <= 0) break;
            send(sock, buf, bytes_read, 0); //дескриптор, указатель на буфер с данными, длинна буфера в байтах и набор битовых флагов. возвращает -1 в случае неудачи
            cout << "messenge sent" << endl;
        }

        close(sock); //закрыть соединение
    }

    return 0;
}

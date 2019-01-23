#include "net/TcpServer.h"
#include <string.h>
#include <iostream>
#include "log.h"

using namespace Cook;

int main(int argc,char **argv)
{
    if(argc < 2)
    {
        std::cout << " args too few " << std::endl;
        return 0;
    }
    int32_t thread_num = atoi(argv[1]);
    sockaddr_in ina;
    memset(&ina, 0,sizeof ina);
    ina.sin_family=AF_INET;
    ina.sin_port=htons(8080);
    ina.sin_addr.s_addr = inet_addr("127.0.0.1");  
    LogMgr::Instance().Register(0);
    TcpServer server(ina,thread_num);
    server.Start();
    return 0;
}
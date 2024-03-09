#include <iostream>
#include "Socket.hpp"
void Usage(std::string proc)
{
    std::cout<<"\n\rUsage: "<<proc<<" port[1024+]\n"<<std::endl;
}
//./tcpclient ip port
int main(int args,char* argv[])
{
    if(args!=3)
    {
     Usage(argv[0]);
     exit(1);
    }
    uint16_t serverport=std::stoi(argv[2]);
    std::string serverip=argv[1];

    Sock sockfd;
    sockfd.Socket();//创建套接字
    bool r=sockfd.Connect(serverip,serverport);//发起连接
    if(!r)return 1;

}
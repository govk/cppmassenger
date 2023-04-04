#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;

int main()
{
  int client = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in seraddr;
  memset(&seraddr, 0, sizeof(seraddr));
  seraddr.sin_family = AF_INET;//TCP/IP协议族
  seraddr.sin_port = htons(8888);  //服务器端口
  seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //服务器ip

  if(connect(client,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0)
  {
    perror("connect");
    exit(1);
  }
  cout<<"我上线了\n";
  /*
   * int connect(int sockfd,structsockaddr *server_addr,int sockaddr_len)
   * 功能： 同远程服务器建立主动连接，成功时返回0，若连接失败返回－1。
   * 参数说明：
   * Sockfd:套接字描述符，指明创建连接的套接字
   * Server_addr:指明远程端点：IP地址和端口号
   * sockaddr_len :地址长度
  */

  char sendbuf[100];
  char recvbuf[100];
  while(1)
  {
    memset(sendbuf,0,sizeof(sendbuf));
    cout<<"发送：";
    cin>>sendbuf;
    send(client,sendbuf,strlen(sendbuf),0);
    /*
     * int send(int sockfd, const void * data, int data_len, unsigned int flags)
     * 功能：在TCP连接上发送数据,返回成功传送数据的长度，出错时返回－1。send会将数据移到发送缓冲区中。
     * 参数说明：
     * sockfd:套接字描述符
     * data:指向要发送数据的指针
     * data_len:数据长度
     * flags:通常为0
    */
    if(strcmp(sendbuf,"exit")==0) break;
  }
  close(client);
  return 0;
}
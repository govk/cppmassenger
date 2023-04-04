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
  //定义服务端socket描述符

  int server = socket(AF_INET,SOCK_STREAM,0);
  /*
   * int socket( int domain, int type,int protocol)
   * 功能：创建一个新的套接字，返回套接字描述符
   * 参数说明：
   * 第1个参数：domain：域类型，指明使用的协议栈，TCP/IP使用的是PF_INET，其他还有AF_INET6、AF_UNIX
   * 第2个参数：type:指明需要的服务类型, SOCK_DGRAM:数据报服务，UDP协议 SOCK_STREAM:流服务，TCP协议
   * 第3个参数：protocol:一般都取0(由系统根据服务类型选择默认的协议)
  */

  //定义sockaddr_in套接字地址
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8888);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//ip地址:127.0.0.1是环回地址，相当于本机ip   

  /*
   *  struct sockaddr_in {
   *    short int sin_family; //地址族 
   *    unsigned short int sin_port;  //端口号 
   *    struct in_addr sin_addr; // ip地址 
   *    unsigned char sin_zero[8];
   *  };
  */

  //用bind给socket绑定地址，成功返回0，失败返回-1
  if(bind(server,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
  {
    perror("bind");//输出错误原因
    exit(1);//结束程序
  }
  
  /*
   * int bind(int sockfd,struct sockaddr* my_addr,int addrlen)
   * 功能：为套接字绑定地址
   * TCP/IP协议使用sockaddr_in结构，包含IP地址和端口号，服务器使用它来指明熟知的端口号，然后等待连接
   * 参数说明：
   * sockfd:套接字描述符，指明创建连接的套接字
   * my_addr:本地地址，IP地址和端口号
   * addrlen:地址长度
  */

  //用listen吧socket设置成监听模式，成功返回0，失败返回-1
  if(listen(server,20)==-1)
  {
    perror("listen");//输出错误原因
    exit(1);//结束程序
  }

  /*
   * int listen(int sockfd,int backlog)
   * 功能：
   * 将一个套接字置为监听模式，准备接收传入连接。用于服务器，指明某个套接字连接是被动的监听状态。
   * 参数说明：
   * Sockfd:套接字描述符，指明创建连接的套接字
   * backlog: linux内核2.2之前，backlog参数=半连接队列长度+已连接队列长度；linux内核2.2之后，backlog参数=已连接队列（Accept队列）
长度
  */

  //声明客户端套接字
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);

  //从已完成连接队列中去除连接，成功返回非负描述字，出错返回-1
  int conn = accept(server, (struct sockaddr*)&client_addr, &length);
  if(conn<0)
  {
      perror("connect");//输出错误原因
      exit(1);//结束程序
  }
  cout<<"有人上线啦\n";

  /*
   * int accept(int sockfd, structsockaddr *addr, int *addrlen) 
   * 功能：从已完成连接队列中取出成功建立连接的套接字，返回成功连接的套接字描述符。
   * 参数说明：
   * Sockfd:套接字描述符，指明正在监听的套接字
   * addr:提出连接请求的主机地址
   * addrlen:地址长度
  */

  //接收缓冲区
  char buffer[1000];

  //不断接收数据
  while(1)
  {
    memset(buffer,0,sizeof(buffer));
    int len = recv(conn, buffer, sizeof(buffer),0);

    /*
     * int recv(int sockfd, void *buf, intbuf_len,unsigned int flags)
     * 功能：接收数据,返回实际接收的数据长度，出错时返回－1。
     * 参数说明：
     * Sockfd:套接字描述符
     * Buf:指向内存块的指针
     * Buf_len:内存块大小，以字节为单位
     * flags:一般为0
    */
    //客户端发送exit或者异常结束时，退出
    if(strcmp(buffer,"exit")==0 || len<=0) break;
    cout<<"他说："<<buffer<<endl;
  }
  close(conn);
  close(server);

  /*
   * close(int sockfd)
   * 功能：撤销套接字。如果只有一个进程使用，立即终止连接并撤销该套接字，如果多个进程共享该套接字，将引用数减一，如果引用数降到零
，则关闭连接并撤销套接字。
   * 参数说明：
   * sockfd:套接字描述符
  */

  return 0;
}
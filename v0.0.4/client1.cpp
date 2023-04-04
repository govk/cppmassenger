#include "client1.h"

client::client(int port,string ip):server_port(port),server_ip(ip){}
client::~client(){
    close(sock);
}

void client::run(){

    //定义sockfd
    sock = socket(AF_INET,SOCK_STREAM, 0);

    //定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);  //服务器端口
    servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());  //服务器ip

    //连接服务器，成功返回0，错误返回-1
    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    //cout<<"成功连接服务器\n";

    HandleClient(sock);
    return;
}
//注意，前面不用加static！
void client::SendMsg(int conn,string login_name){
    char str1[100];
    while (1)
    {
//      cout<<"["<<login_name<<"]:";
//      string str;
//      cin>>str;
        memset(str1, 0, sizeof(str1));
        scanf("%[^\n]",str1);
        getchar();//吞掉一个回车
        string str=str1;
        //私聊消息
        if(conn>0){
            str="content:"+str;
        }
        //群聊信息
        else if(conn<0){
            str="gr_message:"+str;
        }
        int ret=send(abs(conn), str.c_str(), str.length(),0); //发送
        //输入exit或者对端关闭时结束
        if(str=="content:exit"||str=="gr_message:exit"||ret<=0){
            break;
        }
    }
}
//注意，前面不用加static！
void client::RecvMsg(int conn){
    //接收缓冲区
    char buffer[1000];
    //不断接收数据
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer),0);
        //recv返回值小于等于0，退出
        if(len<=0)  break;
        if(strstr(buffer,"离开私聊！")!=nullptr)  break;
        if(strstr(buffer,"退出群聊！")!=nullptr)  break;
        cout<<buffer<<endl;
    }
}

void client::HandleClient(int conn){
    char choice;                //储存选项
    string name,pass,pass1;     //储存姓名和密码值
    bool if_login=false;//记录是否登录成功
    string login_name;//记录成功登录的用户名

    //发送本地cookie，并接收服务器答复，如果答复通过就不用登录
    //先检查是否存在cookie文件
    ifstream f("cookie.txt");
    string cookie_str;
    if(f.good()){
        f>>cookie_str;
        f.close();
        cookie_str="cookie:"+cookie_str;
        //将cookie发送到服务器
        send(sock,cookie_str.c_str(),cookie_str.length()+1,0);
        //接收服务器答复
        char cookie_ans[100];
        memset(cookie_ans,0,sizeof(cookie_ans));
        recv(sock,cookie_ans,sizeof(cookie_ans),0);
        //判断服务器答复是否通过
        string ans_str(cookie_ans);
        if(ans_str!="NULL"){//redis查询到了cookie，通过
            if_login=true;
            login_name=ans_str;
        }
    }
    if(!if_login){
        cout<<" ------------------\n";
        cout<<"|                  |\n";
        cout<<"|  聊天通信应用:   |\n";
        cout<<"|    0:退出        |\n";
        cout<<"|    1:登录        |\n";
        cout<<"|    2:注册        |\n";
        cout<<"|                  |\n";
        cout<<" ------------------ \n\n";
        cout<<"输入选项:";
    }
    //开始处理各种事务
    while(1){
        if(if_login)  break;
        cin>>choice;
        if(choice=='0')  break;
        //注册
        else if(choice=='2'){
            cout<<"注册的用户名:";
            cin>>name;
            while(1){
                cout<<"密码:";
                cin>>pass;
                cout<<"确认密码:";
                cin>>pass1;
                if(pass==pass1)
                    break;
                else
                    cout<<"两次密码不一致!\n\n";
            }
            name="name:"+name;
            pass="pass:"+pass;
            string str=name+pass;
            send(conn,str.c_str(),str.length(),0);//str.c_str()返回一个指向正规C字符串的指针常量
            cout<<"注册成功！\n";
            cout<<"\n输入选项:";
        }
        //登录
        else if(choice=='1'&&!if_login){
            while(1){
                cout<<"用户名:";
                cin>>name;
                cout<<"密码:";
                cin>>pass;
                //格式化
                string str="login"+name;
                str+="pass:";
                str+=pass;
                send(sock,str.c_str(),str.length(),0);//发送登录信息
                char buffer[1000];
                memset(buffer,0,sizeof(buffer));
                recv(sock,buffer,sizeof(buffer),0);//接收响应
                string recv_str(buffer);
                //登录成功
                if(recv_str.substr(0,2)=="ok"){
                    if_login=true;
                    login_name=name;
                    cout<<"登录成功\n\n";

                    //本地建立cookie文件保存sessionid
                    string tmpstr=recv_str.substr(2);
                    tmpstr="cat > cookie.txt <<end \n"+tmpstr+"\nend";
                    system(tmpstr.c_str());

                    break;
                }
                //登录失败
                else  cout<<"密码或用户名错误！\n\n";
            }
		}
        else cout<<"请按提示输入选项进行操作！"<<endl;
    }
    //登录成功
    if(if_login){
        system("clear");
        cout<<"        欢迎回来,"<<login_name<<endl;
        cout<<" -------------------------------------------\n";
        cout<<"|                                           |\n";
        cout<<"|              0:退出                       |\n";
        cout<<"|              1:发起私聊                   |\n";
        cout<<"|              2:发起群聊                   |\n";
        cout<<"|                                           |\n";
        cout<<" ------------------------------------------- \n\n";
    }
    while(if_login&&1){
        cout<<"\n输入选项:";
        cin>>choice;
        if(choice=='0')
            break;
        //私聊
        else if(choice=='1'){
            cout<<"请输入对方的用户名:";
            string target_name,content;
            cin>>target_name;
            string sendstr("target:"+target_name+"from:"+login_name);//标识目标用户+源用户
            send(sock,sendstr.c_str(),sendstr.length(),0);//先向服务器发送目标用户、源用户
            cout<<"开启私聊(输入exit退出)：\n";
            thread t1(client::SendMsg,conn,login_name); //创建发送线程
            thread t2(client::RecvMsg,conn);//创建接收线程
            t1.join();
            t2.join();
        }
        //群聊
        else if(choice=='2'){
            cout<<"请输入群号:";
            int num;
            cin>>num;
            string sendstr("group:"+to_string(num));
            send(sock,sendstr.c_str(),sendstr.length(),0);
            cout<<"开启群聊(输入exit退出)：\n";
            thread t1(client::SendMsg,-conn,login_name); //创建发送线程，传入负数，和私聊区分开
            thread t2(client::RecvMsg,conn);//创建接收线程
            t1.join();
            t2.join();
        }
        else cout<<"请按提示输入选项进行操作！"<<endl;
    }
    close(sock);
}
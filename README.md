# InstantMassenger

## 简介

**InstantMassenger是即时通信应用，简单的客户端服务端通讯项目**

## 开发工具

- **操作系统**： Ubuntu16.04
- **数据库**：MySQL5.7.33、Redis 3.0.6
- **编辑器**： vim
- **编译器**： g++ 
- **版本控制**： git
- **工程构建**： make
- **调试工具**： gdb

## 功能需求

初始需求：

- 用户可以注册和登录
- 用户之间可以互相私聊
- 用户可以发起、加入群聊

### V0.0.1简单socket通讯

### V0.0.2 多线程客户端、服务端

### V0.0.3 增加连接数据库登录注册私聊群聊

#### mysql环境

```
sudo service mysql start

mysql -uroot -p.....

sudo apt update

sudo apt install -y libmysqlclient-dev

ls /usr/include/mysql|grep mysql.h

mysql -uroot -p.....

create database ChatProject;

use ChatProject;

CREATE TABLE USER(
    NAME VARCHAR(20) PRIMARY KEY,
    PASSWORD VARCHAR(20)
);

show tables;

exit;
```

### V0.0.4 增加用Redis记录登录状态

#### Redis环境

```
cd /usr/bin
./redis-server
另开一个终端，使用如下命令启动客户端：
cd /usr/bin
./redis-cli

wget https://github.com/redis/hiredis/archive/v0.14.0.tar.gz
tar -xzf v0.14.0.tar.gz
cd hiredis-0.14.0/
make
sudo make install
sudo cp /usr/local/lib/libhiredis.so.0.14 /usr/lib/

测试 test_redis.cpp
#include <iostream>
#include <hiredis/hiredis.h>
using namespace std;
int main(){
    redisContext *c = redisConnect("127.0.0.1",6379);
    if(c->err){
       redisFree(c);
       cout<<"连接失败"<<endl;
       return 1;
    }
    cout<<"连接成功"<<endl;
    redisReply *r = (redisReply*)redisCommand(c,"PING");
    cout<<r->str<<endl;
    return 0;
}

g++ -o test_redis test_redis.cpp -lhiredis

KEYS 命令
KEYS *     #使用该命令列出所有key
KEYS a*    #列出所有以a开头的key
KEYS a?b   #列出所有axb格式（x为任意单个字符）的key

EXISTS 命令
EXISTS a    #判断key a是否存在

DEL 命令
DEL a   #删除key a及其value

HSET 123321 name xiaoming   # key为123321 ， value为 （name，xiaoming）的键值对
HGET 123321 name    # 查询key为123321的字典中字段name的具体值
HGETALL 123321      # 查看key为123321的字典中的字段和数据
HSET 123321 name xiaoming age 18 sex male   # 设置key为123321的字典中的字段name为xiaoming，age为18，sex为male
```


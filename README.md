## my_ftp_server
a simple ftp server based on C++, ACE, Linux.

#### 编译

```shell
mkdir build
cd build
cmake ..
make
```



#### 运行

```shell
./server your_ip port_number
```








#### 设计思路：
1. Acceptor 和 Handler 挂载到 ace 的 Reactor 上进行事件监听， acceptor 用于处理连接的建立和关闭，handler 用于已建立连接中请求的处理和响应，使用 ACE_SOCK_Stream 维护实际的连接。

2. server.cc 的 main 函数创建 Acceptor 对象并在 ACE_Reactor 的单例上进行登记，当有客户端请求连接时，Acceptor 创建一个 Handler 对象和一个 User 对象，Handler 对象登记到 ACE_Reactor 上，负责此次连接的命令处理和响应，User 则负责维护客户端信息（没有实现持久化，不能恢复到上次客户端连接断开时的状态）。

3. 对于每个 ftp 命令，Handler 创建一个 Command 对象进行实际的处理，利用 ACE_Thread_Manager 的单例（主线程） spawn 一个新线程进行命令的处理和响应，避免命令处理时间过长而阻塞服务器主线程。
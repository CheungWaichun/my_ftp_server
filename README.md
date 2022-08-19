## my_ftp_server

#### 设计思路：
1. acceptor和handler挂载到ace的Reactor上进行事件监听，acceptor用于处理连接的建立和关闭，handler用于已建立连接中请求的处理和响应，使用ACE_SOCK_Stream维护实际的连接。

2. 鉴于ftp的命令比较多，可以将命令处理逻辑的代码从handler的handle_input()方法中分离出来，写到一个单独的Command.cc文件中。

3. 用User类维护用户信息，可以考虑持久化。

4. 先进行服务器大致框架、控制结构的编写，详细的ftp命令实现可以分开完成，变量命名、注释规范等后面再完善。
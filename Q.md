
### Q1
当前命令在switch结构中没有对应的case时（未实现），不是执行default，而是执行case USER，返回530 user not found! 很奇怪:)

### A1
因为USER在enum结构中对应的数字是0，key（命令）不存在时，unordered_map映射默认返回0，所以会执行case USER。  

### Solution
将ftp命令的enum类型的起始数字设为1（默认从0开始）。

<br/>
<br/>

### note
<!-- main函数写成
```
while(1)
    ACE_Reactor::instance()->handle_events();
```
或者
```
    ACE_Reactor::instance()->run_event_loop();
```
的话server每次开机只能接受一次连接。原因应该是第一个Acceptor注销后，后面就没有新的Acceptor被创建（并register到ACE_Reactor上）用于新连接的处理了。 -->

<br/>

### BUG1
server could accept connection only once.After the first connection closed, server stuck, can't response to a new connection.
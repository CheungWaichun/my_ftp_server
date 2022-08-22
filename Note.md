
### Q1
当前命令在switch结构中没有对应的case时（未实现），不是执行default，而是执行case USER，返回530 user not found! 很奇怪:)

### A1
因为USER在enum结构中对应的数字是0，key（命令）不存在时，unordered_map映射默认返回0，所以会执行case USER。  

### Solution
将ftp命令的enum类型的起始数字设为1（默认从0开始）。

<br/>
<br/>

## note
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

1. some func need data connection to transfer data back to client, before implement these func, impl PORT func first.
Because when ftp build a data connection, it exec PORT command first.


2. after tranfer data through data stream(ACE_SOCK_Stream), how to tell client the data is over? 
Answer: close the stream. client will keep waiting even if a '\n' is transferred.

3. when ordering LIST command after TYPE I Command, client will send command:TYPE A, PORT xxx, LIST.So "WARNING:bare linefeeds received in ASCII mode" always here!





<br/>

## Adjust
1. 
move the right of sock_stream control from Handler to User. let Handler controlling stream makes the code tight coupling.

<br/>

2. 
Before: use Handler to send control_msg back, Command return a std::string to Handler and Handler send it.

disadvantage: every command could send control_msg only once(only at the end of the handle process).Some command need to send more than once, such as LIST:first->"ready to send data."; second->"sent completed!".

Solu: 
1. the process of sending control_msg moves inside the command handle process, controlled by class Command.

2. write a Handler::send_control_msg() method, and set a Handler* ptr in class Command so that it can use Handler::send_control_msg() method. But i think it's not good to let class Command know about the existence of class Handler. it's unsafe and tight coupling.

Now: choose Solu 1.


<br/>

### BUG1
server could accept connection only once.After the first connection closed, server stuck, can't response to a new connection.

<br/>


### SOLVE1
let Acceptor::handle_input() return 0, not -1.

<br/>



#### Warning
bare linefeeds received in ASCII mode

Solu: set transfer type -> I (BINARY), through 'TYPE I' command in server, 'binary' command in client.
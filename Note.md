
### Q1
当前命令在switch结构中没有对应的case时（未实现），不是执行default，而是执行case USER，返回530 user not found! 很奇怪:)

### A1
因为USER在enum结构中对应的数字是0，key（命令）不存在时，unordered_map映射默认返回0，所以会执行case USER。  

### Solution
将ftp命令的enum类型的起始数字设为1（默认从0开始）。





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
Detailed reason shows below.(about CRLF)

<img src=".\.Note_images\image-20220823102419717.png" alt="image-20220823102419717" style="zoom: 67%;" />

After test, i know that the FILE which popen() method returns, each row ends with LF, not CRLF. When i modify it into CRLF, the warning is gone! The comment in the picture above is totally right.



**when transfer in ASCII mode, server should make sure that every line of data_msg ends with <CRLF> !!!**  **then clients translate "\r\n" into their system's perform, **

**for instance, unix client: "\r\n" --> '\n', windows client: "\r\n" --> "\r\n". I have verified this:**

<img src=".\.Note_images\$.png" style="zoom: 67%;" />

<img src=".\.Note_images\^M.png" style="zoom: 67%;" />


4. 客户端在 Windows 环境时，可能需要关闭系统防火墙，不然Connector::connect() 方法发出的连接请求会被过滤。



<br/>




## Adjust
1. 
move the right of sock_stream control from Handler to User. let Handler controlling stream makes the code tight coupling.



2. **Before:** use Handler to send control_msg back, Command return a std::string to Handler and Handler send it.

**disadvantage:** every command could send control_msg only once(only at the end of the handle process).Some command need to send more than once, such as LIST:first->"ready to send data."; second->"sent completed!".

**Solu:** 

1. the process of sending control_msg moves inside the command handle process, controlled by class Command.

2. write a Handler::send_control_msg() method, and set a Handler* ptr in class Command so that it can use Handler::send_control_msg() method. But i think it's not good to let class Command know about the existence of class Handler. it's unsafe and tight coupling.

**Now:** choose Solu 1.

3. 将‘通过数据连接发送’的逻辑独立出来，写到User类中，免得每个要发送数据的command都要自己init_data_stream.


### BUG1
server could accept connection only once.After the first connection closed, server stuck, can't response to a new connection.




### SOLVE1
let Acceptor::handle_input() return 0, not -1.





#### Warning

1.  bare linefeeds received in ASCII mode

Solu: 
(1)set transfer type -> I (BINARY), through 'TYPE I' command in server, 'binary' command in client.(X)
(2)see note 3.
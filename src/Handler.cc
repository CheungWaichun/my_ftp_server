#include "../include/Handler.h"
#include <iostream>

Handler::Handler(){
    // this->user = NULL;
}

Handler::~Handler(){

}

int Handler::set_user(User* user){
    this->user = user;
    return 0;
}

int Handler::set_stream(ACE_SOCK_Stream stream){
    this->stream = stream;
    return 0;
}

// int Handler::register_read(){

// }

int Handler::handle_input(ACE_HANDLE){
    // std::cout<<"handle_input called"<<std::endl;
    memset(data, 0, 128);
    int count = this->stream.recv(data, 128);
    ACE_DEBUG((LM_DEBUG, "%s\n", data));
    std::cout<<"data is:"<<data<<std::endl;
    

    //命令处理逻辑
    // ACE_DEBUG((LM_DEBUG,ACE_TEXT("i am command handler!Waiting to be implemented!\n")));
    // std::cout<<"i am command handler!Waiting to be implemented!\n"<<std::endl;
    Command* cmd = new Command(data, this->user);
    std::string ret = cmd->handle();
    std::cout<<"ret is:" << ret<<std::endl;
    // this->stream.send("331 OK.\n", 10);
    // std::cout<<"ret size is:"<<ret.size()<<std::endl;
    this->stream.send(ret.c_str(), ret.length());
    ret.clear();
    return 0;
}

ACE_HANDLE Handler::get_handle() const{
    return this->stream.get_handle();
}

ACE_SOCK_Stream& Handler::get_stream(){
    return this->stream;
}
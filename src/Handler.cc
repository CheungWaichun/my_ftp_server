#include "../include/Handler.h"
#include <iostream>

Handler::Handler(){
    // this->user = NULL;
}

Handler::~Handler(){

}

int Handler::set_user(User* user){
    this->user = user;
}

int Handler::set_stream(ACE_SOCK_Stream stream){
    this->stream = stream;
}

// int Handler::register_read(){

// }

int Handler::handle_input(ACE_HANDLE){
    this->stream.recv_n(data, 12);
    // printf("hi");
    ACE_DEBUG((LM_DEBUG, "%s\n", data));
    //命令处理逻辑
    // printf("hi");
    ACE_DEBUG((LM_DEBUG,ACE_TEXT("i am command handler!Waiting to be implemented!\n")));

    return 0;
}

ACE_HANDLE Handler::get_handle() const{
    return this->stream.get_handle();
}

ACE_SOCK_Stream& Handler::get_stream(){
    return this->stream;
}
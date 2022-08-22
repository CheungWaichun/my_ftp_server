#include "../include/Handler.h"
#include <iostream>

Handler::Handler(){
    // this->user = NULL;
    std::cout<<"i am a new Handler."<<std::endl;
}

Handler::~Handler(){
    std::cout<<"i am a Handler, i am dying."<<std::endl;
}

int Handler::set_user(User* user){
    this->user = user;
    return 0;
}

int Handler::set_control_stream(ACE_SOCK_Stream stream){
    this->control_stream = stream;
    return 0;
}

// int Handler::register_read(){

// }

int Handler::handle_input(ACE_HANDLE fd){
    // std::cout<<"handle_input called"<<std::endl;
    memset(this->command, 0, 128);
    int count = this->control_stream.recv(this->command, 128);
    ACE_DEBUG((LM_DEBUG, "%s\n", this->command));
    std::cout<<"command is:"<<this->command<<std::endl;
    

    //命令处理逻辑

    Command* cmd = new Command(this->command, this->user);

    //
    this->user->set_control_stream(this->control_stream);


    cmd->handle();

    return 0;
}

ACE_HANDLE Handler::get_handle() const{
    return this->control_stream.get_handle();
}

ACE_SOCK_Stream& Handler::get_control_stream(){
    return this->control_stream;
}
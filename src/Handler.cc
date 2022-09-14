#include "../include/Handler.h"
#include <iostream>

Handler::Handler(){
    // std::cout<<"i am a new Handler."<<std::endl;
}

Handler::~Handler(){
    // std::cout<<"i am a Handler, i am dying."<<std::endl;
}

int Handler::set_user(User* user){
    this->user = user;
    return 0;
}

int Handler::set_control_stream(ACE_SOCK_Stream stream){
    this->control_stream = stream;
    return 0;
}

int Handler::handle_input(ACE_HANDLE fd){
    memset(this->command, 0, 128);
    int count = this->control_stream.recv(this->command, 128);
    ACE_DEBUG((LM_DEBUG, "%s\n", this->command));
    std::cout<<"command is:"<<this->command<<std::endl;

    Command* cmd = new Command(this->command, this->user);
    this->user->set_control_stream(this->control_stream);
    //spawn一个线程进行命令处理
    ACE_Thread_Manager::instance()->spawn(run_svc, cmd, THR_NEW_LWP | THR_DETACHED);
    return 0;
}

ACE_HANDLE Handler::get_handle() const{
    return this->control_stream.get_handle();
}

ACE_SOCK_Stream& Handler::get_control_stream(){
    return this->control_stream;
}

ACE_THR_FUNC_RETURN Handler::run_svc(void *arg){
    // std::cout<<"run_svc tid:"<<syscall(SYS_gettid)<<std::endl;
    Command* cmd = static_cast<Command*>(arg);
    cmd->handle();
    return 0;
}